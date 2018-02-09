//头文件static方法 被多个cpp包含 数据共享问题
/*
当h头文件中含有static方法，在头文件被多个cpp文件包含且此方法有数据共享的时候
，会有问题存在， 1、若static方法不在类中，则 多个cpp文件不共享
static方法内容。反之 static在类中，则共享。 2、非static方法
不能放在头文件中，否则链接出错(error LNK2005: "void __cdecl fun3(void)"
(?fun3@@YAXXZ) already defined in test.obj、fatal error LNK1169: one or more
multiply defined symbols found)，
可行的修改方式是非static方法声明放在头文件，定义放在一个cpp文件中。
*/
#include "common.h"
#include <boost/thread/thread.hpp>
#include <vector>
std::map<int, int> map_int;
test2 t2;
boost::mutex mutex_main;

void fun1() {
  int index = 0;
  for (int i = 0; i < 1000; ++i) {
    boost::mutex::scoped_lock l(mutex_main);
    index = t2.show();
    map_int.insert(std::make_pair<int, int>(index, index));
  }
}

void fun2() {
  int index = 0;
  for (int i = 0; i < 1000; ++i) {
    boost::mutex::scoped_lock l(mutex_main);
    index = test2::get_request_id();
    map_int.insert(std::make_pair<int, int>(index, index));
  }
}

void check() {
  for (int i = 0; i < 2000; ++i) {
    if (i != map_int[i]) {
      std::cout << "err,index: " << i << ", value: " << map_int[i] << std::endl;
    }
  }
}

int main(int argc, char *argv[]) {
  boost::thread th1(fun1);
  boost::thread th2(fun2);
  th1.detach();
  th2.detach();

  boost::this_thread::sleep(boost::posix_time::seconds(5));
  check();
  return 0;
}

//-----------------------------------------------------------
//  common.h:
#include <boost/thread/mutex.hpp>

class test2 {
public:
  test2();
  ~test2();
  int show();

  static int get_request_id() {
    int ret;
    static boost::mutex mutex_request_id_seq_;
    static volatile int request_id_seq_;
    {
      boost::mutex::scoped_lock l(mutex_request_id_seq_);
      request_id_seq_++;
      if (request_id_seq_ == 0) {
        request_id_seq_++;
      }
      ret = request_id_seq_;
    }

    return ret;
  }
};

//-----------------------------------------------------------
//  test2.cpp:

#include "common1.h"

test2::test2() {}

test2::~test2() {}

int test2::show() { return get_request_id(); }
