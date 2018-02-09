/*
std::chrono::steady_clock
为了表示稳定的时间间隔，后一次调用now()得到的时间总是比前一次的值大（这句话的意思其实是，如果中途修改了系统时间，也不影响now()的结果），每次tick都保证过了稳定的时间间隔。
duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
这个是记录的开机到现在的时间间隔
*/

#include <boost/chrono.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <string>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

boost::posix_time::ptime base_at;
void fun() {
  // boost::posix_time::ptime base_at;
  using namespace boost::chrono;
  // tick_count_now 记录开机到现在的时间
  boost::int64_t tick_count_now =
      duration_cast<microseconds>(steady_clock::now().time_since_epoch())
          .count();

  boost::int64_t tick_count_now_ =
      duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();

  std::string base_str;
  base_at = boost::posix_time::microsec_clock::local_time();
  base_str = boost::posix_time::to_simple_string(base_at);

  base_at -= boost::posix_time::microsec(tick_count_now);
  base_str = boost::posix_time::to_simple_string(base_at);

  int i = 0;
}

void fun3() {
  using namespace boost::chrono;
  boost::posix_time::ptime time =
      base_at +
      boost::posix_time::microsec(
          duration_cast<microseconds>(steady_clock::now().time_since_epoch())
              .count());

  std::cout << "localtime :" << boost::posix_time::to_simple_string(time)
            << std::endl;
}

void fun1() {
  using namespace boost::chrono;

  system_clock::time_point tp = system_clock::now();
  system_clock::duration dtn = tp.time_since_epoch();

  std::cout << "current time since epoch, expressed in:" << std::endl;
  std::cout << "periods: " << dtn.count() << std::endl;
  std::cout << "seconds: "
            << dtn.count() * system_clock::period::num /
                   system_clock::period::den;
  std::cout << std::endl;
}

int main() {
  fun();
  fun3();

  return 0;
}