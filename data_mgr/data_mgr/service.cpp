#include "service.h"

void service::init_net(boost::asio::io_service& io_service, short port, boost::shared_ptr<data_mgr>& ptr_data_mgr)
{
  net_mgr = boost::make_shared<network_mgr>(io_service, port, ptr_data_mgr);
}

void service::accept_connecter_loop()
{
  LOG(INFO) << "begin accept network connect." << std::endl;
  do
  {
    net_mgr->receive_client_conn();
  } while(true);
}