/*
  快乐的数字：
  定义：写一个算法来判断一个数是不是"快乐数"。
    一个数是不是快乐是这么定义的：对于一个正整数，每一次将该数替换为他每个位置上的数字的平方和，然后重复这个过程直到这个数变为1，或是无限循环但始终变不到1。如果可以变为1，那么这个数就是快乐数。
  eg:
  19 就是一个快乐数。

  1^2 + 9^2 = 82
  8^2 + 2^2 = 68
  6^2 + 8^2 = 100
  1^2 + 0^2 + 0^2 = 1
*/
#include <iostream>     // std::cout
#include <algorithm>    // std::all_of
#include <array>        // std::array
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>

std::stringstream numberLists;

// 无排序的set 为啥插入1 放在头？
bool sum(int inputNumber) {
  numberLists.str("");
  numberLists << inputNumber << ":";
  bool bSucc = false;
  std::unordered_set<int> setSum;

  if (inputNumber < 1) {
    return false;
  }

  while(1) {
    int SumOfSeparateNum = 0;
    while (inputNumber != 0)
    {
      SumOfSeparateNum += (inputNumber%10) * (inputNumber%10);
      inputNumber = inputNumber/10;
    }
    
    numberLists << SumOfSeparateNum;
    if (SumOfSeparateNum == 1) {
      bSucc = true;
      break;
    }

    if (setSum.find(SumOfSeparateNum) != setSum.end()) {
      break;
    }

    numberLists << ", ";
    setSum.insert(SumOfSeparateNum);
    inputNumber = SumOfSeparateNum;
  }
  numberLists << std::endl;

  if (!bSucc) {
    return false;
  }

  return true;
}

void write(const std::string filename, const std::string& inputContent) {
  std::fstream file(filename,
    std::fstream::in | std::fstream::out | std::fstream::app);
  file <<inputContent;
  file.close();
}

int main ()
{
  for (int i=1;i<10000;++i)
  {
    bool ret = sum(i);
    if (ret) {
      write("resultSucc.txt", numberLists.str());
    } else {
      write("resultfaild.txt", numberLists.str());
    }
  }

  return 0;
}