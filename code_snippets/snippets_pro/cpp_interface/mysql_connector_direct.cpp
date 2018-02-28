
//32位版本 直接连接 mysql，不使用 第三方库测试
#include <Windows.h>
#include <mysql.h>
#include <string>
#include <winsock.h>

int main() {
  MYSQL *conn = NULL;
  MYSQL_RES *res = NULL;
  MYSQL_ROW row;

  // database configuartion
  std::string userName("root"), pwd(""), loginIp("127.0.0.1");
  std::string databaseName("test"), tablename("test");
  char *query = NULL;

  //初始化
  conn = mysql_init(0);

  do {
    if (conn == NULL) {
      break;
    }

    //连接数据库
    if (!mysql_real_connect(conn, loginIp.c_str(), userName.c_str(), pwd.c_str(),
      databaseName.c_str(), 3306, NULL, 0)) {
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
  } while (false);

  return 0;
}