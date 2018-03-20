#include <iostream>
#include <sstream>
#include <string>

#include "client_net.h"

int main() {
  // client 慢启动
  Sleep(100);

  net_manage net(tNetClient);
  int ret = net.conn_server();
  if (ret) {
    std::cout << "connecting server failed, error code:" << ret << std::endl;
  } else {
    std::stringstream ssql;
    std::string strResponse;
    //暂时写成死循环，先验证数据的通达信
    for (int i = 0; i < 1000; i++) {
      ssql << "insert into test.test values(" << i << ", " << i << i << ");";
      int ret = net.send_data(ssql.str());
      //net.receive_data(strResponse);
      //if (strResponse.compare("OK") != 0) {
      //  std::cout << "sql: " << ssql.str() << ", is not execute right."
      //            << "content: " << strResponse << std::endl;
      //}
      //strResponse.clear();

      ssql.str("");
       Sleep(10);
    }
    std::cout << "send datas finished." << std::endl;
  }

  do {
    Sleep(1000);
  } while (true);

  return 0;
}