// steady_timer 定时器使用

// boost::posix_time::to_simple_string函数需要这两个头文件
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
//使用boost.chrono代替std.chrono,
#define BOOST_ASIO_DISABLE_STD_CHRONO
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread.hpp>

class MyClass {
public:
  MyClass() : m_work(m_io), m_timer(m_io) {}

public:
  boost::thread_group m_thgp;
  boost::asio::io_service m_io;
  boost::asio::io_service::work m_work;
  boost::asio::steady_timer m_timer;

public:
  void Init() {
    boost::system::error_code errCode;
    m_thgp.create_thread(
        boost::bind(&boost::asio::io_service::run, boost::ref(m_io), errCode));
    std::cout << "Init_1, " << LocalTime() << std::endl;
    m_timer.expires_from_now(
        boost::chrono::milliseconds(4000)); //设置过期时间长度
    std::cout << "Init_2, " << LocalTime() << std::endl;
    m_timer.async_wait(boost::bind(
        &MyClass::Test, this, boost::asio::placeholders::error)); //异步等待
    std::cout << "Init_3, " << LocalTime() << std::endl;
    //由Console可知, 函数立即返回了, 定时器的expires_from_now是由完成端口处理的
  }
  void Stop() {
    std::cout << "Stop_1, " << LocalTime() << std::endl;
    m_timer.cancel(); // 取消所有handler
    m_work.~work();
    m_thgp.join_all();
    std::cout << "Stop_2, " << LocalTime() << std::endl;
  }
  static std::string LocalTime() {
    return boost::posix_time::to_simple_string(
        boost::posix_time::microsec_clock::local_time());
  }

  void Test(const boost::system::error_code &ec) {
    printf("test_1, %s, ec.value=%d, ec.message=%s\n", LocalTime().c_str(),
           ec.value(), ec.message().c_str());
    if (ec)
      return;
    m_timer.expires_from_now(boost::chrono::milliseconds(4000));
    std::cout << "test_2, " << LocalTime() << std::endl;
    m_timer.async_wait(boost::bind(&MyClass::Test, boost::ref(*this), _1));
    std::cout << "test_3, " << LocalTime() << std::endl;
  }
};

int main(int argc, char **argv) {
  MyClass my;
  my.Init();
  for (int i = 0; i < 30; ++i) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
  }
  my.Stop();
  std::cout << "press ENTER to exit..." << std::endl;
  // 	std::cin.sync();
  // 	while (getchar() != '\n') {}
  return 0;
}