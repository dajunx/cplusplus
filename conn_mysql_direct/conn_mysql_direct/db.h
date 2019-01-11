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

//=============================���ݶ���=========================================

enum enumSqlMgrType { tIOERR, tINPUT, tGET };
enum enumSqlExcuteType { tSqlERR, tInsert, tGetRecordCount, tSimpleSearch };

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
    /// TODO û������Ҫ��ʲô
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

//=============================DB����===========================================

class DB_managment {
public:
  DB_managment() : conn(NULL), res(NULL), p_io(new IO()) {
    dbName.append("test");
  }
  ~DB_managment() {}

  int init(std::string userName, std::string pwd, std::string ip,
           int loginPort) {
    //��ʼ��
    try {
      //����apiʹ�ó������⣬����ֱ�ӳ����˳�??? Release û������
      //ԭ�� ����ʱʹ�� libmysqld.dll �������⣬ʹ�� libmysql.dll��û���⣿����
      conn = mysql_init(NULL);
    } catch (int e) {
      int i = 0;
    }
    // conn = mysql_init(0);

    if (conn == NULL) {
      return -1;
    }

    //�������ݿ�
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

    // �������
    switch (iSqlType) {
    case tInsert: {
      // ������������ sql�޷���ֵ
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

//=============================��Ϣ����==========================================

struct strRequest {
  strRequest(const std::string &sql, enumSqlExcuteType type)
      : strSql(sql), iSqlType(type) {}
  ~strRequest() {}
  std::string strSql;
  enumSqlExcuteType iSqlType;
};

static std::list<strRequest> slist_str;
static HANDLE mutex_data_box = CreateMutex(NULL, false, NULL);

// sqlִ�����Ĺ���1. ����sql������䣻 2.ȡ��sql�������ȥִ�У�
void sqlReqMgr(enumSqlMgrType sqlMgrType, enumSqlExcuteType &sqlExcuteType,
               std::string &str_data) {
  WaitForSingleObject(mutex_data_box, INFINITE);
  switch (sqlMgrType) {
  case tINPUT: {
    //��ֹ���һ���
    // insert��䣬ԭ��mysql_real_queryֻ��һ��һ����ִ�в������;
    if (tInsert == sqlExcuteType) {
      while (!str_data.empty() &&
             str_data.find_first_of(';') != std::string::npos) {
        std::string::size_type pos_begin = 0, pos_end = 0;
        pos_end = str_data.find_first_of(';');
        strRequest req(str_data.substr(pos_begin, pos_end - pos_begin + 1),
                       sqlExcuteType);
        slist_str.insert(slist_str.end(), req);
        str_data.erase(pos_begin, pos_end - pos_begin + 1);
      }
    } else {
      strRequest req(str_data, sqlExcuteType);
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
    sqlExcuteType = slist_str.front().iSqlType;
    slist_str.pop_front();
  } break;
  default: { std::cout << "" << std::endl; } break;
  }
  ReleaseMutex(mutex_data_box);
}

//=============================�̺߳���==========================================

struct strMultiThread {
  strMultiThread()
      : pConnDb_3306(new DB_managment), pConnDb_3307(new DB_managment),
        mutex(NULL) {}

  int init() {
    int ret = 1;
    do {
      /// TODO ����ʹ�ñ���mysql��Ϊ�����������ӣ�����ʱע�͵�ʹ�� 3307 �˿��߼�
      if (init_db(pConnDb_3306, 3306) /* || init_db(pConnDb_3307, 3307)*/) {
        break;
      }
      mutex = CreateMutex(NULL, false, NULL);
      ret = 0;
    } while (false);

    return ret;
  }

  int init_db(DB_managment *pdb, int port) {
    if (pdb->init("root", "", "127.0.0.1", port)) {
      pdb->print_errcode(__LINE__);
      return 1;
    }
    return 0;
  }

  void chooseDatabase() {
    /// TODO ����ʹ�ñ���mysql��Ϊ�����������ӣ�����ʱע�͵�ʹ�� 3307 �˿��߼�
    if (pConnDb_3306
            ->choose_database() /* && pConnDb_3307->choose_database()*/) {
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

// ���� sql�������� �����̣߳���� ȥȡҪִ�е�sql���
DWORD WINAPI workerThread(LPVOID lpParameter) {
  strMultiThread *pMultiThread = static_cast<strMultiThread *>(lpParameter);

  std::string strSql;
  while (true) {
    enumSqlExcuteType iSqlType = tSqlERR;
    sqlReqMgr(tGET, iSqlType, strSql);
    DB_managment *pdb = pMultiThread->pConnDb_3306;

    /// TODo ��������ʹ��3306ʵ������ѯ��ʹ��3307
    if (iSqlType != tSqlERR) {
      pdb->doSql(strSql, iSqlType);
    }
    Sleep(10);
    strSql.clear();
  }

  std::cout << "handle request finished." << std::endl;
  return 0L;
}

//=============================����==========================================

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
    thread_handler =
        CreateThread(NULL, 0, workerThread, &multiThread_, 0, NULL);
  }

private:
  HANDLE thread_handler;
  bool bInitRes_;
  strMultiThread multiThread_;
};

#endif