#ifndef NET_H
#define NET_H

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

enum enumNetType { tNetServer, tNetClient };

class net_manage {
public:
  net_manage(enumNetType netType) {
    init_net_lib();

    switch (netType) {
    case tNetClient:
      init_net_client("127.0.0.1", 9998);
      break;
    case tNetServer:
      init_net_server(9999);
      break;
    default:
      break;
    }
    std::cout << "init net err, net type is:" << netType << std::endl;
  }
  ~net_manage() {
    closesocket(socket_server_);
    closesocket(socket_client_);
  }

private:
  void init_net_lib() {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0) {
      return;
    }
  }

  //===============================服务端======================================
  void init_net_server(int port) {
    socket_server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_server_ == INVALID_SOCKET) {
      printf("socket error !");
      return;
    }

    sin_server_.sin_family = AF_INET;
    sin_server_.sin_port = htons(port);
    sin_server_.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(socket_server_, (LPSOCKADDR)&sin_server_, sizeof(sin_server_)) ==
        SOCKET_ERROR) {
      printf("bind error !");
    }

    if (listen(socket_server_, 5) == SOCKET_ERROR) {
      printf("listen error !");
      return;
    }
  }

  void accept_new_conn() {
    printf("等待连接...\n");
    accept_socket_ =
        accept(socket_server_, (SOCKADDR *)&remoteAddr_, &remotAddrlen_);
    if (accept_socket_ == INVALID_SOCKET) {
      printf("accept error !");
      // accept 失败，如何做？
      // continue;
    }
    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr_.sin_addr));
  }

  //===============================客户端======================================
  void init_net_client(const std::string &ip, int port) {
    socket_client_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_client_ == INVALID_SOCKET) {
      printf("invalid socket !");
      return;
    }

    sin_client_.sin_family = AF_INET;
    sin_client_.sin_port = htons(port);
    sin_client_.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
  }

  int conn_server() {
    if (connect(socket_client_, (sockaddr *)&sin_client_,
                sizeof(sin_client_)) != SOCKET_ERROR) {
      return 1;
    }
    return 0;
  }

public:
  //===============================收发数据=====================================
  ///TODO 有了数据，但是得把数据结构化 --add by ljj 2018-03-19
  void receive_data(SOCKET target_socket) {
    int ret = recv(target_socket, receive_data_, 255, 0);
    if (ret > 0) {
      receive_data_[ret] = 0x00;
      printf(receive_data_);
    }
  }

  void send_data(SOCKET target_socket, const std::string &strSendData) {
    send(target_socket, strSendData.c_str(), strSendData.size(), 0);
  }

private:
  // server data;
  SOCKET socket_server_;
  sockaddr_in sin_server_;
  SOCKET accept_socket_;
  sockaddr_in remoteAddr_;
  int remotAddrlen_;

  // client data;
  SOCKET socket_client_;
  sockaddr_in sin_client_;

  // data.
  char receive_data_[255];
};

#endif