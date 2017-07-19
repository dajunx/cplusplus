//类函数 指针 (未单独编译)
//参考链接：https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer

#include <iostream>
#include <map>
#include <vector>

class test1
{
public:
  test1() {}
  ~test1() {}
  void show() {
    int i = 0;
    std::cout<<"i am show function"<<std::endl;
  }
};

//高级应用
class test2
{
public:
  test2() {}
  ~test2() {}
  struct ff
  {
    void null() {
      std::cout<<"i am class test2::ff function."<<std::endl;
    }
  };

  typedef void(ff::*null)();
  operator null()
  {
    int i = 0;
    return &ff::null;
  }
  ff f1;
};

int main()
{
  test1 t1;
  void (test1::*p)() = NULL;
  p = &test1::show;
  (t1.*p)();

  test2 t2;
  void (test2::ff::*p1)() = NULL;
  p1 = t2;
  (t2.f1.*p1)();

  return 0;
}