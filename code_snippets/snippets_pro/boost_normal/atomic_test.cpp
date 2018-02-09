// atomic 原子变量
// link: http://www.boost.org/doc/libs/1_53_0/doc/html/atomic.html
#include <boost/atomic.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

// boost::atomic<int> a(0);
int a(0);

void fun1() {
  for (int i = 0; i < 10000; ++i) {
    ++a;
  }
}

int main() {
  for (int i = 0; i < 100; i++) {
    boost::thread t1(fun1);
    boost::thread t2(fun1);
    t1.join();
    t2.join();
    if (a != 20000)
      std::cout << a << '\n';
    a = 0;
  }
  return 0;
}