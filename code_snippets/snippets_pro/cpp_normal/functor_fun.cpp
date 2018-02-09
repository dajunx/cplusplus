//仿函数
/*
仿函数(functor)，就是使一个类的使用看上去像一个函数。其实现就是类中实现一个operator()，
  这个类就有了类似函数的行为，就是一个仿函数类了。
在我们写代码时有时会发现有些功能实现的代码，
  会不断的在不同的成员函数中用到，但是又不好将这些代码独立出来成为一个类的一个成员函数。
  但是又很想复用这些代码。写一个公共的函数，可以，这是一个解决方法，不过函数用到的一些变量，
  就可能成为公共的全局变量，再说为了复用这么一片代码，就要单立出一个函数，也不是很好维护。
  这时就可以用仿函数了，写一个简单类，除了那些维护一个类的成员函数外，就只是实现一个operator()，
  在类实例化时，就将要用的，非参数的元素传入类中。这样就免去了对一些公共变量的全局化的维护了。
  又可以使那些代码独立出来，以便下次复用。而且这些仿函数，还可以用关联，聚合，依赖的类之间的关系，
  与用到他们的类组合在一起，这样有利于资源的管理（这点可能是它相对于函数最显著的优点了）。
  如果在配合上模板技术和policy编程思想，那就更是威力无穷了，大家可以慢慢的体会。
*/
// link:http://baike.baidu.com/view/2070037.htm

#include <iostream>
class compare_class {
public:
  bool operator()(int A, int B) const { return A < B; }
};
// Declaration of C++ sorting function.
template <class ComparisonFunctor>
void sort_ints(int *begin_items, int num_items, ComparisonFunctor c) {
  c(begin_items[0], begin_items[1]);
}

int main() {
  int items[] = {4, 3, 1, 2};
  compare_class functor;
  sort_ints(items, sizeof(items) / sizeof(items[0]), functor);
}