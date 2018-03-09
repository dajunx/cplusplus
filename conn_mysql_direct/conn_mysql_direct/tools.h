#include <Windows.h>
#include <mysql.h>
#include <winsock.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class DB_managment;
class IO;
class worker;

//=============================IO===============================================

class IO {
public:
  IO() { readFileContent("test.log"); }
  ~IO() {}

  void wrapSave(const std::string &data) {
    saveContentToFile("test.log", data);
  }

  void readFileContent(const std::string fileName) {
    return;
    /// TODO 没想好这点要做什么
    std::ifstream file(fileName);
    std::string s;
    while (file >> s) {
      std::cout << "ch : " << s << std::endl;
    }
  }

  void saveContentToFile(const std::string &fileName,
                         const std::string &inputData) {
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
    dbName.append("test");
  }
  ~DB_managment() {}

  int init(std::string userName, std::string pwd, std::string ip,
           int loginPort) {
    //初始化
    conn = mysql_init(0);

    if (conn == NULL) {
      return -1;
    }

    //连接数据库
    if (!mysql_real_connect(conn, ip.c_str(), userName.c_str(), pwd.c_str(),
                            dbName.c_str(), loginPort, NULL, 0)) {
      return -1;
    }
    conn->reconnect = 1;
    return 0;
  }

  int choose_database() {
    if (mysql_select_db(conn, dbName.c_str())) {
      return -1;
    }
    return 0;
  }

  // mysql api位置处偶尔崩溃，极可能是 db操作在多线程环境下造成。
  int run_search(const std::string &strSql) {
    if (conn == NULL) {
      return -1;
    }

    //执行查询
    if (mysql_real_query(conn, strSql.c_str(), strSql.size())) {
      // 查询出错
      return -1;
    }

    //获取返回结果
    res = mysql_store_result(conn);
    // mysql_num_rows 字段数(列)
    std::stringstream data;
    int column = mysql_num_fields(res);
    while (row = mysql_fetch_row(res)) {
      for (int i = 0; i < column; ++i) {
        // std::cout << (*row)[i] << " "; 读取第二个元素出错，移动步长错误
        data << row[i] << " ";
      }
      data << std::endl;
    }
    p_io->wrapSave(data.str());

    mysql_free_result(res);
    return 0;
  }

  int run_getTableRecordCount(const std::string &strSql) {
    if (conn == NULL) {
      return -1;
    }

    if (mysql_real_query(conn, strSql.c_str(), strSql.size())) {
      return -1;
    }

    res = mysql_store_result(conn);
    int column = mysql_num_fields(res), recordCount = 0;
    while (row = mysql_fetch_row(res)) {
      recordCount = atoi(row[0]);
    }

    mysql_free_result(res);
    return recordCount;
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

  void print_errcode(long iLine = -1) {
    //?如何让错误码显示对于中文含义
    std::cout << "err ,err_code:" << mysql_errno(conn)
              << ", line number: " << iLine << std::endl;
  }

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
  std::string dbName;
  IO *p_io;
};

//=============================线程函数==========================================

struct strMultiThread {
  strMultiThread() : pConnDb_3306(NULL), pConnDb_3307(NULL), mutex(NULL) {}
  ~strMultiThread() {
    if (pConnDb_3306) {
      delete pConnDb_3306;
      pConnDb_3306 = NULL;
    }

    if (pConnDb_3307) {
      delete pConnDb_3307;
      pConnDb_3307 = NULL;
    }
  }
  DB_managment *pConnDb_3306;
  DB_managment *pConnDb_3307;
  HANDLE mutex;

  /// TODO 设想的是只启动一个执行线程用来执行如下给予的
  /// sql语句。在建一个线程来接应请求。
  std::string strSql;
};

DWORD WINAPI addSomeDatas(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  return 0L;
  std::stringstream strSql;
  for (int i = 0; i < 1000; i++) {
    strSql << "insert into test values(" << i << ", " << i << i << i << ")";
    WaitForSingleObject(pMultiThread->mutex, INFINITE);
    if (pMultiThread->pConnDb_3306->run_insert(strSql.str())) {
      pMultiThread->pConnDb_3306->print_errcode(__LINE__);
    }
    strSql.str("");
    ReleaseMutex(pMultiThread->mutex);
    Sleep(1);
  }

  std::cout << "add some datas into mysql finished." << std::endl;
  return 0L;
}

DWORD WINAPI getDatas(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  std::stringstream strSql;
  while (true) {
    strSql << "select * from test";
    WaitForSingleObject(pMultiThread->mutex, INFINITE);
    if (pMultiThread->pConnDb_3307->run_search(strSql.str())) {
      pMultiThread->pConnDb_3307->print_errcode(__LINE__);
    }
    strSql.str("");
    ReleaseMutex(pMultiThread->mutex);
    Sleep(1000);
  }

  std::cout << "search mysql finished." << std::endl;
  return 0L;
}

DWORD WINAPI getDataCount(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  return 0L;
  std::stringstream strSql;
  {
    strSql << "select count(*) record_count from test";
    WaitForSingleObject(pMultiThread->mutex, INFINITE);
    int ret = pMultiThread->pConnDb_3306->run_getTableRecordCount(strSql.str());
    if (ret) {
      pMultiThread->pConnDb_3306->print_errcode(__LINE__);
    }
    strSql.str("");
    ReleaseMutex(pMultiThread->mutex);
  }

  std::cout << "get mysql table count finished." << std::endl;
  return 0L;
}

//=============================工作==========================================

class worker {
public:
  worker() : bInitRes_(false) {
    multiThread_.pConnDb_3306 = new DB_managment;
    multiThread_.pConnDb_3307 = new DB_managment;
    multiThread_.mutex = CreateMutex(NULL, false, NULL);

    int res_init_two_db_point = 0;
    if (multiThread_.pConnDb_3306) {
      res_init_two_db_point += init_db(multiThread_.pConnDb_3306, 3306);
    }

    if (multiThread_.pConnDb_3307) {
      res_init_two_db_point += init_db(multiThread_.pConnDb_3307, 3307);
    }

    if (2 == res_init_two_db_point) {
      bInitRes_ = true;
    } else {
      std::cout << "init mysql connect failed." << std::endl;
    }
  }
  ~worker() {}

private:
  int init_db(DB_managment *pdb, int port) {
    if (pdb->init("kaka", "", "192.168.221.138", port)) {
      pdb->print_errcode(__LINE__);
      return 0;
    }
    return 1;
  }

public:
  void chooseDatabase() {
    if (!bInitRes_) {
      return;
    }

    if (multiThread_.pConnDb_3306->choose_database() &&
        multiThread_.pConnDb_3307->choose_database()) {
      multiThread_.pConnDb_3306->print_errcode(__LINE__);
    }
  }

  void execute_sql() {
    if (!bInitRes_) {
      return;
    }

    std::cout << "start two thread, one for addData, one for getData."
              << std::endl;
    HANDLE thread1 =
        CreateThread(NULL, 0, addSomeDatas, &multiThread_, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, getDatas, &multiThread_, 0, NULL);
    HANDLE thread3 =
        CreateThread(NULL, 0, getDataCount, &multiThread_, 0, NULL);
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);
    WaitForSingleObject(thread3, INFINITE);

    std::cout << "begin close threads." << std::endl;
    CloseHandle(thread1);
    CloseHandle(thread2);
    CloseHandle(thread3);
  }

private:
  bool bInitRes_;
  strMultiThread multiThread_;
};