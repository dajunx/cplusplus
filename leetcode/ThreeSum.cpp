/*3Sum
  Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? 
Find all unique triplets in the array which gives the sum of zero.
Note: The solution set must not contain duplicate triplets.
For example, given array S = [-1, 0, 1, 2, -1, -4],
A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
*/
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <sstream>
using namespace std;

struct comparer
{
public:
  bool operator()(const std::string a, const std::string b)
  {
    if (a.size() < b.size())
      return true;
    if (a.size() == b.size() && a.compare(b) < 0)
      return true;
    return false;
  }
};

//a + b + c = 0?
//特暴力原始方式
vector<vector<int> > threeSum1(vector<int>& nums) 
{
  vector<vector<int> > vec_ret;
  vector<int>::iterator it;
  std::unordered_multimap<int, int> mul_map_content;
  std::unordered_multimap<int, int>::iterator it_mul_map1,it_mul_map2, it_mul_find;
  int pos = 0;
  for (it = nums.begin(); it != nums.end(); ++it)
  {
    mul_map_content.insert(std::pair<int, int>(*it, pos++));
  }

  it_mul_map1 = mul_map_content.begin();
  int value_tmp = 0;
  vector<int> sub_vec;
  std::multiset<std::string, comparer> mul_set_compare;
  std::stringstream tmp;
  for (; it_mul_map1 != mul_map_content.end(); ++it_mul_map1)
  {
    it_mul_map2 = it_mul_map1;
    it_mul_map2++;
    for (; it_mul_map2 != mul_map_content.end(); ++it_mul_map2)
    {
      value_tmp = it_mul_map1->first + it_mul_map2->first;
      it_mul_find = mul_map_content.find(0 - value_tmp);
      if (it_mul_find != mul_map_content.end() 
        && it_mul_find->second != it_mul_map1->second
        && it_mul_find->second != it_mul_map2->second) {
          sub_vec.push_back(it_mul_map1->first);
          sub_vec.push_back(it_mul_map2->first);
          sub_vec.push_back(it_mul_find->first);
          std::sort(sub_vec.begin(), sub_vec.end());
          tmp<<sub_vec[0];
          tmp<<sub_vec[1];
          tmp<<sub_vec[2];
          if (mul_set_compare.find(tmp.str()) == mul_set_compare.end()) {
            mul_set_compare.insert(mul_set_compare.end(), tmp.str());
            vec_ret.push_back(sub_vec); //添加数据
          }
          tmp.str("");
          sub_vec.clear();
      }
    }
  }

  return vec_ret;
}

/*
算法大概：
1、首先把输入数组分成大于0和小于0两堆并分别排序;( map<abs(item), item> )
2、从左侧小于0的堆中从左往右依次取出两个元素，与大于0的堆中元素进行对比；
3、反之亦然，从大于0的堆中取两个元素与小于0的堆中元素进行对比；
*/
vector<vector<int> > threeSum(vector<int>& nums)
{
  vector<vector<int> > vv_ret;
  std::multimap<int, int> mul_left,mul_right;
  std::multimap<int, int>::iterator it_left1, it_left2, it_right1, it_right2;

  vector<int> sub_vec;
  //拆分待检测数组为2个阵营，实现 a+b=c策略
  for (vector<int>::iterator it_v = nums.begin(); it_v != nums.end(); ++it_v)
  {
    if (*it_v >= 0) {
      mul_right.insert(std::pair<int, int>(*it_v, *it_v));
    } else{
      mul_left.insert(std::pair<int, int>(-*it_v, *it_v));
    }
  }

  // 0 + 0 + 0 = 0 特case
  if (mul_right.count(0) >= 3) {
    sub_vec.push_back(0);
    sub_vec.push_back(0);
    sub_vec.push_back(0);
    vv_ret.push_back(sub_vec);
    sub_vec.clear();
  }

  std::multiset<std::string, comparer> mul_set_compare;
  std::stringstream tmp;
  int i = 2;
  while (i--)
  {
    it_right2 = mul_right.begin();
    for (it_left1 = mul_left.begin(); it_left1 != mul_left.end() && it_right2 != mul_right.end(); ++it_left1)
    {
      bool b_left_two_change = true;
      it_right1 = it_right2;
      it_left2 = it_left1;

      for (it_left2++; it_left2 != mul_left.end() && it_right1 != mul_right.end();)
      {
        int ab_value = it_left1->first + it_left2->first;

        if(ab_value < it_right1->first) { // a + b < c，提高b值
          //std::cout<<"step < left1: "<<it_left1->first<<", left2: "<<it_left2->first<<", right: "<<it_right1->first<<std::endl;
          b_left_two_change = false;
          ++it_left2;        
        } else if(ab_value > it_right1->first) {// a + b > c,提高c的值
          //std::cout<<"step > left1: "<<it_left1->first<<", left2: "<<it_left2->first<<", right: "<<it_right1->first<<std::endl;
          it_right1++;
          if(b_left_two_change) {it_right2 = it_right1;} //核心，保存内循环中 c相对于a+b最小值
        } else { // a + b = c
          //std::cout<<"step = left1: "<<it_left1->first<<", left2: "<<it_left2->first<<", right: "<<it_right1->first<<std::endl;
          //std::sort(sub_vec.begin(), sub_vec.end());
          sub_vec.push_back(it_left1->second);
          sub_vec.push_back(it_left2->second);
          sub_vec.push_back(it_right1->second);
          tmp<<sub_vec[0];
          tmp<<sub_vec[1];
          tmp<<sub_vec[2];
          if (mul_set_compare.find(tmp.str()) == mul_set_compare.end()) { //排重
            mul_set_compare.insert(mul_set_compare.end(), tmp.str());
            vv_ret.push_back(sub_vec); //添加数据
          }
          sub_vec.clear();
          tmp.str("");
          b_left_two_change = false;
          ++it_left2;
        }
      }
    }
    //tmp.str("");
    std::swap(mul_left, mul_right);// 规则从a+a' = c 切换为 c + c' = a
  }

  return vv_ret;
}

//multiset std::string find ???是否有效果
void three_compare()
{
  int i = 0;
  std::map<std::string, int, comparer> mm;
  mm.insert(std::pair<std::string, int>("lini", 1));
  if(mm.find("ljjj") != mm.end()) {
    i = 1;
  }
  i = 2;
}

void show_vv_items(vector<vector<int> >& vv)
{
  vector<vector<int> >::iterator it_vv = vv.begin();
  vector<int>::iterator it_v;
  std::stringstream out_print_str;

  for (;it_vv != vv.end();++it_vv)
  {
    it_v = (*it_vv).begin();
    out_print_str<<"data: [";
    for (;it_v != (*it_vv).end(); ++it_v)
    {
      out_print_str<<*it_v<<",";
    }
    std::string str = out_print_str.str();
    str.erase(str.end()-1);
    std::cout<<str<<"]"<<std::endl;
    out_print_str.str("");
  }
}

int main()
{
  three_compare();

  //int a[] = {-1, 0, 1, 2, -1, -4};
  //int a[] = {-1,0,1,2,-1,-4};
  //int a[] = {-4,-3,-2,-2,-2,-1,0,1,2,3,5,9};
  //int a[] = {0,0,0};
  //int a[] = {0,0};
  //int a[] = {-1,0,1,0};
  int a[] = {-4,-2,1,-5,-4,-4,4,-2,0,4,0,-2,3,1,-5,0};
  vector<int> vec_input(a, a + sizeof(a)/sizeof(int));
  vector<vector<int> > vv;
  vv = threeSum(vec_input);
  show_vv_items(vv);

  vv.clear();
  std::cout<<"---------------------------------------"<<std::endl;
  vv = threeSum1(vec_input);
  show_vv_items(vv);

  return 0;
}