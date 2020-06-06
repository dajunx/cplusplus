#include <iostream>
#include <map>
#include <utility>
  /*
   0000 0001
   0000 0010
   0000 0011
  */
std::pair<int,int> ten_2_two(const int& key) // 10进制 转换成 2进制表现成10进制数值
{
    int tmp_key = key, cycle = 0;
    int changed_key = tmp_key%2;
    int onenum_count = changed_key; // 返回结果中的字面数字含有1的个数
    for(int j = 10;cycle < 32 && tmp_key > 0;cycle++)
    {   
      tmp_key = tmp_key/2; // 往右移, 用以计算每一位
      changed_key += (tmp_key%2) * j;
      j *= 10; 
      if(1 == tmp_key%2)
        onenum_count++;
    }   
    // std::cout<<" key="<<key<<",changed_key value:"<<changed_key<<std::endl;
    return std::make_pair(changed_key, onenum_count);
}

int count_appear_onenum(const int& N)
{
  int num = 0;
  std::map<int,int> mTemp;// 记录字面只包含1或者0的数字 比如 010,101
  std::map<int,int>::iterator itr;
  for(int i = 1;i <= 255;++i)
  {
    std::pair<int,int> couple_value;
    couple_value = ten_2_two(i);
    mTemp[couple_value.first] = couple_value.second;
  }

  // 计算1~N中数字出现1数字的个数
  for(itr = mTemp.begin(); itr != mTemp.end(); ++itr)
  {
    if(itr->first <= N)
      num += itr->second;
    else
      break;
  }
  return num;
}

int normal_count_appear_onenum(const int& N)
{
  int num = 0;
  for(int i = 1;i <= N; i++)
  {
    int tmp = i;
    bool b_print_apear_onenum_N = false;
    for(;tmp > 0;) 
    {   
      if(tmp%10 == 1)
      {   
        num++;
        if(b_print_apear_onenum_N == false)
        {   
          std::cout<<" the N is "<<i<<std::endl;
          b_print_apear_onenum_N = true;
        }   
      }   
      tmp = tmp/10;
    }   
  }
  return num;
}

int main()
{
  int N = 9999999;
  // int one_count = count_appear_onenum(N);
  // std::cout<<"N is "<<N<<", one_count is "<<one_count<<std::endl;
  int one_count = normal_count_appear_onenum(N);
  std::cout<<"N is "<<N<<", one_count is "<<one_count<<std::endl;
  return 0;
}
