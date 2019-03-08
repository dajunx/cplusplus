/*
1.https://leetcode.com/problems/implement-strstr/
  ʵ�� c �ַ��� strstr����
2.����Դ������
  a.���ַ��� �ǿ� �����ַ����� ����0
  b.���ַ��� �ǿգ� ���ַ����ǿ� ==> ����
  c.���ַ��� �գ� ���ַ�����  ����0
  d.���ַ��� �գ� ���ַ����ǿ� ����-1
3.��ǰ��˼·��
  �������ַ�����
  a������һ�������ָ��ַ�����ǰ�ַ������ַ�����һ���ַ���ͬ�����벽���
  b����������Ӹ��ַ�����ǰλ�ÿ�ʼ�����ַ����Աȣ������ֲ�ͬ���ַ��������벽����
  c���������������ַ����ߵ���β�����������򣬸��ַ�������ƶ�һλ�����벽��һ
*/
#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>
#include <windows.h> // ����ʹ�� ������ɫ
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

        // �����ַ��� ����ĸ�ȶ�
        if (haystack[haystack_curr_pos] != needle[needle_pos]) {
          haystack_curr_pos++;
          continue;
        }

        // ѭ��needle ���и����ַ��� �����ȶ�
        haystack_compare_pos = haystack_curr_pos;
        while (haystack_compare_pos < haystack.size() && needle_pos < needle.size()) {
          if (haystack[haystack_compare_pos] != needle[needle_pos]) {
            break;
          }
          haystack_compare_pos++;
          needle_pos++;
        }

        // �����ַ����Ƿ��ܱȶ��ϣ�
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

// �����㷨 + ������
void impl_test_fun(Solution & slu, std::string const& haystack, std::string const& needle, bool expect_res) {
  bool ret = slu.strStr(haystack, needle) != -1 ? true : false;
  int ret_color_num = ret == expect_res? 10:12;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  std::stringstream sstr_haystack, sstr_needle, sstr_expect_res, sstr_ret;
  sstr_haystack << "\"" << (haystack.empty()?"":haystack) << "\"";
  sstr_needle << "\"" << (needle.empty()?"":needle) << "\"";
  sstr_expect_res << (expect_res?"true":"false");
  sstr_ret << (ret == expect_res?"true":"false");

  std::cout<<"[test] compre<"<<sstr_haystack.str()<<", "<<sstr_needle.str()<<">, is "<<sstr_expect_res.str();

  // ����У������ɫ��ͨ�� ��ɫ��ʾ�� ��ͨ�� ��ɫ��ʾ
  SetConsoleTextAttribute(hConsole, ret_color_num); // ��success ����
  std::cout<<" ["<<sstr_ret.str()<<"]."<<std::endl;
  SetConsoleTextAttribute(hConsole, 7);  // �ָ�������ɫ
}

// �Զ���������������ڲ����㷨
void self_test_cases() {
  Solution slu;
  /** ��������
   * imple_test_fun ��������:
   *   1.�ڶ�������Ϊ ���ַ��� haystack
   *   2.����������Ϊ ���ַ��� needle
   *   3.���ĸ�����Ϊ �������㷨���
   */
  impl_test_fun(slu, "mississippi", "issip", true); // ���ַ���Ҫ�Ա����β����ҵ�
  impl_test_fun(slu, "missi", "ss", true);
  impl_test_fun(slu, "missi", "ssa", false);
  impl_test_fun(slu, "a", "a", true); // ��ȫƥ��
  impl_test_fun(slu, "missi", "mi", true); // �׶�ƥ��
  impl_test_fun(slu, "missi", "si", true); // β�˶�ƥ��
  impl_test_fun(slu, "mi", "missi", false); // ���ַ���������case

  // �������� case
  impl_test_fun(slu, "", "ss", false);
  impl_test_fun(slu, "missi", "", true);
  impl_test_fun(slu, "", "", true);

  // ��������
  //impl_test_fun(slu, "missi", "si", false);
}

int main() {
  self_test_cases();

  return 0;
}