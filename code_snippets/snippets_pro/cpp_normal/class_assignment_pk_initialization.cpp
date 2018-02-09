
// class 中
// 初始化和赋值不同点，注意25~28行代码动作！首先对t2调用default构造，28号在调用带有一个int参数的构造函数！！！
#include <iostream>
#include <string>

#define call_with_max(a, b)

class test2 {
public:
  test2() { j = 0; }
  test2(int a) { j = a; }
  test2 &operator=(const test2 &t2) {
    this->j = t2.j;
    return *this;
  }
  ~test2() {}
  int j;
};

class test1 {
public:
  test1(test2 &t2) {
    int i = 0;
    a = t2;
  }
  ~test1() {}
  test2 a;
};

int main() {
  test2 t2(5);
  test1 t1(t2);

  return 0;
}