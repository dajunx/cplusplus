#ifndef NET_H
#define NET_H

#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

enum enumNetType { tNetServer, tNetClient };

class net_manage {
public:
  net_manage(enumNetType nt) : netType_(nt) {
    int ret = -2;
    init_net_lib();

    switch (netType_) {
    case tNetClient:
      ret = init_net_client("127.0.0.1", 9999);
      break;
    case tNetServer:
      ret = init_net_server(9998);
      break;
    default:
      break;
    }

    if (-1 == ret) {
      std::cout << "init net err, net type is:" << netType_ << std::endl;
    }
  }
  ~net_manage() {
    if (tNetClient == netType_) {
      closesocket(socket_client_);
    } else {
      closesocket(socket_server_);
    }
  }

private:
  void init_net_lib() {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0) {
      return;
    }
  }

  int init_net_server(int port) {
    socket_server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_server_ == INVALID_SOCKET) {
      printf("socket error !");
      return -1;
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
      return -1;
    }

    return 0;
  }

  int init_net_client(const std::string &ip, int port) {
    socket_client_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_client_ == INVALID_SOCKET) {
      printf("invalid socket !");
      return -1;
    }

    sin_client_.sin_family = AF_INET;
    sin_client_.sin_port = htons(port);
    sin_client_.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    return 0;
  }

public:
  void accept_new_conn() {
    printf("等待连接...\n");
    remotAddrlen_ = sizeof(remoteAddr_);
    accept_socket_ =
        accept(socket_server_, (SOCKADDR *)&remoteAddr_, &remotAddrlen_);
    if (accept_socket_ == INVALID_SOCKET) {
      printf("accept error !");
      // accept 失败，如何做？
      // continue;
    }
    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr_.sin_addr));
  }

  int conn_server() {
    int ret =
        connect(socket_client_, (sockaddr *)&sin_client_, sizeof(sin_client_));
    if (SOCKET_ERROR == ret) {
      int err_code = WSAGetLastError();
      return err_code;
    }
    return 0;
  }

public:
  //===============================收发数据=====================================
  /// TODO 有了数据，但是得把数据结构化 --add by ljj 2018-03-19
  int receive_data(std::string &strReceiveData) {
    int ret = recv(accept_socket_, receive_data_, 255, 0);
    if (ret > 0) {
      receive_data_[ret] = 0x00;
    } else {
      int err_code = WSAGetLastError();
      return err_code;
    }

    return 0;
  }

  int send_data(const std::string &strSendData) {
    int ret = send(socket_client_, strSendData.c_str(), strSendData.size(), 0);
    if (SOCKET_ERROR == ret) {
      int err_code = WSAGetLastError();
      return err_code;
    } else {
    }

    return 0;
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
  enumNetType netType_;
};

#endif