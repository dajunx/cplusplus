#include <iostream>
#include <fstream>
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

bool intToRoman(int num, string& roman_str)
{
  //待处理的整型数字必须大于0且小于3999
  if (num <= 0 || num > 3999) {
    return false;
  }

  map<int, char> map_roman_int;
  map_roman_int[1]    = 'I'; //1
  map_roman_int[2]    = 'V'; //5
  map_roman_int[3]    = 'X'; //10
  map_roman_int[4]    = 'L'; //50
  map_roman_int[5]    = 'C'; //100
  map_roman_int[6]    = 'D'; //500
  map_roman_int[7]    = 'M'; //1000

  roman_str.clear();
  int i_copy = num;
  for (int seq = 1;i_copy > 0;)
  {
    int pos = 0;
    if (i_copy > 10) {
      pos = i_copy%10;
    }
    else if (i_copy == 10) {
      roman_str.insert(roman_str.begin(), map_roman_int[seq+2]);
      break;
    } else {
      pos = i_copy;
    }

    switch(pos)
    {
    case 1:
    case 2:
    case 3:
      {
        for (;pos > 0;)
        {
          roman_str.insert(roman_str.begin(), map_roman_int[seq]);
          pos--;
        }
        break;
      }
    case 4:
      {
        roman_str.insert(roman_str.begin(), map_roman_int[seq+1]);
        roman_str.insert(roman_str.begin(), map_roman_int[seq]);        
        break;
      }
    case 5:
    case 6:
    case 7:
    case 8:
      {
        pos -= 5;
        for (;pos > 0;)
        {
          roman_str.insert(roman_str.begin(), map_roman_int[seq]);
          pos--;
        }
        roman_str.insert(roman_str.begin(), map_roman_int[seq+1]);
        break;
      }
    case 9:
      {
        roman_str.insert(roman_str.begin(), map_roman_int[seq+2]);
        roman_str.insert(roman_str.begin(), map_roman_int[seq]);        
      }
    default:
      break;
    }
    i_copy /= 10;
    seq += 2;
  }  

  return true;
}

int main()     
{
  string input_romain_str("MCMXCVI");
  std::fstream fs;
  fs.open ("linjj.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  for (int i = 1; i< 4000;++i)
  {
    if (true == intToRoman(i, input_romain_str)) {
      fs<<i<<" "<<input_romain_str<<std::endl;
    }
  }
  fs.close();

  //romanToInt(input_romain_str);
//   for (int i = 1; i< 4000;++i)
//   {
//     if (true == intToRoman(i, input_romain_str)) {
//       int i_ret = romanToInt(input_romain_str);
//       if (i != i_ret) {
//         std::cout<<"err, i:"<<i<<", conver_to string:"<<input_romain_str<<std::endl;
//         break;
//       }
//     }
//   }
  

  int i = 0;
  return 0;
}