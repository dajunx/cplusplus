/*
  使用boost 库 分割 string 文本内容;
*/
#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>
#include <boost/token_functions.hpp>

void fun(std::string& input) {
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(":");
  tokenizer tokens(input.begin(), input.end(), sep);

  tokenizer::iterator it = tokens.begin();
  if (it == tokens.end()) {
    int i = 0;
  } else {
    if (++it == tokens.end()) {
      int i = 0;
    }

    std::string temp = *it;
  }
}

int main() {
  std::string uid_and_chang_info("1:2,2,2");
  fun(uid_and_chang_info);

  uid_and_chang_info = "";
  fun(uid_and_chang_info);

  uid_and_chang_info = "1,2,3";
  fun(uid_and_chang_info);

  return 0;
}