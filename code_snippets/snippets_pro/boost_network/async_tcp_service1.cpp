/*
1、接受client连接采用阻塞方式；
2、数据传输放在io_sevice上面，非阻塞方式；
*/
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

#define max_length 1024
using boost::asio::ip::tcp;

class server {
public:
  server(boost::asio::io_service &io_service, short port)
      : io_service_(io_service),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  //, socket_(io_service)
  {}

  void receive_client_conn() {
    boost::shared_ptr<tcp::socket> ptr_socket =
        boost::make_shared<tcp::socket>(io_service_);
    vec_socket_ptr_.push_back(ptr_socket);
    boost::system::error_code ecc;
    ecc = acceptor_.accept(*ptr_socket, ecc);
    io_service_.post(
        boost::bind(&server::handle_accept, this, ecc, ptr_socket));
  }

  void handle_accept(const boost::system::error_code &error,
                     boost::shared_ptr<tcp::socket> ptr_socket) {
    if (!error) {
      ptr_socket->async_read_some(
          boost::asio::buffer(data_, max_length),
          boost::bind(
              &server::handle_read, this, boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred, ptr_socket));
    } else {
      std::cout << __FUNCTION__ << ", error value:" << error.message()
                << std::endl;
    }
  }

  void handle_read(const boost::system::error_code &error,
                   size_t bytes_transferred,
                   boost::shared_ptr<tcp::socket> ptr_socket) {
    if (!error) {
      data_[bytes_transferred] = '\0';
      std::cout << __FUNCTION__ << ", data:" << data_ << std::endl;
      boost::asio::async_write(
          *ptr_socket, boost::asio::buffer(data_, bytes_transferred),
          boost::bind(&server::handle_write, this,
                      boost::asio::placeholders::error, ptr_socket));
    } else {
      std::cout << __FUNCTION__ << ", error value:" << error.message()
                << std::endl;
    }
  }

  void handle_write(const boost::system::error_code &error,
                    boost::shared_ptr<tcp::socket> ptr_socket) {
    if (!error) {
      ptr_socket->async_read_some(
          boost::asio::buffer(data_, max_length),
          boost::bind(
              &server::handle_read, this, boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred, ptr_socket));
    } else {
      std::cout << __FUNCTION__ << ", error value:" << error.message()
                << std::endl;
    }
  }

private:
  boost::asio::io_service &io_service_;
  tcp::acceptor acceptor_;
  // boost::asio::ip::tcp::socket socket_;
  char data_[max_length];
  std::vector<boost::shared_ptr<tcp::socket>> vec_socket_ptr_;
};

int main() {
  boost::asio::io_service io_service;
  boost::asio::io_service::work work(io_service);
  server s(io_service, 9999);
  boost::thread th(
      boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
  th.detach();

  //接受连接循环,阻塞
  do {
    s.receive_client_conn();
  } while (1);

  return 0;
}