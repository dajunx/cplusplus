/*
  测试 template
  编程容易出现的一个问题。尚不能描述错处的原因，只知道当template和typedef有冲突的case。
  TODO 待研究
*/
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

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

  // 这个地方会让下面函数 typename
  typedef child1 des;
  template <typename des> void fun1(boost::shared_ptr<base> const &ptr_base);
  // ps:若把上述两行逻辑修改为如下实现，则child2 调用
  // fun1函数能够正常调用show函数
  // template <typename child1> void fun1(boost::shared_ptr<base> const
  // &ptr_base);
};

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
  // 在当前程序编码环境下，如下调用fun1函数 会造成
  // ptr_ch为NULL的情况，原因待探究
  tt.fun1<child2>(boost::dynamic_pointer_cast<base>(ptr_ch2));

  return 0;
}