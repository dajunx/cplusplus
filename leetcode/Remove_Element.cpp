//Remove Element
/*
Given an array and a value, remove all instances of that value in place and return the new length.

Do not allocate extra space for another array, you must do this in place with constant memory.

The order of elements can be changed. It doesn't matter what you leave beyond the new length.

Example:
Given input array nums = [3,2,2,3], val = 3

Your function should return length = 2, with the first two elements of nums being 2.
*/
//尽量不使用 vector 迭代器，加快函数效率
#include <iostream>
#include <vector>
using namespace std;

//使用 pos 而不是 iterator
int removeElement(vector<int>& nums, int val) 
{
  int new_elements_pos = 0, old_elements_pos = 0;
  int old_nums_size = nums.size();

  while (old_elements_pos < old_nums_size)
  {
    if(nums[old_elements_pos] == val) {
      old_elements_pos++;
      continue;
    }

    nums[new_elements_pos++] = nums[old_elements_pos++];
  }  

  nums.resize(new_elements_pos);
  return nums.size();
}

int main ()
{
  int a[] = {3, 2, 1, 3};
  //int a[] = {3, 3, 3, 3};
  //vector<int> vec_test(a, a + sizeof(a)/sizeof(int));
  vector<int> vec_test;
  removeElement(vec_test, 3);

  return 0;
}