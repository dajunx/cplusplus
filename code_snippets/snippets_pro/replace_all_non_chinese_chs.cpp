//替换掉内容中所有非中文字符，正则表达式为  [^\u4e00-\u9fa5]+，替换掉中文字符类似表达式为 [\u4e00-\u9fa5]+

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <boost/locale.hpp>

#include <boost/regex.hpp>
using namespace std;

void read(std::wstring& ret)
{
  std::ifstream file("text_reocord.log");
  std::string str;
  while(file>>str){
    //std::cout<<"ch : "<<str<<std::endl;
    ret.append(boost::locale::conv::utf_to_utf<wchar_t>(str));
  }
}

void write(std::wstring& ret)
{
  std::string str = boost::locale::conv::utf_to_utf<char>(ret);
  std::fstream file("text_reocord_to.log",
    std::fstream::in | std::fstream::out | std::fstream::app);
  file<<str;
  file.close();
}

void fun1()
{
  std::wstring s1 = L"Boost Libraries";
  std::wstring s2 = L"林机 拉风的经 适房fasdf";
  boost::wregex reg(L"^[\u4e00-\u9fa5]+");
  //std::cout << std::boolalpha << boost::regex_match(s1, reg) << '\n';

  boost::wregex reg1(L"[^\u4e00-\u9fa5]+", boost::regex::icase|boost::regex::perl);
  std::wstring s3;
  read(s3);
  std::wstring res = boost::regex_replace(s3, reg1, "$1");
  write(s3);

  int i = 0;
}

int main(int argc, char** argv)
{
  fun1();

  int i = 0;
  return 0;  
}