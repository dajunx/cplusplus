#include <stdio.h>
#include <winsock2.h>

#include <vector>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
  WORD socketVersion = MAKEWORD(2,2);
  WSADATA wsaData; 
  if(WSAStartup(socketVersion, &wsaData) != 0)
  {
    return 0;
  }
  SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(8888);
  sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  int len = sizeof(sin);

  std::cout<<"client begin send data."<<std::endl;
  while (true)
  {
    //发送数据
    std::string strSrcData;
    while (true) {
      if (strSrcData.compare("----------") == 0) {
        strSrcData.clear();
      }
      sendto(sclient, strSrcData.c_str(), strSrcData.size(), 0, (sockaddr *)&sin, len);
      strSrcData.append("-");
      Sleep(40);
    }
  }  

  //接受数据
  //char recvData[255];     
  //int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
  //if(ret > 0)
  //{
  //  recvData[ret] = 0x00;
  //  printf(recvData);
  //}

  closesocket(sclient);
  WSACleanup();
  return 0;
}