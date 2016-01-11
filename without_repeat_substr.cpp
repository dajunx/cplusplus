#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
/*Given a string, find the length of the longest substring without repeating characters. 
 *For example, the longest substring without repeating letters for "abcabcbb" is "abc", 
 *which the length is 3. For "bbbbb" the longest substring is "b", with the length of 1.
 *Subscribe to see which companies asked this question
 */
int lengthOfLongestSubstring(string s)
{
	unordered_map<char,int> cache_substr;
	int iSubStrLength = 0,lengthTemp = 0;
	char* sbustr_temp = new char[s.length() + 1];
	strcpy(sbustr_temp, s.c_str());
	sbustr_temp[s.length()] = '\0';
	char* p_substr = sbustr_temp;	// while循环一次，向后移动一个单词

	while ( '\0' != *sbustr_temp)
	{
		if(cache_substr.find(*sbustr_temp) == cache_substr.end())
		{
			cache_substr.insert(pair<char,int>(*sbustr_temp,1));
			lengthTemp++;
		}
		else if(iSubStrLength <= lengthTemp) // 找到重复值 并且长度不小于缓存长度
		{
			iSubStrLength = lengthTemp;
			cache_substr.clear();// 清理
			lengthTemp = 0;
			sbustr_temp = ++p_substr;
			continue;
		}
		sbustr_temp ++;
	}
	if(iSubStrLength < lengthTemp) iSubStrLength = lengthTemp;
	return iSubStrLength;
}
int main(int argCount,char ** args)
{
	string input_str;
	for (int i=0;i<361;i++)
	{
		input_str.append("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ");
	}
	input_str.append("abcdefghijklmnopqrstuvwxyzAB");
	lengthOfLongestSubstring(input_str);
	return 0;
};