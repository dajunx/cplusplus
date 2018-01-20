#include <iostream>
#include <vector>

//冒泡排序;
void BubbleSort(int* inputArray, int arrSize)
{
  // 排序规则是从小到大排列;
  int pos1 = 0, pos2 = 0;
  int dataTemp = 0;
  if (inputArray == NULL) {return;}

  for (;pos1 < arrSize; pos1++)
  {
    pos2 = pos1 + 1;
    for (;pos2 < arrSize; pos2++)
    {
      if (inputArray[pos1] > inputArray[pos2]) {
        dataTemp = inputArray[pos1];
        inputArray[pos1] = inputArray[pos2];
        inputArray[pos2] = dataTemp;
      }
    }
  }
}

//选择排序，从小到大排序，寻找最大值方式进行排序;
// 算法：每次循环选择一个最大值，放在数组末端;
void SelectionSort(int* inputArray, int arrSize)
{
  if (inputArray == NULL) {return;}

  int comparedPos = 0, lastNotSortedPos = 0, maxDataPos = 0;
  while(arrSize > 1)
  {
    maxDataPos = 0;
    comparedPos = 1;
    for (;comparedPos < arrSize; comparedPos++)
    {
      if (inputArray[maxDataPos] < inputArray[comparedPos]) {
        maxDataPos = comparedPos;
      }
    }

    lastNotSortedPos = --comparedPos;
    if (lastNotSortedPos != maxDataPos) {
      std::swap(inputArray[maxDataPos], inputArray[lastNotSortedPos]);
    }
    arrSize--;
  }
}

//插入排序，从小到大;
// 在已排序序列中插入元素，数组不适合，得自己移动数据，暂时使用 两个 vector容器来实现;
// 算法：遍历old数组，把其中元素插入有序集合中并保证其有序
/// TODO list容器貌似也可以
void InsertionSort(std::vector<int>& vec_int, int arrSize)
{
  if (vec_int.size() <= 1) {return;}

  std::vector<int> targetVec;
  targetVec.push_back(vec_int.at(0));

  std::vector<int>::iterator posBeCompared;
  std::vector<int>::iterator pos = vec_int.begin();

  while(++pos != vec_int.end())
  {
    bool posMiddleBeCompared = false;
    posBeCompared = targetVec.begin();
    for ( ;posBeCompared != targetVec.end(); ++posBeCompared)
    {
      if(*posBeCompared > *pos)
      {
        targetVec.insert(posBeCompared, *pos);
        posMiddleBeCompared = true;
        break;
      }
    }

    if (false == posMiddleBeCompared) {
      targetVec.insert(targetVec.end(), *pos);
    }
  }
}

int main()
{
  int a1[] = {5, 1, 4, 2, 8};
  BubbleSort(a1, sizeof(a1)/sizeof(int));

  int a2[] = {5, 1, 4, 2, 8};
  SelectionSort(a2, sizeof(a2)/sizeof(int));

  int a3[] = {5, 1, 4, 2, 8};
  std::vector<int> vec_int3(a3, a3 + sizeof(a3)/sizeof(int));
  InsertionSort(vec_int3, sizeof(a3)/sizeof(int));

  return 0;
}