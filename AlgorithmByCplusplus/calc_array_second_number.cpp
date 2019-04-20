/*
** 本代码目的：计算数组中第二大的数字;
*/
#include <iostream>

// 返回数组中第二大的数字
int get_second_number(int arr[], int arr_size)
{
  if (arr_size == 1) return -1;
  int first = arr[0] >= arr[1]? arr[0]:arr[1];
  int second = arr[0] < arr[1]? arr[0]:arr[1];
  int arr_pos = 2;

  while (arr_pos < arr_size) {
    if (arr[arr_pos] > first) {
      second = first;
      first = arr[arr_pos];
    } else if (arr[arr_pos] > second) {
      second = arr[arr_pos];
    } else {
      //
    }

    arr_pos++;
  }

  return second;
}

int main() 
{
  int ten_nums[10] = {2, 3, 5, 7, 1, 0, 9, 4, 6, 8};
  int second_number = get_second_number(ten_nums, 10);
  std::cout<<"second number: " << second_number<<std::endl;

  return 0;
}