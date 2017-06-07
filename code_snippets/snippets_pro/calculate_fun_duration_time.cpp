//程序计算 时间差
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
  boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
  std::map<std::string, int> mm;
  std::stringstream map_key;
  for (int i=1000; i < 100000;++i)
  {
    map_key.str("");
    map_key<<i;
    mm.insert(std::pair<std::string, int>(map_key.str(), i));
  }
  boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();

  boost::posix_time::time_duration cost = end - start;
  std::cout<<cost.total_milliseconds()<<"ms"<<std::endl;

  start = boost::posix_time::microsec_clock::local_time();
  std::map<int, int> mm1;
  for (int i=1000; i < 100000;++i)
  {
    map_key.str("");
    map_key<<i;
    map_key.str();
    mm1.insert(std::pair<int, int>(i, i));
  }
  end= boost::posix_time::microsec_clock::local_time();

  cost = end - start;
  std::cout<<cost.total_milliseconds()<<"ms"<<std::endl;
  return 0;
}