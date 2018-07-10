#ifndef DB_H
#define DB_H

#include <Windows.h>
#include <mysql.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

class DB_managment;
class IO;
class worker;

#define LogErrorSql(lineNumber)                                                \
  std::stringstream ssErrContent;                                              \
  ssErrContent << strSql << ", line:" << (lineNumber) << "\n";                 \
  p_io->wrapSave("err_sql.log", ssErrContent.str());

//=============================数据定义=========================================

enum enumIOType { tIOERR, tINPUT, tGET };
enum enumSqlType { tSqlERR, tInsert, tGetRecordCount, tSimpleSearch };

//=============================IO=============================================

class IO {
public:
  IO() { readFileContent("test.log"); }
  ~IO() {}

  void wrapSave(const std::string &fileName, const std::string &data) {
    saveContentToFile("F:\\github_git\\conn_mysql_direct\\" + fileName, data);
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
    try {
      //如下api使用出现问题，调用直接程序退出??? Release 没有问题
      //原因 运行时使用 libmysqld.dll 会有问题，使用 libmysql.dll则没问题？？？
      conn = mysql_init(NULL);
    } catch (int e) {
      int i = 0;
    }
    // conn = mysql_init(0);

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

  void doSql(const std::string &strSql, int iSqlType) {
    p_io->wrapSave("doSql.log", strSql + "\n");
    if (conn == NULL || strSql.empty()) {
      LogErrorSql(__LINE__);
      return;
    }

    if (mysql_real_query(conn, strSql.c_str(), strSql.size())) {
      LogErrorSql(__LINE__);
      return;
    }

    res = mysql_store_result(conn);
    if (res == NULL && iSqlType != iSqlType) {
      print_errcode(__LINE__);
      LogErrorSql(__LINE__);
      return;
    }

    // 结果处理
    switch (iSqlType) {
    case tInsert: {
      // 插入数据类型 sql无返回值
    } break;
    case tGetRecordCount: {
      int recordCount = 0;
      while (row = mysql_fetch_row(res)) {
        recordCount = atoi(row[0]);
      }
      std::cout << "table record count: " << recordCount << std::endl;
    } break;
    case tSimpleSearch: {
      std::stringstream data;
      int column = mysql_num_fields(res);
      while (row = mysql_fetch_row(res)) {
        for (int i = 0; i < column; ++i) {
          data << row[i] << " ";
        }
        data << std::endl;
      }
      p_io->wrapSave("test.log", data.str());
    } break;
    default:
      break;
    }

    mysql_free_result(res);
  }

  void logSqlContent(const std::string &sql) {
    p_io->wrapSave("execute_sql.log", sql);
  }

  void print_errcode(long lineNumber = -1) {
    std::cout << "err ,err_code:" << mysql_errno(conn)
              << ", line number: " << lineNumber << std::endl;
  }

  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
  std::string dbName;
  IO *p_io;
};

//=============================消息容器==========================================

struct strRequest {
  strRequest(const std::string &sql, enumSqlType type)
      : strSql(sql), iSqlType(type) {}
  ~strRequest() {}
  std::string strSql;
  enumSqlType iSqlType;
};

static std::list<strRequest> slist_str;
std::vector<std::string> vecInsertSql;
static HANDLE mutex_data_box = CreateMutex(NULL, false, NULL);
void data_box(enumIOType ioType, enumSqlType &SqlType, std::string &str_data) {
  WaitForSingleObject(mutex_data_box, INFINITE);
  switch (ioType) {
  case tINPUT: {
    //拆分裹在一起的
    // insert语句，原因：mysql_real_query只能一条一条的执行插入语句;
    if (tInsert == SqlType) {
      while (!str_data.empty() &&
             str_data.find_first_of(';') != std::string::npos) {
        std::string::size_type pos_begin = 0, pos_end = 0;
        pos_end = str_data.find_first_of(';');
        strRequest req(str_data.substr(pos_begin, pos_end - pos_begin + 1),
                       SqlType);
        slist_str.insert(slist_str.end(), req);
        str_data.erase(pos_begin, pos_end - pos_begin + 1);
      }
    } else {
      strRequest req(str_data, SqlType);
      slist_str.insert(slist_str.end(), req);
    }

    // strRequest req(str_data, SqlType);
    // slist_str.insert(slist_str.end(), req);
  } break;
  case tGET: {
    if (slist_str.empty()) {
      ReleaseMutex(mutex_data_box);
      return;
    }

    str_data.swap(slist_str.front().strSql);
    SqlType = slist_str.front().iSqlType;
    slist_str.pop_front();
  } break;
  default: { std::cout << "" << std::endl; } break;
  }
  ReleaseMutex(mutex_data_box);
}

//=============================线程函数==========================================

struct strMultiThread {
  strMultiThread()
      : pConnDb_3306(new DB_managment), pConnDb_3307(new DB_managment),
        mutex(NULL) {}

  int init() {
    int ret = 1;
    do {
      if (init_db(pConnDb_3306, 3306) || init_db(pConnDb_3307, 3307)) {
        break;
      }
      mutex = CreateMutex(NULL, false, NULL);
      ret = 0;
    } while (false);

    return ret;
  }

  int init_db(DB_managment *pdb, int port) {
    if (pdb->init("kaka", "kaka", "172.20.24.190", port)) {
      pdb->print_errcode(__LINE__);
      return 1;
    }
    return 0;
  }

  void chooseDatabase() {
    if (pConnDb_3306->choose_database() && pConnDb_3307->choose_database()) {
      pConnDb_3306->print_errcode(__LINE__);
    }
  }

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
};

DWORD WINAPI HandleReq(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  std::string strSql;
  while (true) {
    enumSqlType iSqlType = tSqlERR;
    data_box(tGET, iSqlType, strSql);
    DB_managment *pdb = pMultiThread->pConnDb_3306;

    /// TODo 插入数据使用3306实例，查询类使用3307
    if (iSqlType != tSqlERR) {
      pdb->doSql(strSql, iSqlType);
    }
    Sleep(10);
    strSql.clear();
  }

  std::cout << "handle request finished." << std::endl;
  return 0L;
}

//=============================工作==========================================

class worker {
public:
  worker() : bInitRes_(false) {
    if (!multiThread_.init()) {
      bInitRes_ = true;
    }
  }
  ~worker() {
    std::cout << "begin close threads." << std::endl;
    WaitForSingleObject(thread_handler, INFINITE);
    CloseHandle(thread_handler);
  }

public:
  void chooseDatabase() {
    if (!bInitRes_) {
      return;
    }

    multiThread_.chooseDatabase();
  }

  void start_handle_request() {
    if (!bInitRes_) {
      return;
    }
    thread_handler = CreateThread(NULL, 0, HandleReq, &multiThread_, 0, NULL);
  }

private:
  HANDLE thread_handler;
  bool bInitRes_;
  strMultiThread multiThread_;
};

#endif