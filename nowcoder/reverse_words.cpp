/*
	题目描述: 给定一个句子（只包含字母和空格）， 将句子中的单词位置反转，单词用空格分割, 单词之间只有一个空格，前后没有空格。 比如： （1） “hello xiao mi”-> “mi xiao hello”
	输入描述: 输入数据有多组，每组占一行，包含一个句子(句子长度小于1000个字符)
	输出描述: 对于每个测试示例，要求输出句子中单词反转后形成的句子.

	示例1:
		输入：hello xiao mi
		输出：mi xiao hello
*/
#include <iostream>
#include <string>

int main()
{
    std::string str_src, str_tmp;
    while(std::cin >> str_tmp)
    {
        str_src.append(str_tmp + " ");
    }
    str_src.pop_back();
    
    std::string target_string;
    std::string::size_type space_pos;
    while ((space_pos = str_src.find_first_of(" ", 0)) != std::string::npos)
    {
        target_string.insert(0, str_src.substr(0, space_pos));
        target_string.insert(0, " ");
        str_src.erase(0, space_pos+1);
    }

    target_string.insert(0, str_src);
    std::cout<<target_string<<std::endl;
    
    return 0;
}