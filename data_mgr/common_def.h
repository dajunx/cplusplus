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
///TODO 函数变成 非inline之后 链接错误，提示 main中与 network_mgr 中标识符冲突
inline void parse_cmd_and_content(char* content, std::string& msg, std::string& key, std::string& value)
{
  if (!content) return;

  char* p_ch = content;
  std::string pair;
  bool first_para = true;

  while (*p_ch != '\0') {
    if (*p_ch != ' ') {
      pair.push_back(*p_ch);
    } else if (*p_ch == ' ') {
      if (!pair.empty() ) {
        first_para? msg = pair : key = pair;
        first_para = false;
      }
      pair.clear();
    }
    p_ch++;
  }

  if ( !pair.empty() ) {
    first_para? msg = pair : value = pair;
  }
}

#endif