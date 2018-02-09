//同步方式 tcp client
#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;
#define max_length 1024

int main(int argc, char *argv[]) {
  try {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);

    std::stringstream str_port;
    if (argc == 2) {
      str_port << argv[1];
    } else {
      str_port << 9999;
    }
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", str_port.str());
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);

    bool b_send_data = true;
    std::string str_end("quit");
    do {
      using namespace std; // For strlen.
      std::cout << "Enter message: ";
      char request[max_length];
      std::cin.getline(request, max_length);
      size_t request_length = strlen(request);
      boost::asio::write(s, boost::asio::buffer(request, request_length));

      char reply[max_length];
      size_t reply_length =
          boost::asio::read(s, boost::asio::buffer(reply, request_length));
      std::cout << "Reply is: ";
      std::cout.write(reply, reply_length);
      std::cout << "\n";

      b_send_data = (str_end.compare(request) != 0);
    } while (b_send_data);

  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}