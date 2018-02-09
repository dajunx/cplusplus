// typedef 与 模板函数 结合产生的坑，原因待分析
/*
typedef 与 模板函数结合使用的时候（typedef 别名月模板参数名相同），
  若2个定义都放在头文件中，在使用的时候 会造成使用达不到预想的效果。
*/
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/typeof/typeof.hpp>
#include <iostream>

#include "common.h"

template <typename des>
void test1::fun1(boost::shared_ptr<base> const &ptr_base) {
  boost::shared_ptr<des> ptr_ch = boost::dynamic_pointer_cast<des>(ptr_base);
  if (ptr_ch) {
    ptr_ch->show();
  } else {
    int i = 0;
  }
}

int main() {
  boost::shared_ptr<child1> ptr_ch1 = boost::make_shared<child1>();
  boost::shared_ptr<child2> ptr_ch2 = boost::make_shared<child2>();

  test1 tt;
  tt.fun1<child1>(boost::dynamic_pointer_cast<base>(ptr_ch1));
  tt.fun1<child2>(boost::dynamic_pointer_cast<base>(ptr_ch2));

  return 0;
}

//---------------------------------------------------------------------------
// common.h file:

class base {
public:
  base() {}
  virtual ~base() {}
};

class child1 : public base {
public:
  child1() {}
  ~child1() {}
  void show() { std::cout << "child1" << std::endl; }
};

class child2 : public base {
public:
  child2() {}
  ~child2() {}
  void show() { std::cout << "child2" << std::endl; }
};

class test1 {
public:
  test1() {}
  ~test1() {}

  // 在类里面，这个 typedef 会造成fun1函数针对主函数的第二次调用<child2>
  // 产生问题！ 解决办法：typedef child1 des; 放在cpp中即可
  typedef child1 des;

  template <typename des> void fun1(boost::shared_ptr<base> const &ptr_base);
};
