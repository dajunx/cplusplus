
/**
 * 返回 50000以内的质数
 * 质数概念: 在大于1的自然数中，除了1和它本身以外不再有其他因数
 */
#include <iostream>
#include <set>

// 最简易的算法，效率待提高，用作校验其他算法使用;
std::set<int> get_prime_numbers_version1() {
  std::set<int> set_prime_numbers;
  set_prime_numbers.insert(2);

  for (int i=3;i<50000;++i)
  {
    int temp = 2;
    while (temp < i)
    {
      if (i % temp == 0) {
        // 找到了能被1和i整除的数，则为合数；
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