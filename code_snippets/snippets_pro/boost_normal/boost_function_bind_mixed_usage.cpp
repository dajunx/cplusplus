// boost function bind 混合使用

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>

void fun1(int i, int j) {
  std::cout << "fun1 i:" << i << ", j:" << j << std::endl;
}

int main() {
  boost::function<void(int, int)> ff =
      boost::bind(&fun1, boost::lambda::_1, boost::lambda::_2);
  boost::function<void(int)> ff1 = boost::bind(ff, 9, boost::lambda::_1);
  ff1(10);

  int i = 0;

  return 0;
}