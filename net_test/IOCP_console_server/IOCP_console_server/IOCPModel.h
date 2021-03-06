﻿#pragma once

// winsock 2 的头文件和库
#include <winsock2.h>

#include <MSWSock.h>
#include <Windows.h>
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

// 缓冲区长度 (1024*8)
// 之所以为什么设置8K，也是一个江湖上的经验值
// 如果确实客户端发来的每组数据都比较少，那么就设置得小一些，省内存
#define MAX_BUFFER_LEN 8192
// 默认端口
#define DEFAULT_PORT 12345
// 默认IP地址
#define DEFAULT_IP ("127.0.0.1")

// 在完成端口上投递的I/O操作的类型
typedef enum _OPERATION_TYPE {
  ACCEPT_POSTED, // 标志投递的Accept操作
  SEND_POSTED,   // 标志投递的是发送操作
  RECV_POSTED,   // 标志投递的是接收操作
  NULL_POSTED    // 用于初始化，无意义

} OPERATION_TYPE;

//单IO数据结构体定义(用于每一个重叠操作的参数)
typedef struct _PER_IO_CONTEXT {
  // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
  OVERLAPPED m_Overlapped;
  SOCKET m_sockAccept; // 这个网络操作所使用的Socket
  WSABUF m_wsaBuf; // WSA类型的缓冲区，用于给重叠操作传参数的
  char m_szBuffer[MAX_BUFFER_LEN]; // 这个是WSABUF里具体存字符的缓冲区
  OPERATION_TYPE m_OpType; // 标识网络操作的类型(对应上面的枚举)

  _PER_IO_CONTEXT() {
    ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
    ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
    m_sockAccept = INVALID_SOCKET;
    m_wsaBuf.buf = m_szBuffer;
    m_wsaBuf.len = MAX_BUFFER_LEN;
    m_OpType = NULL_POSTED;
  }
  // 释放掉Socket
  ~_PER_IO_CONTEXT() {
    if (m_sockAccept != INVALID_SOCKET) {
      closesocket(m_sockAccept);
      m_sockAccept = INVALID_SOCKET;
    }
  }
  // 重置缓冲区内容
  void ResetBuffer() { ZeroMemory(m_szBuffer, MAX_BUFFER_LEN); }
} PER_IO_CONTEXT, *PPER_IO_CONTEXT;

//单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
typedef struct _PER_SOCKET_CONTEXT {
  SOCKET m_Socket;          // 每一个客户端连接的Socket
  SOCKADDR_IN m_ClientAddr; // 客户端的地址
  // 客户端网络操作的上下文数据，也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的
  std::vector<_PER_IO_CONTEXT *> m_arrayIoContext;

  _PER_SOCKET_CONTEXT() {
    m_Socket = INVALID_SOCKET;
    memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
  }

  // 释放资源
  ~_PER_SOCKET_CONTEXT() {
    if (m_Socket != INVALID_SOCKET) {
      closesocket(m_Socket);
      m_Socket = INVALID_SOCKET;
    }
    // 释放掉所有的IO上下文数据
    for (int i = 0; i < m_arrayIoContext.size(); i++) {
      delete m_arrayIoContext.at(i);
    }
    m_arrayIoContext.clear();
  }

  // 获取一个新的IoContext
  _PER_IO_CONTEXT *GetNewIoContext() {
    _PER_IO_CONTEXT *p = new _PER_IO_CONTEXT;
    m_arrayIoContext.push_back(p);
    return p;
  }

  // 从数组中移除一个指定的IoContext
  void RemoveContext(_PER_IO_CONTEXT *pContext) {
    assert(pContext != NULL);

    std::vector<_PER_IO_CONTEXT *>::iterator it = m_arrayIoContext.begin();
    for (; it != m_arrayIoContext.end(); ++it) {
      if (pContext == *it) {
        delete pContext;
        pContext = NULL;
        m_arrayIoContext.erase(it);
        break;
      }
    }
  }

} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

// 工作者线程的线程参数
class CIOCPModel;
typedef struct _tagThreadParams_WORKER {
  CIOCPModel *pIOCPModel; // 类指针，用于调用类中的函数
  int nThreadNo;          // 线程编号

} THREADPARAMS_WORKER, *PTHREADPARAM_WORKER;

// CIOCPModel类
class CIOCPModel {
public:
  CIOCPModel(void);
  ~CIOCPModel(void);

public:
  // 启动服务器
  bool InitServer();

  //	停止服务器
  void Stop();

  // 加载Socket库
  inline bool LoadSocketLib() {
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 错误
    if (NO_ERROR != nResult) {
      printf("初始化WinSock 2.2失败！\n");
      return false;
    }

    return true;
  }

  // 卸载Socket库，彻底完事
  void UnloadSocketLib() { WSACleanup(); }

  // 获得本机的IP地址
  std::string GetLocalIP();

  // 设置监听端口
  void SetPort(const int &port) { nPort = port; }

protected:
  bool InitWorkerThread();

  // 初始化Socket
  bool InitListenSocket();

  bool GetAccpetExPoints(SOCKET& m_Socket);

  // 最后释放资源
  void ReleaseAllRes();

  // 投递Accept请求
  bool PostAccept(PER_IO_CONTEXT *pAcceptIoContext);

  // 投递接收数据请求
  bool PostRecv(PER_IO_CONTEXT *pIoContext);

  // 在有客户端连入的时候，进行处理
  bool DoAccpet(PER_SOCKET_CONTEXT *pSocketContext, PER_IO_CONTEXT *pIoContext);

  // 在有接收的数据到达的时候，进行处理
  bool DoRecv(PER_SOCKET_CONTEXT *pSocketContext, PER_IO_CONTEXT *pIoContext);

  // 将客户端的相关信息存储到数组中
  void AddToContextList(PER_SOCKET_CONTEXT *pSocketContext);

  // 将客户端的信息从数组中移除
  void RemoveContext(PER_SOCKET_CONTEXT *pSocketContext);

  // 清空客户端信息
  void ClearContextList();

  // 将句柄绑定到完成端口中
  bool AssociateWithIOCP(PER_SOCKET_CONTEXT *pContext);

  // 处理完成端口上的错误
  bool HandleError(PER_SOCKET_CONTEXT *pContext);

  // 线程函数，为IOCP请求服务的工作者线程
  static DWORD WINAPI WorkerThread(LPVOID lpParam);

  // 获得本机的处理器数量
  int GetNumberOfProcessors();

  // 判断客户端Socket是否已经断开
  bool IsSocketAlive(SOCKET s);

  void ShowMessage(const std::string szFormat, ...) const;

private:
  HANDLE hServerStopEvent; // 用来通知线程系统退出的事件，为了能够更好的退出线程
  HANDLE hCompletionPort;         // 完成端口的句柄
  HANDLE *phWorkerThreads;        // 工作者线程组的句柄指针
  int nThreads;                   // 生成的线程数量
  std::string strIP;              // 服务器端的IP地址
  int nPort;                      // 服务器端的监听端口
  CRITICAL_SECTION csContextList; // 用于Worker线程同步的互斥量
  std::vector<PER_SOCKET_CONTEXT *>
      vecClientContext;               // 客户端Socket的Context信息
  PER_SOCKET_CONTEXT *pListenContext; // 用于监听的Socket的Context信息
  LPFN_ACCEPTEX lpfnAcceptEx;         // AcceptEx 和 GetAcceptExSockaddrs
                              // 的函数指针，用于调用这两个扩展函数
  LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs;
};
