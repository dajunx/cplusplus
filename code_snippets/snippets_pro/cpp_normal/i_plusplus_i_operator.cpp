// i++ 与++i 操作符重载
/*
Time operator++();//重载++i;
Time operator++(int);//重载i++；
C++约定，后置自增/自减运算符增加一个int型参数。
*/

#include <iostream>

class test1 {
public:
  test1() : x(0) {}
  test1 &operator++() {
    ++x;
    return *this;
  }
  test1 &operator++(int) {
    test1 t1(*this);
    ++this->x;
    return *this;
  }
  int x;
};
int main() {
  test1 t1;
  t1++;
  ++t1;
  return 0;
}