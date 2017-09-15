#include "network_mgr.h"
#include "common_fun.h"


void network_mgr::receive_client_conn()
{
  boost::shared_ptr<tcp::socket> ptr_socket = boost::make_shared<tcp::socket>
                                              (io_service_);
  vec_socket_ptr_.push_back(ptr_socket);
  boost::system::error_code ecc;

  //main进程在此处阻塞，等待新的client连接
  /*ecc = */acceptor_.accept(*ptr_socket, ecc);
  if(ecc) {
    //An error occurred.
    return;
  }

  //接受的client连接 post到io_service线程进行处理(数据交互 ...)
  io_service_.post(boost::bind(&network_mgr::handle_read_data, this, ecc, ptr_socket));
}

void network_mgr::handle_read_data(const boost::system::error_code& error,
                           socket_ptr ptr_socket)
{
  if (!error) {
    ptr_socket->async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&network_mgr::handle_write_data, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred,
                                            ptr_socket));
  } else {
    std::cout << __FUNCTION__ << ", error value:" << error.message() << std::endl;
  }
}

void network_mgr::handle_write_data(const boost::system::error_code& error,
                         size_t bytes_transferred,
                         socket_ptr ptr_socket)
{
  if (!error) {
    data_[bytes_transferred] = '\0';
    std::cout << __FUNCTION__ << ", data:" << data_ << std::endl;

    std::vector<std::string> vec_data;
    vec_data.resize(3);
    parse_cmd_and_content(data_, vec_data);

    
    boost::shared_ptr<msg_parameter> ptr_msg_para_ = boost::make_shared<msg_parameter>(vec_data[1], vec_data[2]);
    if (ptr_dis_msg_->check_point_msg(vec_data[0])) {
      ptr_dis_msg_->dispatch_msg(vec_data[0], ptr_msg_para_);
    }

    memset(data_, 0, bytes_transferred);
    if (!ptr_msg_para_->key_.empty()) {
      strncpy(data_, ptr_msg_para_->value_.c_str(), ptr_msg_para_->value_.size());
    }

    boost::asio::async_write(*ptr_socket,
                             boost::asio::buffer(data_, bytes_transferred),
                             boost::bind(&network_mgr::handle_read_data, this,
                                         boost::asio::placeholders::error,
                                         ptr_socket));
  } else {
    std::cout << __FUNCTION__ << ", error value:" << error.message() << std::endl;
  }
}