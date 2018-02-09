//使用boost库asio实现的udp客户端
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
using boost::asio::ip::udp;

class service {
public:
  service(boost::asio::io_service &io_service_tmp, udp::endpoint endpoint_tmp)
      : io_service_(io_service_tmp), socket_(io_service_tmp, endpoint_tmp) {
    udp::resolver resolver(io_service_);
    udp::resolver::query query("127.0.0.1", "9999");
    it = resolver.resolve(query);
    boost::uint32_t buff_size = 0;
    produce_buff(buff_size);

    socket_.async_send_to(
        boost::asio::buffer(buff, buff_size), *it,
        boost::bind(&service::receive_data, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void send_data_back(const boost::system::error_code &error,
                      size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
      buff[bytes_transferred] = '\0';
      std::cout << "buff: " << buff << std::endl;
      boost::uint32_t buff_size = 0;
      produce_buff(buff_size);

      socket_.async_send_to(
          boost::asio::buffer(buff, buff_size), endpoint_,
          boost::bind(&service::receive_data, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }

  void receive_data(const boost::system::error_code &error,
                    size_t bytes_transferred) {
    if (!error) {
      socket_.async_receive_from(
          boost::asio::buffer(buff, max_length), endpoint_,
          boost::bind(&service::send_data_back, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }

  static boost::uint32_t get_request_id() {
    static boost::mutex mutex;
    boost::mutex::scoped_lock l(mutex);
    return ++request_id_;
  }

  void produce_buff(boost::uint32_t &buff_size) {
    std::stringstream input;
    input << get_request_id();
    memcpy(buff, input.str().c_str(), strlen(input.str().c_str()) + 1);
    buff_size = input.str().size();
  }

private:
  boost::asio::io_service &io_service_;
  udp::socket socket_;
  udp::endpoint endpoint_;
  udp::resolver::iterator it;
  enum { max_length = 65535 };
  char buff[max_length];
  static boost::uint32_t request_id_;
};
boost::uint32_t service::request_id_(0);

int main() {
  boost::asio::io_service io_service;
  boost::asio::ip::udp::endpoint endpoint(udp::v4(), 0);

  service s(io_service, endpoint);
  io_service.run();

  return 0;
}