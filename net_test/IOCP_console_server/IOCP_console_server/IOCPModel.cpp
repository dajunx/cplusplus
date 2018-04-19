#include "IOCPModel.h"

// 每一个处理器上产生多少个线程(为了最大限度的提升服务器性能，详见配套文档)
#define WORKER_THREADS_PER_PROCESSOR 2
// 同时投递的Accept请求的数量(这个要根据实际的情况灵活设置)
#define MAX_POST_ACCEPT 10
// 传递给Worker线程的退出信号
#define EXIT_CODE NULL

#define RELEASE_POINT(x)                                                       \
  {                                                                            \
    if (x != NULL) {                                                           \
      delete x;                                                                \
      x = NULL;                                                                \
    }                                                                          \
  }

#define RELEASE_HANDLE(x)                                                      \
  {                                                                            \
    if (x != NULL && x != INVALID_HANDLE_VALUE) {                              \
      CloseHandle(x);                                                          \
      x = NULL;                                                                \
    }                                                                          \
  }

#define RELEASE_SOCKET(x)                                                      \
  {                                                                            \
    if (x != INVALID_SOCKET) {                                                 \
      closesocket(x);                                                          \
      x = INVALID_SOCKET;                                                      \
    }                                                                          \
  }

CIOCPModel::CIOCPModel(void)
    : nThreads(0), hServerStopEvent(NULL), hCompletionPort(NULL),
      phWorkerThreads(NULL), strIP(DEFAULT_IP), nPort(DEFAULT_PORT),
      lpfnAcceptEx(NULL), pListenContext(NULL) {}

CIOCPModel::~CIOCPModel(void) { this->Stop(); }

//工作者线程：为IOCP的请求服务的工作者线程，即每当完成端口上出现完成数据包，就将之取出来进行处理
DWORD WINAPI CIOCPModel::WorkerThread(LPVOID lpParam) {
  THREADPARAMS_WORKER *pParam = (THREADPARAMS_WORKER *)lpParam;
  CIOCPModel *pIOCPModel = (CIOCPModel *)pParam->pIOCPModel;
  int nThreadNo = (int)pParam->nThreadNo;

  printf("工作者线程启动，ID: %d.\n", nThreadNo);

  OVERLAPPED *pOverlapped = NULL;
  PER_SOCKET_CONTEXT *pSocketContext = NULL;
  DWORD dwBytesTransfered = 0;

  // 循环处理请求，直到接收到Shutdown信息为止
  while (WAIT_OBJECT_0 !=
         WaitForSingleObject(pIOCPModel->hServerStopEvent, 0)) {
    BOOL bReturn = GetQueuedCompletionStatus(
        pIOCPModel->hCompletionPort, &dwBytesTransfered,
        (PULONG_PTR)&pSocketContext, &pOverlapped, INFINITE);

    // 如果收到的是退出标志，则直接退出
    if (EXIT_CODE == (DWORD)pSocketContext) {
      break;
    }

    // 判断是否出现了错误
    if (!bReturn) {
      // 显示一下提示信息
      if (!pIOCPModel->HandleError(pSocketContext)) {
        break;
      }

      continue;
    }

    // 读取传入的参数
    PER_IO_CONTEXT *pIoContext =
        CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);

    // 判断是否有客户端断开了
    if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType ||
                                     SEND_POSTED == pIoContext->m_OpType)) {
      printf("客户端 %s:%d 断开连接.\n",
             inet_ntoa(pSocketContext->m_ClientAddr.sin_addr),
             ntohs(pSocketContext->m_ClientAddr.sin_port));

      // 释放掉对应的资源
      pIOCPModel->RemoveContext(pSocketContext);
      continue;
    }

    switch (pIoContext->m_OpType) {
    case ACCEPT_POSTED: {
      // 处理连入请求
      pIOCPModel->DoAccpet(pSocketContext, pIoContext);
    } break;
    case RECV_POSTED: {
      // 处理接收请求
      pIOCPModel->DoRecv(pSocketContext, pIoContext);
    } break;

      // SEND 略
    case SEND_POSTED: {

    } break;
    default:
      // 不应该执行到这里
      std::cout << ("_WorkThread中的 pIoContext->m_OpType 参数异常.\n")
                << std::endl;
      break;
    }
  }

  std::cout << "工作者线程" << nThreadNo << " 号退出.\n" << std::endl;

  // 释放线程参数
  RELEASE_POINT(lpParam);
  return 0;
}

// 初始化业务层服务
bool CIOCPModel::InitServer() {
  // 初始化线程互斥量
  InitializeCriticalSection(&csContextList);

  // 建立系统退出的事件通知
  hServerStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  bool ret = false;
  do {
    hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    if (NULL == hCompletionPort) {
      printf("建立完成端口失败！错误代码: %d!\n", WSAGetLastError());
      break;
    }

    InitWorkerThread(); // 函数实现没有返回false case

    // 初始化Socket
    if (false == InitListenSocket()) {
      printf("Listen Socket初始化失败！\n");
      this->ReleaseAllRes();
      break;
    }

    printf("系统准备就绪，等候连接....\n");

    ret = true;
  } while (false);

  return ret;
}

//	开始发送系统退出消息，退出完成端口和线程资源
void CIOCPModel::Stop() {
  if (pListenContext != NULL && pListenContext->m_Socket != INVALID_SOCKET) {
    // 激活关闭消息通知
    SetEvent(hServerStopEvent);

    for (int i = 0; i < nThreads; i++) {
      // 通知所有的完成端口操作退出
      PostQueuedCompletionStatus(hCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
    }

    // 等待所有的客户端资源退出
    WaitForMultipleObjects(nThreads, phWorkerThreads, TRUE, INFINITE);

    // 清除客户端列表信息
    this->ClearContextList();

    // 释放其他资源
    this->ReleaseAllRes();

    printf("停止监听\n");
  }
}

// 根据机器处理器数量建立对应数目线程
bool CIOCPModel::InitWorkerThread() {
  nThreads = WORKER_THREADS_PER_PROCESSOR * GetNumberOfProcessors();

  phWorkerThreads = new HANDLE[nThreads];

  DWORD nThreadID;
  for (int i = 0; i < nThreads; i++) {
    THREADPARAMS_WORKER *pThreadParams = new THREADPARAMS_WORKER;
    pThreadParams->pIOCPModel = this;
    pThreadParams->nThreadNo = i + 1;
    phWorkerThreads[i] = CreateThread(
        0, 0, WorkerThread, static_cast<void *>(pThreadParams), 0, &nThreadID);
  }

  printf("建立了%d个 WorkerThread .\n", nThreads);
  return true;
}

bool CIOCPModel::InitListenSocket() {
  // 服务器地址信息，用于绑定Socket
  struct sockaddr_in ServerAddress;

  // 生成用于监听的Socket的信息
  pListenContext = new PER_SOCKET_CONTEXT;

  // 需要使用重叠IO，必须使用支持重叠IO操作的 WSASocket函数来建立Socket
  pListenContext->m_Socket =
      WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == pListenContext->m_Socket) {
    printf("初始化Socket失败，错误代码: %d.\n", WSAGetLastError());
    return false;
  }

  // 将Listen Socket绑定至完成端口中
  if (NULL == AssociateWithIOCP(pListenContext)) {
    RELEASE_SOCKET(pListenContext->m_Socket);
    return false;
  }

  // 填充地址信息
  ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
  ServerAddress.sin_family = AF_INET;
  // ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定任何IP
  ServerAddress.sin_addr.s_addr = inet_addr(strIP.c_str());
  ServerAddress.sin_port = htons(nPort);

  if (SOCKET_ERROR == bind(pListenContext->m_Socket,
                           (struct sockaddr *)&ServerAddress,
                           sizeof(ServerAddress))) {
    printf("bind()函数执行错误. err_code:%d\n", WSAGetLastError());
    return false;
  }

  if (SOCKET_ERROR == listen(pListenContext->m_Socket, SOMAXCONN)) {
    printf("Listen()函数执行出现错误, err_code:%d\n", WSAGetLastError());
    return false;
  }

  // 获取 AcceptEx、GetAcceptExSockaddrs 函数指针
  if (false == GetAccpetExPoints(pListenContext->m_Socket)) {
    return false;
  }

  // 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
  for (int i = 0; i < MAX_POST_ACCEPT; i++) {
    PER_IO_CONTEXT *pAcceptIoContext = pListenContext->GetNewIoContext();

    if (false == this->PostAccept(pAcceptIoContext)) {
      pListenContext->RemoveContext(pAcceptIoContext);
      return false;
    }
  }

  printf("投递 %d 个AcceptEx请求完毕\n", MAX_POST_ACCEPT);
  return true;
}

bool CIOCPModel::GetAccpetExPoints(SOCKET &m_Socket) {
  // AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
  GUID GuidAcceptEx = WSAID_ACCEPTEX;
  GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

  // 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
  // 所以需要额外获取一下函数的指针，获取AcceptEx函数指针(目的：为了效率，避免对象重复构造析构)
  DWORD dwBytes = 0;
  if (SOCKET_ERROR ==
      WSAIoctl(pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
               &GuidAcceptEx, sizeof(GuidAcceptEx), &lpfnAcceptEx,
               sizeof(lpfnAcceptEx), &dwBytes, NULL, NULL)) {
    printf("WSAIoctl未能获取AcceptEx函数指针，错误代码: %d\n",
           WSAGetLastError());
    this->ReleaseAllRes();
    return false;
  }

  // 获取GetAcceptExSockAddrs函数指针，也是同理
  if (SOCKET_ERROR ==
      WSAIoctl(pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
               &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs),
               &lpfnGetAcceptExSockAddrs, sizeof(lpfnGetAcceptExSockAddrs),
               &dwBytes, NULL, NULL)) {
    printf("WSAIoctl 未能获取GuidGetAcceptExSockAddrs函数指针。错误代码: %d\n",
           WSAGetLastError());
    this->ReleaseAllRes();
    return false;
  }

  return true;
}

//	最后释放掉所有资源
void CIOCPModel::ReleaseAllRes() {
  // 删除客户端列表的互斥量
  DeleteCriticalSection(&csContextList);

  // 关闭系统退出事件句柄
  RELEASE_HANDLE(hServerStopEvent);

  // 释放工作者线程句柄指针
  for (int i = 0; i < nThreads; i++) {
    RELEASE_HANDLE(phWorkerThreads[i]);
  }

  RELEASE_POINT(phWorkerThreads);

  // 关闭IOCP句柄
  RELEASE_HANDLE(hCompletionPort);

  // 关闭监听Socket
  RELEASE_POINT(pListenContext);
  printf("释放资源完毕.\n");
}

// 投递异步Accept请求
bool CIOCPModel::PostAccept(PER_IO_CONTEXT *pAcceptIoContext) {
  assert(INVALID_SOCKET != pListenContext->m_Socket);

  // 准备参数
  DWORD dwBytes = 0;
  pAcceptIoContext->m_OpType = ACCEPT_POSTED;
  WSABUF *p_wbuf = &pAcceptIoContext->m_wsaBuf;
  OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;

  // 为以后新连入的客户端先准备好Socket( 这个是与传统accept最大的区别 )
  pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,
                                             NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept) {
    printf("创建用于Accept的Socket失败！错误代码: %d\n", WSAGetLastError());
    return false;
  }

  // 投递 AcceptEx
  if (FALSE == lpfnAcceptEx(pListenContext->m_Socket,
                            pAcceptIoContext->m_sockAccept, p_wbuf->buf,
                            p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
                            sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
                            &dwBytes, p_ol)) {
    if (WSA_IO_PENDING != WSAGetLastError()) {
      printf("投递 AcceptEx 请求失败，错误代码: %d\n", WSAGetLastError());
      return false;
    }
  }

  return true;
}

// 客户端连入的响应流程
// 其中，我们需要新建一份PER_SOCKET_CONTEXT给新连入的Socket用
// 传入的的PER_IO_CONTEXT还需要继续投递用来响应下一个Accept请求
bool CIOCPModel::DoAccpet(PER_SOCKET_CONTEXT *pSocketContext,
                          PER_IO_CONTEXT *pIoContext) {
  SOCKADDR_IN *ClientAddr = NULL;
  SOCKADDR_IN *LocalAddr = NULL;
  int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);

  // 1. 取得客户端和本地端的地址信息 以及 客户端发来的第一组数据
  this->lpfnGetAcceptExSockAddrs(
      pIoContext->m_wsaBuf.buf,
      pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
      sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
      (LPSOCKADDR *)&LocalAddr, &localLen, (LPSOCKADDR *)&ClientAddr,
      &remoteLen);

  printf("客户端 %s:%d 连入，第一份信息:%s.\n", inet_ntoa(ClientAddr->sin_addr),
         ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);

  // 2.这里需要注意，本函数传入的是ListenSocket上的PER_IO_CONTEXT，只需要拷贝其socket即可，
  // 新建一个PER_SOCKET_CONTEXT用以接受客户端数据交互
  PER_SOCKET_CONTEXT *pNewSocketContext = new PER_SOCKET_CONTEXT;
  pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
  // 唯一获取客户端IP port信息渠道
  memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

  // 3.参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
  if (false == this->AssociateWithIOCP(pNewSocketContext)) {
    RELEASE_POINT(pNewSocketContext);
    return false;
  }

  // 4.继续，建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
  PER_IO_CONTEXT *pNewIoContext = pNewSocketContext->GetNewIoContext();
  pNewIoContext->m_OpType = RECV_POSTED;
  pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;

  // 如果Buffer需要保留，就自己拷贝一份出来
  // memcpy( pNewIoContext->m_szBuffer, pIoContext->m_szBuffer, MAX_BUFFER_LEN
  // );

  // 5.绑定完毕之后，就可以开始在这个Socket上投递完成请求了
  if (false == this->PostRecv(pNewIoContext)) {
    pNewSocketContext->RemoveContext(pNewIoContext);
    return false;
  }

  // 6.如果投递成功，那么就把这个有效的客户端信息，加入到ContextList中去(需要统一管理，方便释放资源)
  this->AddToContextList(pNewSocketContext);

  // 7.使用完毕之后，把ListenSocket的那个PER_IO_CONTEXT重置，然后响应新的AcceptEx
  pIoContext->ResetBuffer();
  return this->PostAccept(pIoContext);
}

////////////////////////////////////////////////////////////////////
// 投递接收数据请求
bool CIOCPModel::PostRecv(PER_IO_CONTEXT *pIoContext) {
  // 初始化变量
  DWORD dwFlags = 0;
  DWORD dwBytes = 0;
  WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
  OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

  pIoContext->ResetBuffer();
  pIoContext->m_OpType = RECV_POSTED;

  // 初始化完成后，投递WSARecv请求
  int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes,
                           &dwFlags, p_ol, NULL);

  // 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
  if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError())) {
    printf("投递第一个WSARecv失败,err_code:%d！\n", WSAGetLastError());
    return false;
  }
  return true;
}

// 在有接收的数据到达的时候，进行处理
bool CIOCPModel::DoRecv(PER_SOCKET_CONTEXT *pSocketContext,
                        PER_IO_CONTEXT *pIoContext) {
  // 先把上一次的数据显示出现，然后就重置状态，发出下一个Recv请求
  SOCKADDR_IN *ClientAddr = &pSocketContext->m_ClientAddr;
  printf("收到  %s:%d 信息：%s\n", inet_ntoa(ClientAddr->sin_addr),
         ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);

  // 投递下一个WSARecv请求
  return PostRecv(pIoContext);
}

// 将句柄(Socket)绑定到完成端口中
bool CIOCPModel::AssociateWithIOCP(PER_SOCKET_CONTEXT *pContext) {
  // 将用于和客户端通信的SOCKET绑定到完成端口中
  if (NULL == CreateIoCompletionPort((HANDLE)pContext->m_Socket,
                                     hCompletionPort, (DWORD)pContext, 0)) {
    printf(("绑定socket到IOCP失败.err_code：%d\n"), GetLastError());
    return false;
  }

  return true;
}

// 将客户端的相关信息存储到数组中
void CIOCPModel::AddToContextList(PER_SOCKET_CONTEXT *pHandleData) {
  EnterCriticalSection(&csContextList);

  vecClientContext.push_back(pHandleData);

  LeaveCriticalSection(&csContextList);
}

//	移除某个特定的Context
void CIOCPModel::RemoveContext(PER_SOCKET_CONTEXT *pSocketContext) {
  EnterCriticalSection(&csContextList);

  std::vector<PER_SOCKET_CONTEXT *>::iterator it = vecClientContext.begin();
  for (; it != vecClientContext.end(); ++it) {
    if (pSocketContext == *it) {
      RELEASE_POINT(pSocketContext);
      vecClientContext.erase(it);
      break;
    }
  }

  LeaveCriticalSection(&csContextList);
}

// 清空客户端信息
void CIOCPModel::ClearContextList() {
  EnterCriticalSection(&csContextList);

  std::vector<PER_SOCKET_CONTEXT *>::iterator it = vecClientContext.begin();
  for (; it != vecClientContext.end(); ++it) {
    delete *it;
  }
  vecClientContext.clear();

  LeaveCriticalSection(&csContextList);
}

// 获得本机的IP地址
std::string CIOCPModel::GetLocalIP() {
  // 获得本机主机名
  char hostname[MAX_PATH] = {0};
  gethostname(hostname, MAX_PATH);
  struct hostent FAR *lpHostEnt = gethostbyname(hostname);
  if (lpHostEnt == NULL) {
    return DEFAULT_IP;
  }

  // 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)
  LPSTR lpAddr = lpHostEnt->h_addr_list[0];

  // 将IP地址转化成字符串形式
  struct in_addr inAddr;
  memmove(&inAddr, lpAddr, 4);
  // m_strIP = CString( inet_ntoa(inAddr) );
  strIP.append(inet_ntoa(inAddr));

  return strIP;
}

// 获得本机中处理器的数量
int CIOCPModel::GetNumberOfProcessors() {
  SYSTEM_INFO si;

  GetSystemInfo(&si);

  return si.dwNumberOfProcessors;
}

// 在主界面中显示提示信息
void CIOCPModel::ShowMessage(const std::string szFormat, ...) const {
  // 根据传入的参数格式化字符串
  std::string strMessage;
  va_list arglist;

  // 处理变长参数
  va_start(arglist, szFormat);
  // strMessage.FormatV(szFormat,arglist);
  vprintf(szFormat.c_str(), arglist);
  va_end(arglist);

  /// TODO 消息后面的参数没有合入
  std::cout << "message: " << szFormat << std::endl;
}

/////////////////////////////////////////////////////////////////////
// 判断客户端Socket是否已经断开，否则在一个无效的Socket上投递WSARecv操作会出现异常
// 使用的方法是尝试向这个socket发送数据，判断这个socket调用的返回值
// 因为如果客户端网络异常断开(例如客户端崩溃或者拔掉网线等)的时候，服务器端是无法收到客户端断开的通知的
bool CIOCPModel::IsSocketAlive(SOCKET s) {
  int nByteSent = send(s, "", 0, 0);
  if (-1 == nByteSent) {
    return false;
  }

  return true;
}

// 显示并处理完成端口上的错误
bool CIOCPModel::HandleError(PER_SOCKET_CONTEXT *pContext) {
  DWORD dwErr = GetLastError();
  switch (dwErr) {
  case WAIT_TIMEOUT: // 超时，继续等待
  {
    // 确认客户端是否还活着...
    if (!IsSocketAlive(pContext->m_Socket)) {
      printf("检测到客户端异常退出！\n");
      this->RemoveContext(pContext);
      return true;
    } else {
      printf("网络操作超时！重试中...\n");
      return true;
    }
  }
  case ERROR_NETNAME_DELETED: {
    // 可能是客户端异常退出了
    printf("检测到客户端异常退出！\n");
    this->RemoveContext(pContext);
    return true;
  }
  default: {
    printf("完成端口操作出现错误，线程退出。错误代码：%d\n", dwErr);
    return false;
  }
  }
}