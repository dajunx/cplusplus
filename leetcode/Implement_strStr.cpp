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
#include <sstream>
#include <assert.h>
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

// 运行算法 + 输出结果
void impl_test_fun(Solution & slu, std::string const& haystack, std::string const& needle, bool expect_res) {
  bool ret = slu.strStr(haystack, needle) != -1 ? true : false;
  if (ret != expect_res) {
    assert(false);
  } else {
    std::stringstream sstr_haystack, sstr_needle, sstr_expect_res;
    sstr_haystack << "\"" << (haystack.empty()?"":haystack) << "\"";
    sstr_needle << "\"" << (needle.empty()?"":needle) << "\"";
    sstr_expect_res << (expect_res?"true":"false");

    std::cout<<"[test] compre<"<<sstr_haystack.str()<<", "<<sstr_needle.str()<<">, is "<<sstr_expect_res.str()<<" success."<<std::endl;
  }
}

// 自定义测试用例，用于测试算法
void self_test_case() {
  Solution slu;
  /** 测试用例
   * imple_test_fun 参数解释:
   *   1.第二个参数为 父字符串 haystack
   *   2.第三个参数为 子字符串 needle
   *   3.第四个参数为 期望的算法结果
   */
  impl_test_fun(slu, "mississippi", "issip", true);
  impl_test_fun(slu, "missi", "ss", true);

  // 三种特殊 case
  impl_test_fun(slu, "", "ss", false);
  impl_test_fun(slu, "missi", "", true);
  impl_test_fun(slu, "", "", true);
}

int main() {
  self_test_case();

  return 0;
}