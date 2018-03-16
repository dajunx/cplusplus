#include <Windows.h>
#include <mysql.h>
#include <winsock.h>

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

class DB_managment;
class IO;
class worker;

//=============================数据定义=========================================

enum enumIOType { tIOERR, tINPUT, tGET };
enum enumSqlType { tSqlERR, tInsert, tGetRecordCount, tSimpleSearch };

//=============================IO=============================================

class IO {
public:
  IO() { readFileContent("test.log"); }
  ~IO() {}

  void wrapSave(const std::string &fileName, const std::string &data) {
    saveContentToFile(fileName, data);
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

  void doSql(const std::string &strSql, int iSqlType) {
    if (conn == NULL || strSql.empty()) {
      return;
    }

    if (mysql_real_query(conn, strSql.c_str(), strSql.size())) {
      return;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
      print_errcode(__LINE__);
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
    std::cout << "execute sql done." << std::endl;
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
  strRequest(const std::string &sql, int type) : strSql(sql), iSqlType(type) {}
  ~strRequest() {}
  std::string strSql;
  int iSqlType;
};

static std::list<strRequest> slist_str;
static HANDLE mutex_data_box = CreateMutex(NULL, false, NULL);
void data_box(int ioType, int &SqlType, std::string &str_data) {
  WaitForSingleObject(mutex_data_box, INFINITE);
  switch (ioType) {
  case tINPUT: {
    strRequest req(str_data, SqlType);
    slist_str.insert(slist_str.end(), req);
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

DWORD WINAPI AddRequest(LPVOID lpParameter) {
  strRequest *pRequest = static_cast<strRequest *>(lpParameter);

  /// TODO  模拟请求,暂时写死，有更好的方案再修改;
  std::stringstream ssSql;
  std::string strSql;
  int iSqlType = 0;

  //插入数据
  ssSql << "insert into test.test values";
  for (int i = 0; i < 1000; i++) {
    ssSql << "(" << i << ", " << i << i << "),";
  }
  strSql = ssSql.str();
  strSql.pop_back();
  iSqlType = tInsert;
  data_box(tINPUT, iSqlType, strSql);

  //查询数据条目
  ssSql.str("");
  strSql.clear();
  ssSql << "select count(*) from test.test";
  iSqlType = tGetRecordCount;
  data_box(tINPUT, iSqlType, ssSql.str());

  //普通查询
  ssSql.str("");
  ssSql << "select * from test.test";
  iSqlType = tSimpleSearch;
  data_box(tINPUT, iSqlType, ssSql.str());

  std::cout << "add request finished." << std::endl;
  return 0L;
}

DWORD WINAPI HandleReq(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  std::string strSql;
  while (true) {
    int iSqlType = tSqlERR;
    data_box(tGET, iSqlType, strSql);
    DB_managment *pdb = pMultiThread->pConnDb_3306;

    /// TODo 插入数据使用3306实例，查询类使用3307
    if (iSqlType != tSqlERR) {
      pdb->doSql(strSql, iSqlType);
    }
    Sleep(1000);
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
  ~worker() {}

public:
  void chooseDatabase() {
    if (!bInitRes_) {
      return;
    }

    multiThread_.chooseDatabase();
  }

  void execute_sql() {
    if (!bInitRes_) {
      return;
    }

    std::cout << "start two thread, one for addData, one for getData."
              << std::endl;
    HANDLE thread1 = CreateThread(NULL, 0, HandleReq, &multiThread_, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, AddRequest, &multiThread_, 0, NULL);
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    std::cout << "begin close threads." << std::endl;
    CloseHandle(thread1);
    CloseHandle(thread2);
  }

private:
  bool bInitRes_;
  strMultiThread multiThread_;
};