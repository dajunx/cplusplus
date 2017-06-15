//mysql-connector-c++  连接/查询mysql
//暂时用的3rd库里面的动态库进行编译

//使用lib：  F:\Work\3rd\mysql-connector-c++\build\driver\Debug

#include <driver/mysql_connection.h> 
#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>

#include <iostream>
#include <string>
#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

void read_sql_from_disk_file(std::string& str_sql)
{
  std::ifstream fin("file.txt");  
  while(getline(fin, str_sql))
  {    
    std::cout<<"Read from file into string: "<<str_sql<<std::endl; 
  }
}

void insert_data_2_mysql(int i)
{
  boost::this_thread::sleep(boost::posix_time::seconds(2));
  try { 
    sql::Driver *driver; 
    sql::Connection *con; 
    sql::Statement *stmt; 
    //sql::ResultSet *res; 
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance(); 
    con = driver->connect("192.168.221.133", "linjj", "19900801"); 
    //选择要连接的数据库 
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");
    stmt = con->createStatement(); 

    std::string str_sql, str_sql_tmp;
    read_sql_from_disk_file(str_sql);
    for(int index=0; index < 10000; ++index)
    {
      str_sql_tmp = str_sql;
      int iBegin = i * 10000 + index;
      std::stringstream uid;
      uid<<iBegin;
      str_sql_tmp.append(uid.str());
      str_sql_tmp.append(", \"");
      str_sql_tmp.append(uid.str());
      str_sql_tmp.append("\")");
      stmt->execute(str_sql_tmp);
      boost::this_thread::sleep(boost::posix_time::millisec(10));
    }

    std::cout<<"thread: "<<i<<std::endl;
    //boost::this_thread::sleep(boost::posix_time::seconds(200));

    //delete res; 
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

void test_use_mysql(int i)
{
  try { 
    sql::Driver *driver; 
    sql::Connection *con; 
    sql::Statement *stmt; 
    sql::ResultSet *res; 
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance(); 
    con = driver->connect("192.168.221.133", "linjj", "19900801"); 
    //选择要连接的数据库 
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");
    stmt = con->createStatement(); 

    std::string str_sql;
    read_sql_from_disk_file(str_sql);
    res = stmt->executeQuery(str_sql); 
    //遍历结果集
    while (res->next())  
    { 
      std::string b1_res;
      //这里的ID是user表中的字段名
      int id = res->getInt("uid");
      std::string val = res->getString("name");
    }

    std::cout<<"thread: "<<i<<std::endl;
    boost::this_thread::sleep(boost::posix_time::seconds(200));

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

int main()
{
  boost::thread_group thread_group;
  for(int i=0; i<210; ++i)
  {
    thread_group.create_thread(boost::bind(&test_use_mysql, i));
    boost::this_thread::sleep(boost::posix_time::milliseconds(300));
  }
  thread_group.join_all();

  boost::this_thread::sleep(boost::posix_time::seconds(5));
  return 0;
}