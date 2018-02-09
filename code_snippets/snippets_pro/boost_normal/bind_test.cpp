// bind 测试
/*
bind中第二个参数
若是指针，则在内部不会调用对应的拷贝构造函数；否则要调用多次拷贝构造函数.
boost::lambda 参数只有3个，有限制，使用 _1 _2这种方式可以防止8个参数到bind中
*/
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>

class test1 {
public:
  test1() {}
  test1(const test1 &p) {}
  ~test1() {}
  void do_something1(int &i, int &j) {
    i = 9;
    j = 11;
  }
  void do_something2(int &x_, int &y_, int &z_, int &f_, int &g_, int &h_,
                     int &i_, int &j_) {
    x_ = 7;
    y_ = 7;
    z_ = 7;
    f_ = 7;
    g_ = 7;
    h_ = 7;
    i_ = 7;
    j_ = 7;
  }
  // 第三种方式
  void bind() {
    int x = 0, y = 0;
    test1 *p = this;
    boost::bind(&test1::do_something1, p, boost::lambda::_1,
                boost::lambda::_2)(x, y);
  }
};
int main() {
  test1 t1;
  // t1.bind();
  //   boost::function<void(int&)> f(boost::bind(&test1::do_something, t1,
  //   boost::lambda::_1)); int x = 0; f(x);
  // 第二种使用bind方式
  int x = 0, y = 0, z = 0, f = 0, g = 0, h = 0, i = 0, j = 0;
  // bind中第二个参数
  // 若是指针，则在内部不会调用对应的拷贝构造函数；否则要调用多次拷贝构造函数
  // boost::bind(&test1::do_something, /*t1*/&t1, boost::lambda::_1,
  // boost::lambda::_2)(x, y);
  // 这种方式 不用顾忌 boost::lambda 只有_1 _2 _3三个参数问题了
  boost::bind(&test1::do_something2, &t1, _1, _2, _3, _4, _5, _6, _7,
              _8)(x, y, z, f, g, h, i, j);
  return 0;
}