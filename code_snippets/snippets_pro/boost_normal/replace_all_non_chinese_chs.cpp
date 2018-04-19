//替换掉内容中所有非中文字符，正则表达式为
//[^\u4e00-\u9fa5]+，替换掉中文字符类似表达式为 [\u4e00-\u9fa5]+
//keep_only_english_and_number 保留英文和数组，参照如下连接 的 unicode码
//https://unicode-table.com/cn/#005A  基本拉丁字母

#include <boost/locale.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>

#include <boost/regex.hpp>
using namespace std;

void read(std::wstring &ret) {
  std::ifstream file("text_reocord.log");
  std::string str;
  while (file >> str) {
    // std::cout<<"ch : "<<str<<std::endl;
    ret.append(boost::locale::conv::utf_to_utf<wchar_t>(str));
  }
}

void write(std::wstring &ret) {
  std::string str = boost::locale::conv::utf_to_utf<char>(ret);
  std::fstream file("text_reocord_to.log",
                    std::fstream::in | std::fstream::out | std::fstream::app);
  file << str;
  file.close();
}

void fun2() {
  std::ifstream fin("C:\\Users\\Administrator\\Desktop\\hole_.log");
  std::fstream out("C:\\Users\\Administrator\\Desktop\\hole_to.log",
                   std::fstream::in | std::fstream::out | std::fstream::app);
  std::string s;
  std::wstring ws;
  boost::wregex reg1(L"[^\u4e00-\u9fa5]+",
                     boost::regex::icase | boost::regex::perl);
  while (getline(fin, s)) {
    ws = boost::locale::conv::utf_to_utf<wchar_t>(s);
    ws = boost::regex_replace(ws, reg1, "$1");
    out << boost::locale::conv::utf_to_utf<char>(ws) << std::endl;
  }

  fin.close();
  out.close();
}

void fun1() {
  std::wstring s1 = L"Boost Libraries";
  std::wstring s2 = L"林机 拉风的经 适房fasdf";
  boost::wregex reg(L"^[\u4e00-\u9fa5]+");
  // std::cout << std::boolalpha << boost::regex_match(s1, reg) << '\n';

  boost::wregex reg1(L"[^\u4e00-\u9fa5]+",
                     boost::regex::icase | boost::regex::perl);
  std::wstring s3;
  read(s3);
  std::wstring res = boost::regex_replace(s3, reg1, "$1");
  write(s3);

  int i = 0;
}

void keep_only_english_and_number()
{
  std::wstring s1 = L"Boost Libraries的经32415&*)(^)";
  std::wstring s2 = L"林机 拉风的经 适房fasdf";

  boost::wregex reg1(L"[^\x30-\x39\x41-\x5A\x61-\x7A]+",
    boost::regex::icase | boost::regex::perl);
  std::wstring res;
  res = boost::regex_replace(s1, reg1, "$1");
  res = boost::regex_replace(s2, reg1, "$1");
}

int main(int argc, char **argv) {
  fun1();

  int i = 0;
  return 0;
}