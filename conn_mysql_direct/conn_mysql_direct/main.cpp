#include <iostream>
#include <string>

#include "net.h" //net.h 定义不能放 windows.h头文件
#include "db.h"

int main() {
  net_manage net(tNetServer);

  worker w;
  w.chooseDatabase();
  w.execute_sql();

  return 0;
}