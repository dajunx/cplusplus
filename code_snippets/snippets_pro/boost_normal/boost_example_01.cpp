// boost 例子

#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class test : public boost::enable_shared_from_this<test> {
public:
  int i;
  typedef boost::shared_ptr<test> ptr_test;

  test(int x) : i(x) {}
  ~test() { i = 7; }

public:
  static ptr_test get_instance(int temp) {
    static boost::mutex mutex;
    mutex.lock();
    static ptr_test ptr_static_test;
    printf("ptr_static_test 1\n");
    if (!ptr_static_test)
      ptr_static_test = boost::make_shared<test>(temp);
    printf("ptr_static_test %p\n", ptr_static_test.get());
    printf("ptr_static_test 2\n");
    mutex.unlock();
    return ptr_static_test;
  }
};

class service {
public:
  void get_impl(int x) {
    boost::shared_ptr<test> ptr_impl_test = test::get_instance(9);
    printf("ptr_impl_test %p\n", ptr_impl_test.get());
  }
};

void multi_thread(int tmp) {
  boost::shared_ptr<service> ptr_service = boost::make_shared<service>();
  ptr_service->get_impl(tmp);
  ptr_service.reset();
}

int main() {
  boost::thread th1(boost::bind(multi_thread, 1));
  th1.detach();
  boost::thread th2(boost::bind(multi_thread, 2));
  th2.detach();
  boost::thread th3(boost::bind(multi_thread, 3));
  th3.detach();
  boost::thread th4(boost::bind(multi_thread, 4));
  th4.detach();

  boost::this_thread::sleep(boost::posix_time::seconds(5));
  return 0;
}