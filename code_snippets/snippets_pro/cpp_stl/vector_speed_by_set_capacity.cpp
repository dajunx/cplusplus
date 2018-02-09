// vector 设置capacity和不capacity速度差异
/*
在release版本，分别在2个线程运行，每次运行10000000次，取100次平均值，reserve版本在非reserve版本一半时间，40多毫秒。
在debug版本，结果没啥差异，不解。每次运行10000000次，取10次平均值，时间在3300毫秒多点。
*/

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>
#define loop_num (100)

void fun2() {
  long total_costtime = 0;
  for (int j = 0; j < loop_num; ++j) {
    boost::posix_time::ptime start_time =
        boost::posix_time::microsec_clock::local_time();
    std::vector<int> vec_int;
    vec_int.reserve(10000000);
    for (int i = 0; i < 10000000; ++i) {
      vec_int.push_back(i);
    }
    boost::posix_time::ptime end_time =
        boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration cost = end_time - start_time;
    total_costtime += cost.total_milliseconds();
  }
  std::cout << "cost time2:" << total_costtime / loop_num << " ms."
            << std::endl;
}

void fun1() {
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
}

int main() {
  boost::thread th1(fun1);
  boost::thread th2(fun2);
  th1.join();
  th2.join();
  return 0;
}