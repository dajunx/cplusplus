//随机数 重复了统计

#include <algorithm> // std::random_shuffle
#include <boost/cstdint.hpp>
#include <ctime> //time
#include <iostream>
#include <map>
#include <vector>
std::map<boost::uint64_t, boost::uint64_t> map_collect_random;

void fun1(std::vector<int> &vec_tmp) {
  boost::uint64_t num = 0;
  std::vector<int>::reverse_iterator it = vec_tmp.rbegin();
  for (boost::uint64_t i = 1; it != vec_tmp.rend(); ++it) {
    num += (*it) * i;
    i *= 10;
  }
  map_collect_random[num] = num;
}

int main() {
  std::vector<int> vec_int;
  std::vector<int> vec_int_tmp;
  for (int i = 0; i < 10; ++i)
    vec_int.push_back(i);

  std::srand(unsigned(std::time(0)));
  for (int i = 0; i < 1000000; ++i) {
    vec_int_tmp = vec_int;
    std::random_shuffle(vec_int_tmp.begin(), vec_int_tmp.end());
    fun1(vec_int_tmp);
  }

  return 0;
}