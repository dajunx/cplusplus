/*
	题目描述: 给定一个英文字符串,请写一段代码找出这个字符串中首先出现三次的那个英文字符。
	输入描述: 输入数据一个字符串，包括字母,数字等。
	输出描述: 输出首先出现三次的那个英文字符.

	示例1:
		输入：Have you ever gone shopping and
		输出：e
*/
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string str_src, str_tmp;
    while(std::cin >> str_tmp)
    {
        str_src.append(str_tmp);
    }
    
    int char_array[123] = {0};    
    std::string::iterator it_char = str_src.begin();
    for(; it_char != str_src.end(); it_char++)
    {
        if (!((*it_char >= 65 && *it_char <= 90)
              || (*it_char >= 97 && *it_char <= 122)))
        {
          continue;
        }
        
        char_array[*it_char]++;
        
        if(char_array[*it_char] >= 3) {
            std::cout<<*it_char<<std::endl;
            break;
        }
    }
    
    return 0;
}