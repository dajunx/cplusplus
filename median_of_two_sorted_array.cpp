#include <iostream>
#include <vector>
//求两个已排序int vector的median

//{1, 2, 3, 5, 8, 9},{5, 8, 9}
//算法(走向)：1-2-3-5 8  9
//                |/ \/ \
//                5   8  9
//题目总的思路是两个排序号的数组，再次排序在一起后，求中位数。
double find_two_sorted_median1(std::vector<int>& nums1, std::vector<int>& nums2)
{
  double ret = 0;

  int array_total_size = nums1.size() + nums2.size();
  int left_pos = 0;
  bool even_number = false;
  int tmp[2] = {0};
  std::vector<int>::iterator it1 = nums1.begin(), it2 = nums2.begin();

  //找出中间值
  do 
  {
    if (array_total_size == 1) {
      nums1.size() == 1 ? ret = nums1.at(0) : ret = nums2.at(0);
      break;
    } else if (array_total_size%2 == 0) {
      //偶
      left_pos = array_total_size/2 + 1;
      even_number = true;
    } else {
      //奇
      left_pos = array_total_size/2 + 2;
    }

    while (left_pos-- > 0) {
      //走到了数组中间位置
      if(it1 == nums1.end() && it2 == nums2.end()) {
        break;
      } else if(it1 == nums1.end() && it2 != nums2.end()) {
        tmp[0] = tmp[1];
        tmp[1] = *it2++;
      } else if (it2 == nums2.end() && it1 != nums1.end()) {
        tmp[0] = tmp[1];
        tmp[1] = *it1++;
      } else if (*it1 <= *it2) {
        tmp[0] = tmp[1];
        tmp[1] = *it1++;
      } else if (*it1 > *it2) {
        tmp[0] = tmp[1];
        tmp[1] = *it2++;
      }
    }

    even_number ? ret = static_cast<double>(tmp[0] + tmp[1])/2 : ret = tmp[0];
  } while (false);

  return ret;
}

int main()
{
  //int a[] = {1, 2, 3, 5, 8, 9}, b[] = {5, 8, 9};
  //int a[] = {1, 2, 3}, b[] = {3};
  //std::vector<int> num1(a, a + sizeof(a)/sizeof(int)), num2(b, b+ sizeof(b)/sizeof(int));
  std::vector<int> num1, num2;
  num2.push_back(1);
  double ret = find_two_sorted_median1(num1, num2);

  return 0;
}