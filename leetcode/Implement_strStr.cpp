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

int main() {
  // mississippi - issip.
  Solution slu;
  string haystack("mississippi"), needle("issip");
  
  std::cout<<"result: "<<slu.strStr(haystack, needle)<<std::endl;

  return 0;
}