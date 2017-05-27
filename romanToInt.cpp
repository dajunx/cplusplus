#include <iostream>
#include <map>
#include <string>
using namespace std;
/*
** Given a roman numeral, convert it to an integer.
** Input is guaranteed to be within the range from 1 to 3999.
*/
int romanToInt(string s) {
  map<char, int> map_roman_int;
  map_roman_int['I'] = 1;
  map_roman_int['V'] = 5;
  map_roman_int['X'] = 10;
  map_roman_int['L'] = 50;
  map_roman_int['C'] = 100;
  map_roman_int['D'] = 500;
  map_roman_int['M'] = 1000;

  string roman_str("IVXLCDM");

  //2.合理性校验
  ///TODO

  /*3.拆分处理
  ** 399 CCCXCIX 从右往左检索：100 100 1000 10 100 1 10
  **  1、左侧和自己相等，则相加并累计；
  **  2、左侧大于自己则相加并累计；
  **  3、左侧小于自己，则累计值减去左侧；
  */
  int last_ch_num = 0, total_num = 0, ch_num = 0;
  for (string::reverse_iterator it=s.rbegin(); it != s.rend();++it)
  {
    //1.非法字符校验
    if(roman_str.find(*it) == string::npos) {
      //含有非法字符
      return 0;
    }

    ch_num = map_roman_int.find(*it)->second;
    if(ch_num >= last_ch_num) {
      total_num += ch_num;
    } else if (ch_num < last_ch_num) {
      total_num -= ch_num;
    }
    last_ch_num = ch_num;
  }

  return total_num;
}

bool intToRoman(int i, string& roman_str)
{
  //TODO 逻辑有bug，i 为4 的时候 罗马数字并不是IIII
  //待处理的整型数字必须大于0且小于3999
  if (i <= 0 || i > 3999) {
    return false;
  }

  map<int, char> map_roman_int;
  map_roman_int[1]    = 'I';
  map_roman_int[5]    = 'V';
  map_roman_int[10]   = 'X';
  map_roman_int[50]   = 'L';
  map_roman_int[100]  = 'C';
  map_roman_int[500]  = 'D';
  map_roman_int[1000] = 'M';

  roman_str.clear();
  do
  {
    if (i/1000 > 0) {
      i -= 1000;
      roman_str.append(1, map_roman_int[1000]);
    } else if (i/500 > 0) {
      i -= 500;
      roman_str.append(1, map_roman_int[500]);
    } else if (i/100 > 0) {
      i -= 100;
      roman_str.append(1, map_roman_int[100]);
    } else if (i/50 > 0) {
      i -= 50;
      roman_str.append(1, map_roman_int[50]);
    } else if (i/10 > 0) {
      i -= 10;
      roman_str.append(1, map_roman_int[10]);
    } else if (i/5 > 0) {
      i -= 5;
      roman_str.append(1, map_roman_int[5]);
    } else if (i/1 > 0) {
      i -= 1;
      roman_str.append(1, map_roman_int[1]);
    }

  } while (i > 0);
  

  return true;
}

int main()     
{
  string input_romain_str("MCMXCVI");
  //romanToInt(input_romain_str);
  for (int i = 1; i< 4000;++i)
  {
    if (true == intToRoman(i, input_romain_str)) {
      int i_ret = romanToInt(input_romain_str);
      if (i != i_ret) {
        std::cout<<"err, i:"<<i<<", conver_to string:"<<input_romain_str<<std::endl;
        break;
      }
    }
  }
  

  int i = 0;
  return 0;
}