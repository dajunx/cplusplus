
/**
 * ���� 50000���ڵ�����
 * ��������: �ڴ���1����Ȼ���У�����1�����������ⲻ������������
 */
#include <iostream>
#include <set>

// ����׵��㷨��Ч�ʴ���ߣ�����У�������㷨ʹ��;
std::set<int> get_prime_numbers_version1() {
  std::set<int> set_prime_numbers;
  set_prime_numbers.insert(2);

  for (int i=3;i<50000;++i)
  {
    int temp = 2;
    while (temp < i)
    {
      if (i % temp == 0) {
        // �ҵ����ܱ�1��i������������Ϊ������
        break;
      }
      temp++;
    }

    if (temp == i) {
      set_prime_numbers.insert(i);
    }
  }

  return set_prime_numbers;
}

std::set<int> get_prime_numbers_version2() {

}

int main() {
  get_prime_numbers_version1();

  return 0;
}