//非boost库使用 function和bind

#include <functional>
#include <iostream>

void fun_2(int c) { std::cout << "fun_2------------" << std::endl; }

class test1 {
public:
  test1() {}
  ~test1() {}
  void do_something(int) {}
};

int main(void) {
  test1 t1;
  std::tr1::function<void(int)> f1 =
      std::bind(&test1::do_something, t1, std::placeholders::_1);
  std::tr1::function<void(test1 &, int)> f2 = std::bind(
      &test1::do_something, std::placeholders::_1, std::placeholders::_2);
  f1(9);
  f2(t1, 11);
  return 0;
}