#ifndef COMMON_DEF
#define COMMON_DEF

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/enable_shared_from_this.hpp>

// 解析命令，返还一对命令/值 对;
// set foo lin; get foo; shutdown;
void parse_cmd_and_content(char* content, std::vector<std::string>& cmd_pair)
{
  if (!content) return;

  //std::string content(content);
}

#endif