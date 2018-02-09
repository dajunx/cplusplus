//友元函数 友元类
#include <iostream>

class test {
public:
  test() {}
  ~test() {}

private:
  void doSomeThing() { int i = 0; }
  friend void do_private_fun(test *pT);
  friend class test_friend;
};

class test_friend {
public:
  test_friend() {}
  ~test_friend() {}
  void do_class_private_fun(test *pT) { pT->doSomeThing(); }
};

void do_private_fun(test *pT) { pT->doSomeThing(); }

int main() {
  test tt;
  test_friend tf;
  do_private_fun(&tt);
  tf.do_class_private_fun(&tt);
  return 0;
}