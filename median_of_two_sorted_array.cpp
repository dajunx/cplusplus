#include <iostream>
#include <vector>
//求两个已排序int vector的median

double find_two_sorted_median(std::vector<int>& nums1, std::vector<int>& nums2)
{
  //先汇总2个vector，再找出中位数,不满足 log(m+n)时间复杂度
  double ret = 0;
  std::vector<int>::iterator it1 = nums1.begin(), it2 = nums2.begin();
  std::vector<int> common_arry;
  for (;it1 != nums1.end() && it2 != nums2.end();)
  {
    *it1 < *it2 
      ? common_arry.push_back(*it1++)
      : common_arry.push_back(*it2++), common_arry.push_back(*it1++);
  }

  for(;it1 != nums1.end();) {
    common_arry.push_back(*it1++);
  }

  for(;it2 != nums2.end();) {
    common_arry.push_back(*it2++);
  }

  int com_size = common_arry.size();
  if (com_size%2 == 0) {
    ret = static_cast<double>(common_arry.at(com_size/2 -1) + common_arry.at(com_size/2))/2;
  } else {
    ret = static_cast<double>(common_arry.at(com_size/2));
  }

  return ret;
}

double find_two_sorted_median1(std::vector<int>& nums1, std::vector<int>& nums2)
{
  double ret = 0;

  int array_total_size = nums1.size() + nums2.size();
  int left_pos = 0, right_pos = 0, passed_num_count = 0;
  //int num1_pos = 0, num2_pos = 0;
  //int left_num = 0, right_num = 0;
  int tmp[2] = {0}, pos = 0;
  std::vector<int>::iterator it1 = nums1.begin(), it2 = nums2.begin();

  //找出中间值
  if (array_total_size%2 == 0) {
    //偶
    left_pos = array_total_size/2 - 1;
    right_pos = 1;
  } else {
    //奇
    left_pos = array_total_size/2;
  }

  bool com_left_arr = true;
  while (1)
  {
    //走到了数组中间位置
    if(left_pos == 0 && right_pos == 0) {
      break;
    }

    if(it1 == nums1.end() && it2 == nums2.end()) {
      ///
      break;
    } else if(it1 == nums1.end() && it2 != nums2.end()) {
      pos == 0 ? left_pos-- : right_pos --;
      tmp[pos] = tmp[pos] < *it2 ? *it2 : tmp[pos];
      it2++;
    } else if (it2 == nums2.end() && it1 != nums1.end()) {
      pos == 0 ? left_pos-- : right_pos --;
      tmp[pos] = tmp[pos] < *it1 ? *it1 : tmp[pos];
      it1++;
    } else if (*it1 <= *it2) {
      pos == 0 ? left_pos-- : right_pos --;
      tmp[pos] = tmp[pos] < *it2 ? *it2 : tmp[pos];
      it1++;
    } else if (*it1 > *it2) {
      pos == 0 ? left_pos-- : right_pos --;
      tmp[pos] = tmp[pos] < *it1 ? *it1 : tmp[pos];
      it2++;
    }

    if(left_pos == 0) {
      pos++;
    }
  }

  tmp[1] == 0 ? ret = tmp[0] : ret = static_cast<double>(tmp[0] + tmp[1])/2;

  return ret;
}

int main()
{
  //int a[] = {1, 2, 3, 5, 8, 9}, b[] = {5, 8, 9};
  int a[] = {1, 2, 3}, b[] = {3};
  std::vector<int> num1(a, a + sizeof(a)/sizeof(int)), num2(b, b+ sizeof(b)/sizeof(int));
  double ret = find_two_sorted_median1(num1, num2);

  return 0;
}