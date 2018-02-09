// mysql-connector-c++  连接/查询mysql
//暂时用的3rd库里面的动态库进行编译

//使用lib：  F:\Work\3rd\mysql-connector-c++\build\driver\Debug

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <driver/mysql_connection.h>
//#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

using namespace std;

void read_sql_from_disk_file(std::string &str_sql) {
  std::ifstream fin("file.txt");
  while (getline(fin, str_sql)) {
    std::cout << "Read from file into string: " << str_sql << std::endl;
  }
}

void write_data_to_disk_file(std::string &save_file_name,
                             std::string &save_data) {
  // std::cout<<cost.total_milliseconds()<<"ms"<<std::endl;
  std::fstream file(save_file_name,
                    std::fstream::in | std::fstream::out | std::fstream::app);
  file << save_data;
  file.close();
}

void insert_data_2_mysql(int i) {
  boost::this_thread::sleep(boost::posix_time::seconds(2));
  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    // sql::ResultSet *res;
    // sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("192.168.221.133", "linjj", "19900801");
    //选择要连接的数据库
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");
    stmt = con->createStatement();

    std::string str_sql, str_sql_tmp;
    read_sql_from_disk_file(str_sql);
    for (int index = 0; index < 10000; ++index) {
      str_sql_tmp = str_sql;
      int iBegin = i * 10000 + index;
      std::stringstream uid;
      uid << iBegin;
      str_sql_tmp.append(uid.str());
      str_sql_tmp.append(", \"");
      str_sql_tmp.append(uid.str());
      str_sql_tmp.append("\")");
      stmt->execute(str_sql_tmp);
      // boost::this_thread::sleep(boost::posix_time::millisec(10)); //
      // 每条记录插入前间隔10ms，不间隔，是否对mysql有冲击
    }

    std::cout << "thread: " << i << std::endl;
    // boost::this_thread::sleep(boost::posix_time::seconds(200));

    // delete res;
    delete stmt;
    delete con;

  } catch (sql::SQLException &e) {
    //有异常的情况下，输出异常
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
}

// TODO 对应的存储过程和结果不一致，具体使用修改
void call_procedure(int i) {
  boost::this_thread::sleep(boost::posix_time::seconds(2));
  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res; //用于存储过程返回结果

    driver = get_driver_instance();
    con = driver->connect("192.168.221.136", "linjj", "19900801");
    //选择要连接的数据库
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");

    //执行存储过程
    std::stringstream str_id;
    str_id << i;
    std::string sql_text("call insert_test_data(");
    sql_text.append(str_id.str());
    sql_text.append(")");

    pstmt = con->prepareStatement(sql_text);
    pstmt->execute();

    /*或者使用
    std::string sql_text("call insert_test_data(?)");

    pstmt = con->prepareStatement(sql_text);
    pstmt->setInt(1, i);
    */

    //遍历存储过程的结果(包括存储过程中的select 返回或者 存储过程产生的错误[使用
    //show errors; ])
    do {
      res = pstmt->getResultSet();
      while (res->next()) {
        std::cout << "id: " << res->getInt("id")
                  << " name: " << res->getString("name") << std::endl;
      }
    } while (pstmt->getMoreResults());

    delete pstmt;
    delete con;

  } catch (sql::SQLException &e) {
    //有异常的情况下，输出异常
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
}

void get_record_from_mysql() {
  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    // sql::PreparedStatement *pstmt;
    std::map<int, std::string> map_data;
    std::string save_file_name("get_record_from_mysql.192.168.221.135.txt");
    std::stringstream data;

    driver = get_driver_instance();
    con = driver->connect("192.168.221.135", "linjj", "19900801");
    //选择要连接的数据库
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");
    stmt = con->createStatement();
    res = stmt->executeQuery("select uid, name from test1");
    while (res->next()) { //数据先写入内存，在一次性写入磁盘
      int uid = res->getInt("uid");
      std::string name = res->getString("name");
      data << uid << "\n";
      data << name << "\n";
      // map_data.insert(std::pair<int, std::string>(uid, name));
      // std::cout<<"record, uid: "<<uid<<", name: "<<name<<std::endl;
    }

    write_data_to_disk_file(save_file_name, data.str());

    delete res;
    delete stmt;
    delete con;

  } catch (sql::SQLException &e) {
    //有异常的情况下，输出异常
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
}

class action_record_time {
public:
  action_record_time() {
    start = boost::posix_time::microsec_clock::local_time();
  }
  ~action_record_time() {
    end = boost::posix_time::microsec_clock::local_time();

    boost::posix_time::time_duration cost = end - start;

    std::stringstream str_cost_time;
    std::string file_name("cost_time.txt");
    str_cost_time << cost.total_milliseconds() << "ms";
    write_data_to_disk_file(file_name, str_cost_time.str());
  }

public:
  boost::posix_time::ptime start, end;
};

int main() {
  // 一部分线程写入主库、另开线程读取数据并download到本地
  {
    action_record_time dr;
    boost::thread th_read(boost::bind(get_record_from_mysql));
    th_read.join();
    // get_record_from_mysql();
  }

  {
    action_record_time dr;
    boost::thread_group thread_group;
    for (int i = 0; i < 800; ++i) {
      thread_group.create_thread(boost::bind(&call_procedure, i));
      boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    }
    thread_group.join_all();
  }

  boost::this_thread::sleep(boost::posix_time::seconds(5));
  return 0;
}