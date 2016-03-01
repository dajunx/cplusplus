#include <iostream>
#include <string.h>
#include <string>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
using namespace std;

int reverse(int x)
{
	int64_t tmp = x;
	bool bIsNegativeNum = false;
	if(0 == x || 2147483647 < x || -2147483648 == x || 2147483648 == x)
		return 0;
	else if(x < 0)
	{
		tmp = -tmp;
		bIsNegativeNum = true;
	}

	vector<int64_t> ret_;
	for (int i = 10;tmp > 0;)
	{
		ret_.push_back(tmp%i);
		tmp = tmp / i;
	}

	int length = ret_.size() -1;
	int64_t key = 1;
	while (length --)
	{
		key *= 10;
	}

	tmp = 0;
	for (vector<int64_t>::iterator it = ret_.begin(); it!=ret_.end();++it)
	{
		tmp += static_cast<int64_t>((*it)*key);
		key = key/10;
	}
	if (0 == tmp || 2147483647 < tmp || -2147483648 == tmp || 2147483648 == tmp)
		return 0;
	else if(bIsNegativeNum) 
		return static_cast<int>(-tmp);
	else
		return static_cast<int>(tmp);
}

int main()
{
	int k = sizeof(int64_t);
	int i = reverse(1534236469);
	return 0;
}
