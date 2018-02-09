/*
参照 effective c++ item.17
c++函数调用参数 并不总是以特定次序完成函数参数的核算，如54行代码的调用顺序，
    初看可能是 new man,boost::shared_ptr,right 的顺序，
    其实顺序是 new man,right，boost::shared_ptr 的顺序！！！
*/

#include <fstream>
#include <iostream>
#include <string>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;

void write(std::string &data) {
  std::fstream file("file.txt",
                    std::fstream::in | std::fstream::out | std::fstream::app);
  file << data;
  file.close();
}

class Man {
public:
  Man() {
    std::string tmp("new man");
    write(tmp);
  }
  ~Man() {
    std::string tmp("releaes man");
    write(tmp);
    std::cout << "i'm a man." << std::endl;
  }
  void show_data() { std::cout << "man show data." << std::endl; }
};

void fun1(boost::shared_ptr<Man> ptr_tmp, int rights) { ptr_tmp->show_data(); }

int right() {
  throw 0;
  return 5;
}

int main() {
  // 例子使得 Man构造的临时对象 内存泄露！！！
  try {
    fun1(boost::shared_ptr<Man>(new Man()), right());
  } catch (...) {
    int j = 0;
  }

  int i = 0;
  return 0;
}