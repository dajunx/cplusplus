// mysql-connector-c++ 执行/查询存储过程
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <driver/mysql_connection.h>
using namespace std;

int main() {
  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("localhost", "root", "");
    //选择要连接的数据库
    con->setSchema("test");
    //设置字符格式
    con->setClientOption("characterSetResults", "utf8");

    //执行存储过程
    pstmt = con->prepareStatement("call proc_adder(1, 5, @sum)");
    pstmt->execute();
    // or below
    // pstmt = con->prepareStatement("call proc_adder(?, ?, @sum)");
    // pstmt->setInt(1, 1);
    // pstmt->setInt(2, 5);
    // pstmt->execute();

    //查询存储过程结果
    pstmt = con->prepareStatement("select @sum as sum");
    res = pstmt->executeQuery();
    //遍历结果集
    while (res->next()) {
      int id = res->getUInt64(1);
      cout << id << endl;
    }

    delete res;
    // delete stmt;
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

  return 0;
}