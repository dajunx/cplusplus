// new 的另一种形式
/*
new 的时候，用已经分配内存的地址返回给待分配内存指针。比较特殊
*/

#include <iostream>
class test {
public:
  test() {}
  ~test() {}
};

int main() {
  void *pp = new int(5);
  // int* pp = new int(5); // this way has the same result
  int *p = new (pp) int();
  return 0;
}