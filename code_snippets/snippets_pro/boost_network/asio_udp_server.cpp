//使用boost库asio实现的udp服务器
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#define max_length 1024
using boost::asio::ip::udp;

class service {
public:
  service(boost::asio::io_service &io_service_tmp, udp::endpoint endpoint_tmp)
      : io_service_(io_service_tmp), socket_(io_service_tmp, endpoint_tmp) {
    socket_.async_receive_from(
        boost::asio::buffer(buff_, max_length), endpoint_,
        boost::bind(&service::send_data_back, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void send_data_back(const boost::system::error_code &error,
                      size_t bytes_transferred) {
    if (error) {
      return;
    }
    buff_[bytes_transferred] = '\0';
    std::cout << "buff: " << buff_ << std::endl;
    if (!error && bytes_transferred > 0) {
      socket_.async_send_to(
          boost::asio::buffer(buff_, bytes_transferred), endpoint_,
          boost::bind(&service::receive_data, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }

  void receive_data(const boost::system::error_code &error,
                    size_t bytes_transferred) {
    if (!error) {
      socket_.async_receive_from(
          boost::asio::buffer(buff_, max_length), endpoint_,
          boost::bind(&service::send_data_back, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }

  boost::asio::io_service &io_service_;
  udp::socket socket_;
  udp::endpoint endpoint_;
  char buff_[max_length];
};

int main() {
  boost::asio::io_service io_service;
  boost::asio::ip::udp::endpoint endpoint(udp::v4(), 9999);

  service s(io_service, endpoint);
  io_service.run();

  return 0;
}