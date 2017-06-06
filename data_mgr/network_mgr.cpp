#include "network_mgr.h"
#include "common_fun.h"


void server::receive_client_conn()
{
  boost::shared_ptr<tcp::socket> ptr_socket = boost::make_shared<tcp::socket>
                                              (io_service_);
  vec_socket_ptr_.push_back(ptr_socket);
  boost::system::error_code ecc;
  ecc = acceptor_.accept(*ptr_socket, ecc);
  io_service_.post(boost::bind(&server::handle_accept, this, ecc, ptr_socket));
}

void server::handle_accept(const boost::system::error_code& error,
                           socket_ptr ptr_socket)
{
  if (!error) {
    ptr_socket->async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&server::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred,
                                            ptr_socket));
  } else {
    std::cout << __FUNCTION__ << ", error value:" << error.message() << std::endl;
  }
}

void server::handle_read(const boost::system::error_code& error,
                         size_t bytes_transferred,
                         socket_ptr ptr_socket)
{
  if (!error) {
    data_[bytes_transferred] = '\0';
    std::cout << __FUNCTION__ << ", data:" << data_ << std::endl;

    std::string msg_tmp, key, value;
    parse_cmd_and_content(data_, msg_tmp, key, value);

    
    boost::shared_ptr<msg_parameter> ptr_msg_para_ = boost::make_shared<msg_parameter>(key, value);
    if (ptr_dis_msg_->check_point_msg(msg_tmp)) {
      ptr_dis_msg_->dispatch_msg(msg_tmp, ptr_msg_para_);
    }

    boost::asio::async_write(*ptr_socket,
                             boost::asio::buffer(data_, bytes_transferred),
                             boost::bind(&server::handle_write, this,
                                         boost::asio::placeholders::error,
                                         ptr_socket));
  } else {
    std::cout << __FUNCTION__ << ", error value:" << error.message() << std::endl;
  }
}

void server::handle_write(const boost::system::error_code& error,
                          socket_ptr ptr_socket)
{
  if (!error) {
    ptr_socket->async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&server::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred,
                                            ptr_socket));
  } else {
    std::cout << __FUNCTION__ << ", error value:" << error.message() << std::endl;
  }
}

void server::accept_client_conn_loop()
{
  ptr_dis_msg_->init();
  do 
  {
    receive_client_conn();
  } while (running_);
}
