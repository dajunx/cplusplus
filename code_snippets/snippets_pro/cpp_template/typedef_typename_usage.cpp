// typedef typename 作用
/*
如果把注释取消，就会产生编译错误。必须使用关键字typename的原因是T是一个template
parameter。 在实例化之前，编译器对T一无所知，因此不知道A::a_type
代表的是一个type或是一个member function或是一个data member，
使用typename可以告诉编译器这是一个type使得编译能顺利通过。
*/
// link:http://blog.csdn.net/fallstones/article/details/6265764

#include <iostream>
template <typename T> class A {
public:
  typedef T a_type;
};
template <typename A> class B {
public:
  // typedef A::a_type b_type;
  typedef typename A::a_type b_type;
  b_type x;
};

int main() {
  B<A<int>> b;
  b.x = 9;
  return 0;
}