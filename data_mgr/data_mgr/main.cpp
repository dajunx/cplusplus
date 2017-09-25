
#include "network_mgr.h" //网络
//#include "message_dispatch.h" // 消息分发
#include "data_mgr.h" //数据层
#include "common_fun.h" //公共函数库
#include "service.h"

void init_main(boost::asio::io_service& io_service,
               boost::uint32_t& port,
               boost::shared_ptr<data_mgr>& ptr_data_mgr)
{
  ptr_data_mgr = boost::make_shared<data_mgr>();

  static boost::asio::io_service::work work_(io_service);
  static boost::thread th(boost::bind(&boost::asio::io_service::run,
                               boost::ref(io_service)));
  th.detach();

  port = 9999;
  LOG(INFO) << "data_mgr init finished!" << std::endl;
}

int main(int argc, char* argv[])
{
  //set log info.
  FLAGS_log_dir = "./";
  google::InitGoogleLogging(argv[0]);

  boost::shared_ptr<data_mgr> ptr_data_mgr;
  boost::asio::io_service io_service;
  boost::uint32_t port = 0;
  init_main(io_service, port, ptr_data_mgr);

  service ser;
  ser.init_net(io_service, port, ptr_data_mgr);
  ser.accept_connecter_loop();

  return 0;
}