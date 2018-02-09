//复杂函数指针
//复杂的函数指针，指针形参为 函数指针

#include <iostream>

typedef int (*pfun)(int);
typedef int (*pfunmulti)(pfun);

int fun1(int i) {
  i = 0;
  return i;
}

int fun2(pfun p_fun) { return (*p_fun)(9); }

int main() {
  pfun p_fun;
  pfunmulti p_fun_multi;
  //   int (*p_fun)(int i);
  //   int (*p_fun_multi)(int(*)(int));

  p_fun = &fun1;
  p_fun(5);
  p_fun_multi = &fun2;
  p_fun_multi(p_fun);

  return (0);
}