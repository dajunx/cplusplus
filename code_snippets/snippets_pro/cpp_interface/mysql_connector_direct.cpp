
//32位版本 直接连接 mysql，不使用 第三方库测试
#include <Windows.h>
#include <iostream>
#include <mysql.h>
#include <string>
#include <winsock.h>

int main() {
  MYSQL *conn = NULL;
  MYSQL_RES *res = NULL;
  MYSQL_ROW row;

  // database configuartion
  std::string userName("kaka"), pwd(""), loginIp("192.168.221.138");
  std::string databaseName("test"), tablename("test");
  int loginPort = 3306;
  char *query = NULL;
  bool err = true;

  //初始化
  conn = mysql_init(0);

  do {
    if (conn == NULL) {
      break;
    }

    //连接数据库
    if (!mysql_real_connect(conn, loginIp.c_str(), userName.c_str(),
      pwd.c_str(), databaseName.c_str(), loginPort, NULL,
      0)) {
        break;
    }

    //选择数据库
    if (mysql_select_db(conn, databaseName.c_str())) {
      break;
    }

    conn->reconnect = 1;
    query = "SELECT * FROM test";

    //执行查询
    if (mysql_real_query(conn, query, strlen(query))) {
      // 查询出错
      query = "";
      break;
    }

    //获取返回结果
    res = mysql_store_result(conn);
    while (row = mysql_fetch_row(res)) {
      for (unsigned int t = 0; t < mysql_num_fields(res); t++) {
        printf("%s  ", row[t]);
      }
    }

    mysql_free_result(res);
    err = false;
  } while (false);

  if (err) {
    //?如何让错误码显示对于中文含义
    std::cout << "handle query err,err_code:" << mysql_errno(conn) << std::endl;
  }

  return 0;
}