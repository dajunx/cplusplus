#include <iostream>
#include <vector>
#include <string>
using namespace std;

string longestCommonPrefix(vector<string>& strs)
{
  //算法：双重循环do_while + for 遍历字符串数组中所有字符，效率较低
  string str_ret;
  int vec_size = strs.size();
  int pos = 0;
  while (vec_size > 0) //case strs为空
  {
    char c_tmp;
    int i = 0;
    for (;i < vec_size;++i)
    {
      //遍历到字符串数组中某个字符末尾
      if (pos >= static_cast<int>(strs[i].length())) {
        break;
      }

      //查找字符串数组中相同pos上不同的字符
      if ( i != 0 && strs[i].at(pos) != c_tmp) {
        break;
      }
      c_tmp = strs[i].at(pos);
    }
    if (i != vec_size) {break;} //字符串数组pos上字符不一致
    str_ret.append(1, c_tmp);
    pos ++;
  }

  return str_ret;
}

//总的算法：
/*
** 1、vector空？ 2、vector只有1个元素？ 
** 3、对比逻辑：a、两两字符串找不到公共前缀 --> return 空串
**            b、提取最大字符前缀 tmp，把tmp与vector<string>中下一个 进行对比
**            c、类推
*/
string longestCommonPrefix2(vector<string>& strs)
{
  //算法：每次去字符串数组中2个字符串最大前缀，然后拿这个前缀再去和下一个字符串对比，类推
  string equal_str;
  if (strs.size() == 0) { return equal_str;}

  int strs_size = strs.size();
  int pos = 0, i = 1, mix_pos = 0;
  equal_str = strs[0];
  while (i < strs_size)
  {
    pos = 0;
    mix_pos = equal_str.length() < strs[i].length() 
              ? equal_str.length() : strs[i].length();
    while(pos < mix_pos && equal_str.at(pos) == strs[i].at(pos)) {
      pos++;
    }

    ///TODO string在swap函数中构造 linux端编译会提示如下error
    /*
    **error: cannot bind non-const lvalue reference of type 'std::__cxx11::basic_string<char>&' 
    ** to an rvalue of type 'std::__cxx11::string {aka std::__cxx11::basic_string<char>}'
    */
    string tmp(equal_str, 0, pos);
    equal_str.swap(tmp);
    //若两个字符串没有相同项，立马返回空前缀
    if (equal_str.empty()) {
      break;
    }
    i++;
  }

  return equal_str;
}

int main()
{
  vector<string> vec_str;
  vec_str.push_back("c");
//   vec_str.push_back("acc");
//   vec_str.push_back("ccc");
  longestCommonPrefix2(vec_str);

  return 0;
}