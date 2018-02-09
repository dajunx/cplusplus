
// tuple 测试，库功能：更易于定义返回多个数值的函数
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <string>
boost::tuple<int, double> get_values() { return boost::make_tuple(1, 1.5); }
class base {
public:
  base() {}
  base(const base &a) {}
  virtual ~base(){};
  virtual void test() { std::cout << "base::test()\n"; }
};
class derived : public base {
public:
  derived() {}
  derived(const derived &a) {}
  virtual void test() { std::cout << "derived::test()\n"; }
};
int main() {
  // test for constructor
  boost::tuple<int, int, std::string> tuile0(1, 2, "lin");
  boost::tuple<int, int, std::string> tuile1;
  boost::tuple<int, int, std::string> tuile2(10);
  boost::tuple<int, int, double> tuile3(10);
  tuile1 = boost::make_tuple(1, 2, "jun");
  tuile2 = boost::make_tuple(1, 2, "jian"); // 初始化值后，还可以赋值
  // get value function
  int a = boost::tuples::get<0>(tuile2);
  int b = boost::tuples::get<1>(tuile2);
  std::string c = boost::tuples::get<2>(tuile2);
  // return value
  boost::tuple<int, double> tup_re = get_values();
  // test for copy constructor
  boost::tuple<int, std::string, derived> tup1(-5, "Tuples");
  derived bb;
  boost::tuple<int, std::string, derived> tup3;
  tup3 = boost::make_tuple(1, "lin", bb);
  boost::tuple<unsigned int, std::string, base> tup2;
  tup2 = tup1;
  tup1.get<2>().test();
  tup2.get<2>().test();
  // 遍历元素
  boost::tuples::tuple<int, int, double> tuple1(10, 30, 20.000);
  int head = tuple1.get_head();
  int tailhead = tuple1.get_tail().get_head();
  double tail = tuple1.get_tail().get_tail().get_head();
  return 0;
}