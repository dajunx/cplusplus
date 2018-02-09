//获取 ptime 时间细分粒度，年月日时分秒
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

int main() {
  boost::posix_time::ptime time_now_ptime1 =
      boost::posix_time::microsec_clock::local_time();

  boost::posix_time::ptime time_now_ptime2 =
      boost::posix_time::microsec_clock::local_time();

  struct tm t = to_tm(time_now_ptime1);
  std::cout << "year:" << t.tm_year + 1900 << std::endl
            << ", month:" << t.tm_mon << std::endl
            << ", day:" << t.tm_mday << std::endl
            << ", hour:" << t.tm_hour << std::endl
            << ", min:" << t.tm_min << std::endl
            << ", sec:" << t.tm_sec << std::endl;

  std::cout << "type:" << typeid(time_now_ptime1.date().month()).name()
            << std::endl;
  std::cout << "time:" << time_now_ptime1.date().month() << std::endl;

  unsigned short month1 = time_now_ptime1.date().month();
  unsigned short month2 = time_now_ptime2.date().month();

  if (time_now_ptime1.date().month() == time_now_ptime2.date().month()) {
    int i = 0;
  }

  return 0;
}