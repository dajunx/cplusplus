// shared_ptr 循环引用问题
// std::shared_ptr 和 std::weak_ptr的用法以及引用计数的循环引用问题
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>

class Woman;
class Man {
private:
  boost::weak_ptr<Woman> _wife;
  // boost::shared_ptr<Woman> _wife;
public:
  void setWife(boost::shared_ptr<Woman> woman) { _wife = woman; }
  ~Man() { std::cout << "kill man\n"; }
};

class Woman {
private:
  // boost::weak_ptr<Man> _husband;
  boost::shared_ptr<Man> _husband;

public:
  void setHusband(boost::shared_ptr<Man> man) { _husband = man; }
  ~Woman() { std::cout << "kill woman\n"; }
};

int main(int argc, char **argv) {
  boost::shared_ptr<Man> m(boost::make_shared<Man>());
  boost::shared_ptr<Woman> w(boost::make_shared<Woman>());
  if (m && w) {
    m->setWife(w);
    w->setHusband(m);
  }
  return 0;
}