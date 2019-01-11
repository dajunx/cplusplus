#include <iostream>
#include <string>

#include "server_net.h" //server_net.h 定义不能放 windows.h头文件
#include "db.h"

int main() {
  net_manage net(tNetServer);
  net.receiveRequest();

  worker w;
  w.chooseDatabase();
  w.start_handle_request();

  do {
    Sleep(1000);
  } while (true);

  // std::string strReceiveData, strReceiveDataTemp;
  // do
  //{
  //  net.receive_data(strReceiveData);
  //  strReceiveDataTemp = strReceiveData;
  //
  //  //记录一下TCP收到的sql内容
  //  ///TODO 问题：server端接受到的
  //  sql语句有可能不是完整的，特别是在第一条记录的时候
  //  ///第一条记录会夹杂多条sql语句！！！
  //
  //  enumSqlExcuteType iSqlType = tInsert;
  //  sqlReqMgr(tINPUT, iSqlType, strReceiveData);
  //
  //  IO io;
  //  addTime(strReceiveDataTemp);
  //  io.wrapSave("server_receive_sql.log", strReceiveDataTemp + "\n");
  //  strReceiveData.clear();
  //  net.send_data("OK"); //Response
  //  //Sleep(1000);
  //} while (true);

  return 0;
}