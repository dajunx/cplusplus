// shared_ptr 引用计数use_count_
/*
1、fun1 函数传递方式，在函数内部use_count_为2，传引用是1；
2、使用 io_service post 到函数内部是4，引用是3；
3、使用线程去执行，到函数内部是 3，引用是2。
4、传递函数 const& 不会增加use_count，而bind const&参数 会增加use_count
  (ps:若用boost::ref包裹这个const&参数 则不会增加use_count)
*/
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>

using namespace boost;
typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
typedef boost::shared_ptr<boost::asio::io_service::work> io_work_ptr;

struct test1 {
  test1() {}
  ~test1() {}
};

struct test2 {
  test2() {}
  ~test2() {}
};

boost::function<void(boost::shared_ptr<test1> const &)> f;
void fun2(boost::shared_ptr<test1> const &ptr_test1) { int i = 0; }

void fun1(boost::shared_ptr<test1> const &ptr_test1) {
  // 第4点：函数参数为 const& 不会增加use_count，而bind const&参数
  // 会增加use_count
  boost::shared_ptr<test2> ptr_temp = boost::make_shared<test2>();
  int i = 0;
  f = boost::bind(&fun2, ptr_test1);
  i = 1;
}

int main() {
  io_service_ptr ptr_io_service = boost::make_shared<boost::asio::io_service>();
  io_work_ptr ptr_io_work = boost::make_shared<boost::asio::io_service::work>(
      boost::ref(*ptr_io_service));

  boost::thread th(boost::bind(&boost::asio::io_service::run, ptr_io_service));
  th.detach();

  boost::shared_ptr<test1> ptr_test1 = boost::make_shared<test1>();
  fun1(ptr_test1);
  // ptr_io_service->post(boost::bind(&fun1, ptr_test1));
  // boost::thread th1(boost::bind(&fun1, ptr_test1));

  boost::this_thread::sleep(boost::posix_time::milliseconds(100));

  return 0;
}