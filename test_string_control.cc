#include <iostream>
#include <vector>

using namespace std;

int main()
{
	string str = "a/a/a/0/0/0/0";
	string parent = "/";
	string::size_type pos; // "/" 位置
	int nCount = 0;		// 只去前五次
	vector<string> vec_str_return;

	for(int i = str.size() -1;i >= 0;)
	{
		pos = str.find_last_of(parent,i);
		if(pos != string::npos && nCount < 4)
		{
			nCount++;
			int num = i - (pos+1);
			num = num > 0?num:1;
			string temp = str.substr(pos+1,num);
			vec_str_return.push_back(temp);
			str.erase(pos,str.size() - pos);
			i = str.size() -1 > 0 ? str.size():1;
		}
		else
		{
			vec_str_return.push_back(str);
			break;
		}
	}

	return 0;
}
