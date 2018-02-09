//访问类成员函数、普通函数
// link:http://stackoverflow.com/questions/10231330/how-to-get-a-methods-address-in-a-class

#include <boost/function.hpp>
#include <iostream>

class test1 {
public:
  test1() {}
  ~test1() {}
  void fun() {}
};

void fun1() { int i = 0; }

int main() {
  // 定义类成员函数 原始方式
  void (test1::*ff)(void);
  ff = &test1::fun;

  test1 t1;
  (t1.*ff)();

  // 定义普通函数指针方式
  void (*pfun)();
  pfun = &fun1;
  pfun();

  // boost function 方式
  boost::function<void(test1 *)> ff1(&test1::fun);
  ff1(&t1);
  return 0;
}