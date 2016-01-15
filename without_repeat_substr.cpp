#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;
int lengthOfLongestSubstring(string s)
{
	int nSubstrMaxNonRepeatLength = 0;
	unordered_map<char,vector<int> > cache_;
	unordered_map<char,vector<int> >::iterator it;
	vector<int> vec_char;
	vector<int> vec_(1,0);
	for (int i=32;i<127;i++)
	{
		if(s.find((char)i) != string::npos)
		{
			vec_char.push_back(i);
			cache_.insert(pair<char,vector<int> >((char)i,vec_));
		}
	}

	char* substr_temp = new char[s.length() + 1];
	strcpy(substr_temp, s.c_str());
	substr_temp[s.length()] = '\0';
	char* p_substr = substr_temp;	// while循环一次，向后移动一个单词
	while ( '\0' != *substr_temp)
	{
		for (vector<int>::iterator iIt = vec_char.begin(); iIt != vec_char.end();++iIt)
		{
			it = cache_.find((char)*iIt);
			if ((char)*iIt == *substr_temp && it != cache_.end())
			{
				cache_[*iIt].push_back(0);
			}
			else if(it != cache_.end())
			{
				if(++it->second.back() > nSubstrMaxNonRepeatLength)
					nSubstrMaxNonRepeatLength = it->second.back();
			}
		}
		substr_temp ++;
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
	/*
	for (int i=0;i<326;i++)
	{
		input_str.append("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ");
	}
	input_str.append("abcdefghijklmnopqrstuvwxyzAB");
	*/
	return 0;
};