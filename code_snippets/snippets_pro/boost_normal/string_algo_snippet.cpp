// string_algo 简单使用

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

void fun1() {
  std::string src("filter_src&linjianjun&content");
  std::vector<std::string> vec_str;
  boost::split_regex(vec_str, src, boost::regex("&"));
  // vec_str has three items, eatch data is: filter_src, linjianjun, content

  std::string str1("filter_src=linjianjun&content=linjianjun");
  std::vector<std::string> SplitVec;
  // is_any_of 表示其中任何一个 字符 都可以作为分隔符， - 或者 *
  // token_compress_on
  // 表示去掉字符串两端分割出来的空的token，默认是token_compress_off
  boost::split(SplitVec, str1, boost::is_any_of("=&"),
               boost::token_compress_on);
  // SplitVec has four items, each data is: filter_src, linjianjun, content,
  // linjianjun
}

int main(int argc, char *argv[]) {
  std::stringstream ret_str_tmp;
  std::vector<std::string> strFilter;
  boost::split(strFilter, temp_str, boost::is_any_of("<>"));
  boost::iterator_range<std::string::iterator> ir;

  for (BOOST_AUTO(it, strFilter.begin()); it != strFilter.end(); ++it) {
    ir = boost::find_head(*it, 3); //去头部,以"img"字符开头;
    if (true == boost::equals(std::string(ir.begin(), ir.end()), "img")) {
      ret_str_tmp << "<" << *it << ">";
    } else {
      ret_str_tmp << dc.applyFilter_new(*it, hit_filter);
    }
  }
  ret_str = ret_str_tmp.str();

  return 0;
}