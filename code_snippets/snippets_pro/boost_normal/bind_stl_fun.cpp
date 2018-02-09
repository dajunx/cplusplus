// bind 绑定stl函数
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <set>

typedef std::set<int> sset;

void insertData(boost::shared_ptr<sset> &ptr_set_int, int inputData) {
  ptr_set_int->insert(inputData);
}

int main(int argc, char *argv[]) {
  boost::shared_ptr<sset> ptr_set1 = boost::make_shared<sset>();
  boost::shared_ptr<sset> ptr_set2 = boost::make_shared<sset>();
  sset set_int;

  // win 端方式
  boost::function<void(const int &)> cb1 =
      boost::bind(&sset::insert, ptr_set1, boost::lambda::_1);

  // linux 下
  boost::function<void(const int &)> cb2 =
      boost::bind(&insertData, ptr_set2, boost::lambda::_1);

  for (int i = 0; i < 10; ++i) {
    cb1(i);
    cb2(i);
  }
  return 0;
}