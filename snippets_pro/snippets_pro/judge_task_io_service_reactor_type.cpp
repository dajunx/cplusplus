//≈–∂œ task_io_service reactor ¿‡–Õ
#include <iostream>  
#include <string>  
#include <boost/asio.hpp>  
int main()  
{  
  std::string output;  
#if defined(BOOST_ASIO_HAS_IOCP)  
  output = "iocp" ;  
#elif defined(BOOST_ASIO_HAS_EPOLL)  
  output = "epoll" ;  
#elif defined(BOOST_ASIO_HAS_KQUEUE)  
  output = "kqueue" ;  
#elif defined(BOOST_ASIO_HAS_DEV_POLL)  
  output = "/dev/poll" ;  
#else  
  output = "select" ;  
#endif  
  std::cout << output << std::endl;  
}