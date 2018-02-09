#include <iostream>
#include "boost/generator_iterator.hpp"
#include "boost/random.hpp"
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <vector>
using namespace std;

void test_fun1() {
  std::vector<int> vec_int;
  vec_int.push_back(0);
  vec_int.push_back(0);
  vec_int.push_back(0);
  vec_int.push_back(0);
  vec_int.push_back(0);
  vec_int.push_back(0);

  int num = 0;
  typedef boost::mt19937 RNGType;
  RNGType rng;
  boost::uniform_int<> one_to_six(0, 5);
  boost::variate_generator<RNGType, boost::uniform_int<>> dice(rng, one_to_six);
  for (int i = 0; i < 1000000; ++i) {
    num = dice();
    vec_int[num]++;
  }
}

int get_random_num() {
  typedef boost::mt19937 RNGType;
  RNGType rng(time(0));
  boost::uniform_int<> one_to_six(0, 5);
  boost::variate_generator<RNGType, boost::uniform_int<>> dice(rng, one_to_six);
  return dice();
}

class test1 {
public:
  test1() { srand(static_cast<unsigned int>(time(NULL))); }
  static int test_fun3() {
    int iSecret = rand() % 5 + 1;
    return iSecret;
  }
  static int iSecret;
};
int test1::iSecret(0);

void test_fun2() {
  std::vector<int> vec_int;
  for (int i = 0; i < 6; ++i) {
    vec_int.push_back(0);
  }

  int num = 0;
  for (int i = 0; i < 10000; ++i) {
    num = test1::test_fun3();
    vec_int[num]++;
  }
}

int main() {
  test_fun2();
  return 0;
}