// vector 迭代器失效例子
//从vector中删除某个元素并把元素insert到末尾，对应的迭代器会失效

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/typeof/typeof.hpp>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>

int main() {
  std::vector<int> test;
  test.push_back(1);
  test.push_back(2);
  test.push_back(3);
  test.push_back(4);
  test.push_back(5);

  BOOST_AUTO(it, test.begin());
  int times = 1, size = test.size();
  for (; it != test.end() && times <= size; ++times) {
    if (*it == 5) {
      int temp = *it;
      it = test.erase(it);
      test.insert(test.end(), temp);
    } else {
      std::cout << *it++ << std::endl;
    }
  }
  return 0;
}