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
//特暴力原始方式 (n2时间复杂度)
vector<vector<int>> threeSum(vector<int>& nums) 
{
  vector<vector<int>> vec_ret;
  vector<int>::iterator it;
  std::multimap<int, int> mul_map_content;
  std::multimap<int, int>::iterator it_mul_map1,it_mul_map2, it_mul_find;
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

          std::cout<<"data:["<<it_mul_map1->first
            <<", "<<it_mul_map2->first
            <<"]"
            <<" value:"<<mul_map_content.find(0 - value_tmp)->first<<std::endl;
          std::cout<<"pos :["<<it_mul_map1->second
            <<", "<<it_mul_map2->second
            <<"]"
            <<" value:"<<mul_map_content.find(0 - value_tmp)->second<<std::endl;
          std::cout<<std::endl;
      }
    }
  }

  return vec_ret;
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

void multimap_view(std::vector<int>& vec_input)
{
  std::multimap<int, int> mul_map;
  std::multimap<int, int>::iterator it_mul_map;
  std::vector<int>::iterator it_vec;
  for (it_vec = vec_input.begin(); it_vec != vec_input.end();++it_vec)
  {
    mul_map.insert(std::pair<int, int>(*it_vec,0));
  }

  for (it_mul_map = mul_map.begin();it_mul_map != mul_map.end();++it_mul_map)
  {
    std::cout<<"data: "<<it_mul_map->first<<std::endl;
  }
}

int main()
{
  three_compare();

  //int a[] = {-1, 0, 1, 2, -1, -4};
  int a[] = {-1,0,1,2,-1,-4};
  vector<int> vec_input(a, a + sizeof(a)/sizeof(int));
  threeSum(vec_input);

  return 0;
}