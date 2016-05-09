#include <iostream>
using namespace std;

class unint{
public:
	unint()
	{
		i = 1;
	}
	unint& operator++();
	const unint operator++(int);
	unint& operator+=(int);
	int ret()
	{
		return i;
	}
	int i;
};

unint& unint::operator++()
{
	*this +=1;
	return *this;
}
const unint unint::operator ++(int)
{
	unint oldvalue = *this;
	++(*this);
	return oldvalue;
}
unint& unint::operator+=(int a)
{
	this->i += a;
	return *this;
}

int main()
{
	//int i = 0;
// 	int *p1 = &(++i); //正确
// 	int *p2 = &(i++); //错误
	
	unint i;
	++i;
	i++;
	//int k = (i++++).ret();
	//int k = (++++i).ret();
	return 0;
}
