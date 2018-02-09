// wrap 实现和使用
//把函数调度放到某个线程中去执行

// main.cpp
#include "wrap.hpp"
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>
#include <iostream>
using namespace boost::asio;

void fun1() { int i = 0; }

int main() {
  io_service io;
  io_service::work work(io);
  boost::thread th(boost::bind(&io_service::run, boost::ref(io)));
  th.detach();

  boost::function<void()> ff = post_wrap(boost::ref(io), boost::bind(&fun1));

  ff(); // callback
  boost::this_thread::sleep(boost::posix_time::seconds(5));

  return 0;
}

//--------------------------------------------------------------------------------------------
// wrap.hpp
#include <boost/asio.hpp>
template <typename Handler> class post_wrapped_handler {
public:
  post_wrapped_handler(boost::asio::io_service &io, Handler h)
      : io_(io), h_(h) {}

  void operator()() { io_.post(h_); }

  template <typename Arg1> void operator()(Arg1 arg1) {
    io_.post(boost::bind(h_, arg1));
  }

  template <typename Arg1, typename Arg2>
  void operator()(Arg1 arg1, Arg2 arg2) {
    io_.post(boost::bind(h_, arg1, arg2));
  }

  template <typename Arg1, typename Arg2, typename Arg3>
  void operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
    io_.post(boost::bind(h_, arg1, arg2, arg3));
  }

  boost::asio::io_service &io_;
  Handler h_;
};

template <typename Handler>
inline post_wrapped_handler<Handler> post_wrap(boost::asio::io_service &io,
                                               Handler h) {
  return post_wrapped_handler<Handler>(io, h);
}