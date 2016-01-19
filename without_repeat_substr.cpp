#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
int lengthOfLongestSubstring(string s)
{
	int nSubstrMaxNonRepeatLength = 0;
	string no_rept_substr;

	char* substr_temp = new char[s.length() + 1];
	strcpy(substr_temp, s.c_str());
	substr_temp[s.length()] = '\0';
	char* p_substr = substr_temp;	// while循环一次，向后移动一个单词
	while ( '\0' != *substr_temp)
	{
		size_t pos = no_rept_substr.find(*substr_temp);
		if (pos == string::npos)
		{
			no_rept_substr.push_back(*substr_temp);
			if(no_rept_substr.size() > nSubstrMaxNonRepeatLength) nSubstrMaxNonRepeatLength = no_rept_substr.size();
		}
		else if((int)pos == 0)
		{
			if(no_rept_substr.size() > nSubstrMaxNonRepeatLength) nSubstrMaxNonRepeatLength = no_rept_substr.size();
			no_rept_substr.erase(0,1); // 删除首字母重复
			no_rept_substr.push_back(*substr_temp);
		}
		else
		{
			no_rept_substr.erase(0,pos+1);
			no_rept_substr.push_back(*substr_temp);
			if(no_rept_substr.size() > nSubstrMaxNonRepeatLength) nSubstrMaxNonRepeatLength = no_rept_substr.size();
		}
		substr_temp++;
	}
	if (s.size() > 0 && 0 == nSubstrMaxNonRepeatLength) nSubstrMaxNonRepeatLength++;
	return nSubstrMaxNonRepeatLength;
}
int main(int argCount,char ** args)
{
	string input_str;
	for (int i=0;i<326;i++)
	{
		input_str.append("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ");
	}
	input_str.append("abcdefghijklmnopqrstuvwxyzAB");
	lengthOfLongestSubstring(input_str);
	return 0;
};
