//模板 关键字 typename
#include <iostream>
#include <vector>

template <typename T> void print(T &coll) {
  typename T::iterator begin_pos;
  for (begin_pos = coll.begin(); begin_pos != coll.end(); ++begin_pos) {
    std::cout << "data : " << *begin_pos << std::endl;
  }
}

void print1(std::vector<int> const &coll) {
  // coll传参为const引用，对应此处得使用 const_iterator
  std::vector<int>::const_iterator begin_pos;
  for (begin_pos = coll.begin(); begin_pos != coll.end(); ++begin_pos) {
    std::cout << "data : " << *begin_pos << std::endl;
  }
}

int main() {
  std::vector<int> vec_int;
  vec_int.push_back(1);
  vec_int.push_back(2);
  print1(vec_int);

  return 0;
}