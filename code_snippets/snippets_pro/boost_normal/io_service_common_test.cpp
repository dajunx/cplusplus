// asio io_service 例子
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
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

void fun1() { int i = 0; }

int main() {
  io_service_ptr ptr_io_service = boost::make_shared<boost::asio::io_service>();
  io_work_ptr ptr_io_work = boost::make_shared<boost::asio::io_service::work>(
      boost::ref(*ptr_io_service));

  // boost::thread th(work_fun, ptr_io_service);
  boost::thread th(boost::bind(&boost::asio::io_service::run, ptr_io_service));
  th.detach();

  boost::this_thread::sleep(boost::posix_time::milliseconds(100));

  return 0;
}