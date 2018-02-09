// boost thread库mutex使用
//函数作用域的锁（放在函数开始），会在return
//之后再解锁（把return的数据压栈）？待在研究
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/thread/lock_guard.hpp> // lock_guard 头文件
#include <boost/thread/lock_types.hpp> // unique_lock 头文件
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp> // shared_lock 需要
#include <boost/thread/thread.hpp>
#include <iostream>

boost::mutex mutex_;
boost::shared_mutex shared_mutex_;
int temp = 0;
#define FOR_LOOP_COUNT (10000)

void fun1() {
  for (int i = 0; i < FOR_LOOP_COUNT; ++i) {
    mutex_.lock();
    temp++;
    mutex_.unlock();
  }
}

void fun2() {
  // lock_guard 与fun1类似，只是满足RAII风格，构造时lock，析构时调用unlock
  boost::lock_guard<boost::mutex> l(mutex_);
  for (int i = 0; i < FOR_LOOP_COUNT; ++i) {
    temp++;
  }
}

void fun3() {
  // unique_lock 内部增加is_locked判断是否加锁,有try_lock timed_lock
  boost::unique_lock<boost::mutex> l(mutex_);
  for (int i = 0; i < FOR_LOOP_COUNT; ++i) {
    temp++;
  }
}

void fun4() {
  // shared_lock
  boost::shared_lock<boost::shared_mutex> l(shared_mutex_);
  for (int i = 0; i < FOR_LOOP_COUNT; ++i) {
    temp++;
  }
}

void fun5() {
  //在 mutex 下 unique_lock<mutex>
  boost::mutex::scoped_lock l(mutex_);
  for (int i = 0; i < FOR_LOOP_COUNT; ++i) {
    temp++;
  }
}

int main() {
  boost::function<void()> ff = boost::bind(&fun5);
  boost::thread th1(ff);
  boost::thread th2(ff);
  th1.join();
  th2.join();

  std::cout << "temp :" << temp << std::endl;
  return 0;
}