//消息分发
//使用了boost 的function和bind来做的。

#include <iostream>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::function<void()> fun;
std::vector<fun> vec_fun;

#define HANDLE_PROTO(index) vec_fun[##index] = boost::bind(&fun##index);

#define FUN_LIST(index)                                                        \
  void fun##index() { std::cout << "fun " << index << std::endl; }

FUN_LIST(1);
FUN_LIST(2);
FUN_LIST(3);

void dispatch(int index) { vec_fun[index](); }

int main() {
  vec_fun.resize(4);

  HANDLE_PROTO(1);
  HANDLE_PROTO(2);
  HANDLE_PROTO(3);

  for (int i = 1; i < 4; ++i) {
    dispatch(i);
  }

  return 0;
}

//---------------------------------------------
// old_code:

#include <iostream>
#include <map>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::function<void(int index)> fun;
std::map<int, fun> map_fun;

void fun1(int index) { std::cout << "fun " << index << std::endl; }

void fun2(int index) { std::cout << "fun " << index << std::endl; }

void fun3(int index) { std::cout << "fun " << index << std::endl; }

void register_fun() {
  map_fun.insert(
      std::make_pair<int, fun>(0, boost::bind(&fun1, boost::lambda::_1)));
  map_fun.insert(
      std::make_pair<int, fun>(1, boost::bind(&fun2, boost::lambda::_1)));
  map_fun.insert(
      std::make_pair<int, fun>(2, boost::bind(&fun3, boost::lambda::_1)));
}

void dispatch(int index) { map_fun[index](index); }

int main() {
  register_fun();

  for (int i = 0; i < 3; ++i) {
    dispatch(i);
  }

  return 0;
}