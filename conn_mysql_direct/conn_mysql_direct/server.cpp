#include <iostream>
#include <string>

#include "server_net.h" //server_net.h ���岻�ܷ� windows.hͷ�ļ�
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
  //  //��¼һ��TCP�յ���sql����
  //  ///TODO ���⣺server�˽��ܵ���
  //  sql����п��ܲ��������ģ��ر����ڵ�һ����¼��ʱ��
  //  ///��һ����¼����Ӷ���sql��䣡����
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