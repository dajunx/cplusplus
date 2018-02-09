// c++自带 rand，根据时间随机产生数据

#include "boost/random.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <time.h>

void fun(std::time_t time_now) {
  srand(unsigned(time_now));
  int temp = rand() % 50 + 1;
  std::cout << "time:" << time_now << ", temp:" << temp << std::endl;
}

int main() {
  for (int i = 0; i < 20; ++i) {
    fun(time(NULL));
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
  return 0;
}