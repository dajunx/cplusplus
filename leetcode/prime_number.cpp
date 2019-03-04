
/**
 * ���� 50000���ڵ�����
 * ��������: �ڴ���1����Ȼ���У�����1�����������ⲻ������������
 * �㷨�ο�����:
 *   1.https://blog.csdn.net/wdqfzy/article/details/7034784
 *   2.https://blog.csdn.net/qq_36221862/article/details/74163632
 */
#include <iostream>
#include <list>

// ����׵��㷨��Ч�ʴ���ߣ�����У�������㷨ʹ��;
void get_prime_numbers_ver1(std::list<int>& list_prime_numbers) {
  list_prime_numbers.insert(list_prime_numbers.end(), 2);
  int calculate_while_loop_count = 0; // for test.

  for (int i=3;i<50000;++i)
  {
    int temp = 2;
    while (temp < i)
    {
      calculate_while_loop_count++; // for test, ��������while����һ�ڴ�
      if (i % temp == 0) {
        // �ҵ����ܱ�1��i������������Ϊ������
        break;
      }
      temp++;
    }

    if (temp == i) {
      list_prime_numbers.insert(list_prime_numbers.end(), i);
    }
  }
}

// ��΢��һ����Գ���
void get_prime_numbers_ver2(std::list<int>& list_prime_numbers) {
  list_prime_numbers.insert(list_prime_numbers.end(), 2);
  list_prime_numbers.insert(list_prime_numbers.end(), 3);
  int calculate_while_loop_count = 0; // for test.

  for (int i=4;i<50000;++i)
  {
    int temp = 2;
    bool prime_number = true;
    while (temp * temp < i+1)
    {
      calculate_while_loop_count++; // for test, ��������while����һ�ٶ����
      if (i % temp == 0) {
        // �ҵ����ܱ�1��i������������Ϊ������
        prime_number = false;
        break;
      }
      temp++;
    }

    if (prime_number && temp > 2) {
      list_prime_numbers.insert(list_prime_numbers.end(), i);
    }
  }
}

// �Կռ任ʱ��
// ˼·����Ҫ���Ե������ֱ��ǣ�3��5��7��9�������㷢��û�У���9�ĳ����Ƕ���ġ����ܱ�3��������Ȼ���ܱ�9����
void get_prime_numbers_ver3(std::list<int>& list_prime_numbers) {
  std::list<int>::iterator it_temp;
  list_prime_numbers.insert(list_prime_numbers.end(), 2);
  list_prime_numbers.insert(list_prime_numbers.end(), 3);
  int calculate_while_loop_count = 0; // for test.

  for (int i=4;i<50000;++i)
  {
    it_temp = list_prime_numbers.begin();
    int temp = *it_temp;
    bool prime_number = true;
    while (temp * temp < i+1 && it_temp != list_prime_numbers.end())
    {
      calculate_while_loop_count++; // for test, ��������while������ʮ���
      if (i % temp == 0) {
        // �ҵ����ܱ�1��i������������Ϊ������
        prime_number = false;
        break;
      }
      ++it_temp;
      temp = *it_temp;
    }

    if (prime_number && temp > 2) {
      list_prime_numbers.insert(list_prime_numbers.end(), i);
    }
  }
}

// �Ա������������ݣ�������������㷨��ȷ��
bool compare_two_list(std::list<int> list1, std::list<int> list2) {
  bool ret = false;
  int pos = 1;

  do 
  {
    if (list1.size() != list2.size()) {
      break;
    }
    std::list<int>::iterator list_it1 = list1.begin(), list_it2 = list2.begin();
    
    while(list_it1 != list1.end() && list_it2 != list2.end()) {
      if (*list_it1 != *list_it2) {
        break;
      }
      list_it1++;
      list_it2++;
      pos++;
    }

    if (list_it1 == list1.end()) {
      ret = true;
    }
  } while (false);  

  return ret;
}

int main() {
  std::list<int> ret_ver1, ret_ver2, ret_ver3;
  get_prime_numbers_ver1(ret_ver1);
  get_prime_numbers_ver2(ret_ver2);
  get_prime_numbers_ver3(ret_ver3);
  
  compare_two_list(ret_ver1, ret_ver3);

  return 0;
}