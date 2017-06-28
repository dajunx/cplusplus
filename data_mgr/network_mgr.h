#ifndef NETWORK_MGR
#define NETWORK_MGR
//#include "common_def.h"
#include "message_dispatch.h"

#define max_length 1024
using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socket_ptr;

class network_mgr
{
public:
  network_mgr(boost::asio::io_service& io_service, short port, boost::shared_ptr<data_mgr>& ptr_data_mgr)
    : io_service_(io_service)
    , acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    //, running_(true)
    , ptr_dis_msg_(boost::make_shared<message_mgr>(ptr_data_mgr))
  {
    ptr_dis_msg_->init();
  }

  void receive_client_conn();

  void handle_read_data(const boost::system::error_code& error,
                     socket_ptr ptr_socket);

  void handle_write_data(const boost::system::error_code& error,
                   size_t bytes_transferred, socket_ptr ptr_socket);

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  char data_[max_length];
  std::vector<socket_ptr > vec_socket_ptr_;
  //bool running_;

  boost::shared_ptr<message_mgr> ptr_dis_msg_;
};

#endif