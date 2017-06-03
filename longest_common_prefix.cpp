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

int main()
{
  vector<string> vec_str;
//   vec_str.push_back("-ab");
//   vec_str.push_back("abc");
//   vec_str.push_back("abcd");
  longestCommonPrefix(vec_str);

  return 0;
}