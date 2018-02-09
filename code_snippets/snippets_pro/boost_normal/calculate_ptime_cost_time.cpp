//计算运行时间ptime
//转换ptime 为string类型：boost::posix_time::to_simple_string
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>
#define loop_num (1)

int main() {
  long total_costtime = 0;
  for (int j = 0; j < loop_num; ++j) {
    boost::posix_time::ptime start_time =
        boost::posix_time::microsec_clock::local_time();
    std::vector<int> vec_int;
    for (int i = 0; i < 10000000; ++i) {
      vec_int.push_back(i);
    }
    boost::posix_time::ptime end_time =
        boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration cost = end_time - start_time;
    total_costtime += cost.total_milliseconds();
  }
  std::cout << "cost time1:" << total_costtime / loop_num << " ms."
            << std::endl;
  return 0;
}