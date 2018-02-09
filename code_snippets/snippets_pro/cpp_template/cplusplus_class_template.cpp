// c++ 类模板
/*
模板被实例化2次，1次是实例化前先检查模板代码本身，语法是否正确；2次是在实例化期间，检查模板代码，查看是否所有的调用都有效。
对于类模板，成员函数只有在被使用的时候才会被实例化，既节约了空间和时间，有能保证对于“未能提供所有成员函数中所有操作的”类型的安全
*/
#include <iostream>
#include <string>
#include <vector>

template <typename T> class stack {
private:
  std::vector<T> elems;

public:
  void push(T const &);
  T pop();
  T top() const;
  bool empty() { return elems.empty(); }
};

template <typename T> void stack<T>::push(T const &add_elems) {
  elems.push_back(add_elems);
}

template <typename T> T stack<T>::pop() {
  if (elems.empty()) {
    return;
  }

  T tmp = elems.back();
  elems.pop_back();
  return tmp;
}

template <typename T> T stack<T>::top() const {
  if (elems.empty()) {
    return;
  }

  return elems.back();
}

int main() {
  stack<int> sta_int;
  stack<std::string> str_string;

  sta_int.push(1);
  str_string.push("lin");
  return 0;
}