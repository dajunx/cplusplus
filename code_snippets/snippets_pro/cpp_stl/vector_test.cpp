// vector 测试
/*
(1) empty container constructor (default constructor)
Constructs an empty container, with no elements.
(2) fill constructor
Constructs a container with n elements. Each element is a copy of val.
(3) range constructor
Constructs a container with as many elements as the range [first,last), with
each element constructed from its corresponding element in that range, in the
same order. (4) copy constructor Constructs a container with a copy of each of
the elements in x, in the same order.
*/
#include <iostream>
#include <vector>

class test1 {
public:
  test1() {}
  ~test1() {}
  test1(const test1 &a) {}
};

int main() {
  // Construct
  int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> vec_int;
  std::vector<int> vec_int1(
      &a[0], &a[9] + 1); //必须用&取得数组开始结束地址  [first,last)
  std::vector<int> vec_int4(a, a + sizeof(a) / sizeof(int));
  std::vector<int> vec_int2(4, 5);
  std::vector<int> vec_int3(vec_int2);
  // vec_int2.clear(); // copy constructor 和源对象后续无关

  // operator= 重新分配数据，会调用对应构造、析构、拷贝构造
  test1 b1, b2;
  std::vector<test1> vec_test1, vec_test2;
  vec_test1.push_back(b1);
  vec_test2.push_back(b1);
  vec_test2.push_back(b2);
  vec_test1 = vec_test2;

  // size max_size:3FFFFFFF capacity可容纳数目
  std::vector<int> vec_int01;
  int size = 0, max_size = 0, capacity = 0;
  for (int i = 0; i < 10; i++) {
    vec_int01.push_back(i);
    size = vec_int01.size();
    capacity = vec_int01.capacity();
    max_size = vec_int01.max_size();
  }

  // operator[] 与 at 差异 at 可捕获异常，[]不行
  try {
    vec_int01.at(10) = 8;
    vec_int01[10] = 9;
  } catch (std::underflow_error &e) {
    std::cout << "error reasion:" << e.what();
  } catch (std::out_of_range &e1) {
    std::cout << "error reasion:" << e1.what();
  }

  // swap 非销毁和重新组织数据
  std::vector<test1> vec_obj1, vec_obj2;
  vec_obj1.push_back(b1);
  vec_obj2.push_back(b2);
  vec_obj1.swap(vec_obj2);

  // front 当容器为空 行为未定义
  std::vector<int> vec_int11;
  //   std::vector<int>::reference it = vec_int11.front();
  //   it = 9;
  return 0;
}