//函数返回值的生命周期

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef boost::shared_ptr<int> PTR_INT;
PTR_INT ptr_int;

inline PTR_INT fun2();
PTR_INT fun1()
{
  return fun2();
}

PTR_INT fun2()
{
  return ptr_int;
}

int main()
{
  ptr_int = boost::make_shared<int>(9);
  fun1();
  return 0;
}