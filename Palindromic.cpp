#include "define.h"
#include <map>

// 回文
string longestPalindrome(string s)
{
	if(1 == s.size()) return s;
	char* substr_temp = new char[s.length() + 1];
	strcpy(substr_temp, s.c_str());
	substr_temp[s.length()] = '\0';
	char* p_substr = substr_temp;
	
	vector<char *> pos_near;//1. 找出所有相邻字符位置
	vector<char *> pos_1_near;//2. 找出所有相邻字符相隔一个位置
	pair<int,string> palindrome_pair = make_pair(0,"0");
	while(*p_substr != '\0' && p_substr+1 != NULL)
	{
		if(*p_substr == *(p_substr+1))
		{
			pos_near.push_back(p_substr);
		}
		if(*p_substr == *(p_substr+2) && *p_substr != *(p_substr+1))
		{
			pos_1_near.push_back(p_substr);
		}
		p_substr++;
	}

	//处理1
	string temp;
	for (vector<char *>::iterator it=pos_near.begin(); it!= pos_near.end(); ++it)
	{
		temp.clear();
		char* p1 = *it;
		char* p2 = p1+1;
		while(p1 != NULL && p2 != NULL && *p1 == *p2) // 前端移动元素值相同
		{
			p1--;p2++; // 两侧相反方向移动
		}
		temp.append(++p1,p2);
		if(palindrome_pair.first < temp.size()) palindrome_pair = make_pair(temp.size(),temp);
	}

	//处理2
	for (vector<char *>::iterator it=pos_1_near.begin(); it!= pos_1_near.end(); ++it)
	{
		temp.clear();
		char* p1 = *it;
		char* p2 = p1+2;
		while(p1 != NULL && p2 != NULL && *p1 == *p2) // 前端移动元素值相同
		{
			p1--;p2++; // 两侧相反方向移动
		}
		//temp.append(++p1,p2);
		temp.append(s,p1-substr_temp,p2-p1);
		if(palindrome_pair.first < temp.size()) palindrome_pair = make_pair(temp.size(),temp);
	}

	return palindrome_pair.second;
}

int main()
{
	string ss1("abcddcba");
	string ss2("abcddcbaeabcddcbae");
	string ss3("abcdedcba");
	string ss4("abcdedcbaeabcdfdcbae");
	string ss5("bb");
	string ret = longestPalindrome(ss3);
	return 0;
};