#include <iostream>
#include <string.h>
#include <string>
#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;

int reverse(int x)
{
	static vector<int> ret_vec;
	if( 0 == ret_vec.size() && (2147418111 <= x || 0 == x))
		return 0;
	else if( 0!= ret_vec.size() && 0 == x)
	{
		std::vector<int>::reverse_iterator rit = ret_vec.rbegin();
		int ret_tmp = 0, i = 1;
		for(;rit!=ret_vec.rend();++rit)
		{
			ret_tmp += (*rit)*i;
			i*= 10;
		}
		return ret_tmp;
	}
	ret_vec.push_back(x%10);
	int ret = reverse(x/10);
	if(ret >= 2147418111)
		return 0;
	return ret;
}

int main()
{
	int i = reverse(1230);
	return 0;
}
