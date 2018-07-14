#include <iostream>
#include <sstream>
#include <string>

#include "client_net.h"

int main() {
  // client 慢启动
  Sleep(200);

  net_manage net(tNetClient);
  int ret = net.conn_server();
  if (ret) {
    std::cout << "connecting server failed, error code:" << ret << std::endl;
    return 0;
  }

  ///TODO 业务逻辑，待拆分简化
  std::stringstream ssql;
  std::string strResponse;
  for (int i = 0; i < 1000; i++) {
    ssql << "insert into test.test values(" << i << ", " << i << i << ");";
    int ret = net.send_data(ssql.str());

    // 接受回包，即和server端交互，why？(数据方面：不交互发送的数据容易截断；交互：数据传输慢)
    net.receive_data(strResponse);
    if (strResponse.compare("OK") != 0) {
      std::cout << "sql: " << ssql.str() << ", is not execute right."
        << "content: " << strResponse << std::endl;
    } else {
      IO io;
      ssql << " result:" << strResponse << "\n";
      std::string ss_log = ssql.str();
      io.wrapSave("client_req_rsp.log", ss_log);
      strResponse.clear();
    }      

    ssql.str("");
    //Sleep(10);
  }
  std::cout << "send datas finished." << std::endl;

  do {
    Sleep(1000);
  } while (true);

  return 0;
}