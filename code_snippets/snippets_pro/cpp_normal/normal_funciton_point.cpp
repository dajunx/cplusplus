//普通函数 指针

#include <iostream>
#include <map>
#include <vector>

typedef void fun1(int &i);

void incrNum(int &i) { i++; }

void show(fun1 *f1, int &input) { f1(input); }

int main() {
  fun1 *f1 = incrNum;
  int i = 0;
  // 函数指针，传进函数进行调度
  // show(incrNum, i);
  show(f1, i);

  return 0;
}

/*
#include <iostream>

void fun1()
{
int i = 0;
}

typedef void P_fun();

int main()
{
P_fun* p_fun1 = &fun1;
p_fun1();

return 0;
}
*/