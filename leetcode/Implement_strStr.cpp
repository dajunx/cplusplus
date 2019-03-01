/*
1.https://leetcode.com/problems/implement-strstr/
  实现 c 字符串 strstr函数
2.数据源分析：
  a.父字符串 非空 ，子字符串空 返回0
  b.父字符串 非空， 子字符串非空 ==> 处理
  c.父字符串 空， 子字符串空  返回0
  d.父字符串 空， 子字符串非空 返回-1
3.当前的思路：
  遍历父字符串，
  a、步骤一：当发现父字符串当前字符和子字符串第一个字符相同，进入步骤二
  b、步骤二：从父字符串当前位置开始和子字符串对比，若发现不同的字符串，进入步骤三
  c、步骤三：若父字符串走到结尾，结束；否则，父字符串向后移动一位，进入步骤一
*/
#include <iostream>
#include <string>
using namespace std;

class Solution {
public:
  int strStr(string haystack, string needle) {
    int ret = -1;    
    do
    {
      if (needle.empty()) {
        ret = 0;
        break;
      }

      int haystack_curr_pos = 0, haystack_compare_pos = 0;
      int needle_pos = 0;

      while (haystack_curr_pos < haystack.size() && needle_pos < needle.size()) {

        // 父子字符串 首字母比对
        if (haystack[haystack_curr_pos] != needle[needle_pos]) {
          haystack_curr_pos++;
          continue;
        }

        // 循环needle 进行父子字符串 完整比对
        haystack_compare_pos = haystack_curr_pos;
        while (haystack_compare_pos < haystack.size() && needle_pos < needle.size()) {
          if (haystack[haystack_compare_pos] != needle[needle_pos]) {
            break;
          }
          haystack_compare_pos++;
          needle_pos++;
        }

        // 父子字符串是否能比对上？
        if (needle_pos == needle.size()) {
          ret = haystack_curr_pos;
          break;
        }

        needle_pos = 0;
        haystack_curr_pos++;
      }

    } while (false);
    
    return ret;
  }
};

int main() {
  // mississippi - issip.
  Solution slu;
  string haystack("mississippi"), needle("issip");
  
  std::cout<<"result: "<<slu.strStr(haystack, needle)<<std::endl;

  return 0;
}