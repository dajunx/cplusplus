#include <iostream>
#include <vector>
#include <string>
using namespace std;
//6.ZigZag Conversion
/*
*1     7			   13
*2   6   8		12
*3 5       9    11
*4           10
*/
string convert(string s, int numRows)
{
	if(1 == numRows) return s;
	string str_return;

	//制造取值模板
	vector<pair<int,int> > template_key;
	int key = numRows;
	key--;
	template_key.push_back(make_pair(key*2, key*2));
	do
	{
		key--;
		if(key < 1) break;
		template_key.push_back(make_pair(key*2, (numRows-key)*2-2));
	}while(key > 0);
	template_key.push_back(make_pair(numRows*2-2, numRows*2-2)); //最后一行

	//取值
	str_return.clear();
	int i = 0;
	for (vector<pair<int,int> >::iterator it = template_key.begin(); it!=template_key.end(); ++it)
	{
		if(i < s.size()) 
			str_return.push_back(s.at(i));
		else
			break;

		int j = i;
		do
		{
			j = j + it->first;
			if(j < s.size())
			{
				str_return.push_back(s.at(j));
			}
			else
				break;
			j = j + it->second;
			if (j < s.size())
			{
				str_return.push_back(s.at(j));
			}
			else break;
		}while(true);
		++i;
	}
	return str_return;
}

int main()
{
	string ret = convert("abcdefghijklmnopqrstuvwxyz", 3);
	ret = convert("A", 1);
	return 0;
}
