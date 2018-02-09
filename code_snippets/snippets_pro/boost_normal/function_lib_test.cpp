#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/system/error_code.hpp>

void fun_2(int c) { std::cout << "fun_2------------" << std::endl; }

class test1 {
public:
  test1() {}
  ~test1() {}
  void do_something(int) {}
};

int main(void) {
  // 类对象也从调用位置传入
  boost::function<void(test1 &, int)> f1 =
      boost::bind(&test1::do_something, boost::lambda::_1, boost::lambda::_2);
  test1 t1;
  f1(t1, 9);
  boost::function<void(int)> f2 =
      boost::bind(&test1::do_something, t1, boost::lambda::_1);
  f2(9);
  return 0;
}