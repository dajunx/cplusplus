#include <Windows.h>
#include <mysql.h>
#include <winsock.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class DB_managment;
class IO;
class worker;

//=============================IO===============================================

class IO {
public:
  IO() {
    readFileContent("test.log");
  }
  ~IO() {}

  void wrapSave(MYSQL_ROW *pRes, int column) {
    std::stringstream data;
    for (int i = 0; i < column; ++i) {
      data << (*pRes)[i] << " ";
    }
    data << std::endl;
    saveContentToFile("test.log", data.str());
    data.str("");
  }

  void readFileContent(const std::string fileName) {
    return;
    ///TODO 没想好这点要做什么
    std::ifstream file(fileName);
    std::string s;
    while (file >> s) {
      std::cout << "ch : " << s << std::endl;
    }
  }

  void saveContentToFile(const std::string fileName,
                         const std::string inputData) {
    std::fstream file(fileName,
                      std::fstream::in | std::fstream::out | std::fstream::app);
    file << inputData;
    file.close();
  }
};

//=============================DB管理===========================================

class DB_managment {
public:
  DB_managment() : conn(NULL), res(NULL), p_io(new IO()) {
    databaseName.append("test");
  }
  ~DB_managment() {}

  int init(std::string userName, std::string pwd, std::string loginIp,
           int loginPort) {
    //初始化
    conn = mysql_init(0);

    if (conn == NULL) {
      return -1;
    }

    //连接数据库
    if (!mysql_real_connect(conn, loginIp.c_str(), userName.c_str(),
                            pwd.c_str(), databaseName.c_str(), loginPort, NULL,
                            0)) {
      return -1;
    }
    conn->reconnect = 1;
    return 0;
  }

  int choose_database() {
    if (mysql_select_db(conn, databaseName.c_str())) {
      return -1;
    }
    return 0;
  }

  int run_search(const std::string &strSql) {
    if (conn == NULL) {
      return -1;
    }

    //执行查询
    if (mysql_real_query(conn, strSql.c_str(),
                         strSql.size())) { /// TODO偶尔崩溃
      // 查询出错
      return -1;
    }

    //获取返回结果
    res = mysql_store_result(conn); /// TODO 偶尔崩溃
    std::cout << "number of rows is :" << mysql_num_rows(res) << std::endl;
    while (row = mysql_fetch_row(res)) {
      //       std::cout << "一行记录: ";
      //       for (unsigned int j = 0; j < mysql_num_fields(res); ++j) {
      //         std::cout << row[j] << " ";
      //       }
      //       std::cout << "down." << std::endl;
      p_io->wrapSave(&row, mysql_num_fields(res)); //保存到文本文件中
      Sleep(20);
    }

    mysql_free_result(res);
    std::cout << "search table down." << std::endl << std::endl;
    return 0;
  }

  int run_insert(const std::string &strSql) {
    if (conn == NULL || strSql.empty()) {
      return -1;
    }

    //执行查询
    if (mysql_real_query(conn, strSql.c_str(), strSql.size())) {
      // 查询出错
      return -1;
    }
    return 0;
  }

  void print_errcode() {
    //?如何让错误码显示对于中文含义
    std::cout << "handle execute err,err_code:" << mysql_errno(conn)
              << std::endl;
  }

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
  std::string databaseName;
  IO *p_io;
};

//=============================线程函数==========================================

DWORD WINAPI addSomeDatas(LPVOID lpParameter) {
  DB_managment *p_db_3306 = static_cast<DB_managment *>(lpParameter);

  std::stringstream strSql;
  for (int i = 0; i < 1000; i++) {
    strSql << "insert into test values(" << i << ", " << i << i << i << ")";
    if (p_db_3306->run_insert(strSql.str())) {
      p_db_3306->print_errcode();
    }
    strSql.str("");
  }
  return 0L;
}

DWORD WINAPI getDatas(LPVOID lpParameter) {
  DB_managment *p_db_3306 = static_cast<DB_managment *>(lpParameter);

  std::stringstream strSql;
  while (true) {
    strSql << "select * from test";
    if (p_db_3306->run_search(strSql.str())) {
      p_db_3306->print_errcode();
    }
    strSql.str("");

    Sleep(1000);
  }
  return 0L;
}

//=============================工作==========================================

class worker {
public:
  worker() : bInitRes(false) {
    if (db_3306.init("root", "", "127.0.0.1", 3306)) {
      db_3306.print_errcode();
    }
    bInitRes = true;
  }
  ~worker() {}

  void chooseDatabase() {
    if (bInitRes && db_3306.choose_database()) {
      db_3306.print_errcode();
    }
  }

  void run_thread() {
    if (!bInitRes) {
      return;
    }

    HANDLE thread1 = CreateThread(NULL, 0, addSomeDatas, &db_3306, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, getDatas, &db_3306, 0, NULL);
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    CloseHandle(thread1);
    CloseHandle(thread2);
  }

private:
  DB_managment db_3306;
  bool bInitRes;
};