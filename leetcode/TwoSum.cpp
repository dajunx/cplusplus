// 1. Two Sum
/*
Given an array of integers, return indices of the two numbers such that they add up to a specific target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
(快速定位整型数组中某两个值相加等于一个给定值，有且只有有一个正解 且数组元素不能重复使用)
Example:
Given nums = [2, 7, 11, 15], target = 9,
 Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

//TODO 待优化 <对于vector 数组排序>
void quick_sort(vector<int>& s, int l, int r)
{
	if (l < r)
	{
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
				j--;  
			if(i < j) 
				s[i++] = s[j];

			while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
				i++;  
			if(i < j) 
				s[j--] = s[i];
		}
		s[i] = x;
		quick_sort(s, l, i - 1); // 递归调用 
		quick_sort(s, i + 1, r);
	}
}

// 增加了对 target不合法校验
// 算法：对原始数组进行排序，从有序数组两端相加对比target进行目标查找，瓶颈在排序过程中
// time cost:53ms.
vector<int> twoSum1(vector<int>& nums, int target)
{
	vector<int> return_vec;
	vector<int> temp = nums;
	quick_sort(temp,0,temp.size()-1);

	int nums_size = (int)nums.size();
	int i = 0,j = nums_size-1;
  bool bFinded = false;
	for (;i < nums_size -1 && j > 0 && (i != nums_size || j != nums_size);)
	{
		if(temp.at(i) + temp.at(j) == target)
		{
			i = temp.at(i);
			j = temp.at(j);
      bFinded = true;
			break;
		}
		else if(temp.at(i) + temp.at(j) < target)
			i++;
		else
			j--;
	}

  if (bFinded) {
    // 在排序后的数组中找到 a + b = c 模式中 a和b，在源数组中找出他们的序号
    int pos = 0;
    for(vector<int>::iterator it = nums.begin();it != nums.end(); ++ it,++pos)
    {
      if(i == *it || j == *it)
        return_vec.push_back(pos);
      if ( 2 == return_vec.size()) break;
    }
  }
	
	return return_vec;
}

/*算法2，对无序数组2个2个分批相加和 target进行对比是否相等，相等结束，
        不相等则插入map中，用于校验下一组元素是否和上一组元素可以组合满足条件
time cost:13ms.
*/
vector<int> twoSum(vector<int>& nums, int target)
{
  unordered_map<int, int> unmap_other;
  unordered_map<int, int>::iterator un_it;
  vector<int> vec_ret;
  int i = 0, j = i + 1;
  int vec_size = nums.size();

  for (;i < vec_size || j < vec_size;)
  {
    if (unmap_other.size() != 0) {
      un_it = unmap_other.find(nums.at(i));
      if (un_it != unmap_other.end()) {
        vec_ret.push_back(un_it->second);
        vec_ret.push_back(i);
        break;
      }

      un_it = unmap_other.find(nums.at(j));
      if (un_it != unmap_other.end()) {
        vec_ret.push_back(un_it->second);
        vec_ret.push_back(j);
        break;
      }
      
    }
    
    if (nums.at(i) + nums.at(j) == target) {
      vec_ret.push_back(i);
      vec_ret.push_back(j);
      break;
    } else {
//       unmap_other.insert(std::make_pair<int, int>(target - nums.at(i), i));
//       unmap_other.insert(std::make_pair<int, int>(target - nums.at(j), j));
      //leetcode 编译提示 make_pair不存在，使用 {} 方式向unordered_map插入元素
      unmap_other.insert({target - nums.at(i), i});
      unmap_other.insert({target - nums.at(j), j});
      i += 2;
      j = i + 1;
      if(j >= vec_size) j = i; // 已经到达数组末端
    }
  }

  return vec_ret;
}

int main()
{
  int a[] = {3, 2, 4};
	vector<int> vec_input(a, a + sizeof(a)/sizeof(int)), vec_ret;
	int target = 0; // 0 会造成程序core
	vec_ret = twoSum(vec_input, target);

	return 0;
}
