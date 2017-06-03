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
      if (pos >= strs[i].length()) {
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

string longestCommonPrefix2(vector<string>& strs)
{
  //算法：每次去字符串数组中2个字符串最大前缀，然后拿这个前缀再去和下一个字符串对比，类推
  string str_ret;
  if (strs.size() == 0) { return str_ret;}

  int strs_size = strs.size();
  int pos = 0, i = 1, mix_pos = 0;
  string equal_str;
  str_ret = strs[0];
  while (i < strs_size)
  {
    pos = 0;
    equal_str.clear();
    mix_pos = str_ret.length() < strs[i].length() 
              ? str_ret.length() : strs[i].length();
    while(pos < mix_pos && str_ret.at(pos) == strs[i].at(pos)) {
      equal_str.append(1, str_ret.at(pos));
      pos++;
    }

    str_ret.swap(equal_str);
    //若两个字符串没有相同项，立马返回空前缀
    if (equal_str.empty()) {
      break;
    }
    i++;
  }

  return str_ret;
}

int main()
{
  vector<string> vec_str;
  //vec_str.push_back("c");
  vec_str.push_back("aa");
  vec_str.push_back("ab");
  longestCommonPrefix2(vec_str);

  return 0;
}