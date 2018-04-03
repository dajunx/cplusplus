#include <iostream>
//#include <windows.h>
#include <WINSOCK2.H>
#include <STDIO.H>

#pragma  comment(lib,"ws2_32.lib")

void showSubNetErr(bool& continueConn)
{
  switch(WSAGetLastError()) {
  case WSAECONNREFUSED:
    {
      std::cout<<"Connection refused."<<std::endl;
      continueConn = true;
      break;
    }
  default:
    ;
    //std::cout<<"unknow err."<<std::endl;
  }
}

int main(int argc, char* argv[])
{
  WORD sockVersion = MAKEWORD(2,2);
  WSADATA data; 
  if(WSAStartup(sockVersion, &data) != 0)
  {
    return 0;
  }

  SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sclient == INVALID_SOCKET)
  {
    printf("invalid socket !");
    return 0;
  }

  sockaddr_in serAddr;
  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(8888);
  serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  while(true)
  {
    int ret_conn = connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr));
    if (ret_conn == SOCKET_ERROR) {
      std::cout<<"connect server err:"<<WSAGetLastError()<<std::endl;
      break;
    }

    bool continueConn = false;
    showSubNetErr(continueConn);
    if (continueConn) {
      Sleep(1000);
      continue;
    }

    //接收客户端发送的数据并打印
    while (true) {
      char recData[255];
      int ret = recv(sclient, recData, 255, 0);
      if(ret > 0)
      {
        recData[ret] = 0x00;
        std::cout<<recData/*<<std::endl*/;
      } else {
        std::cout<<"recv server data err:"<<WSAGetLastError()<<std::endl;
        break;
      }
    }
  }

  closesocket(sclient);
  WSACleanup();
  return 0;
}