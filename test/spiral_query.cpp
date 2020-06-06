#include <iostream>

void fun(int input)
{
	int x = 0, y = 0, k = 1;
	int count = input;
	int step = 1;
	int sign = 1;
	for (int i = 1;i < count;)
	{
		for (int j = 0;j < step;j++)
		{
			if(i >= count) goto end;
			x += sign;
			i++;
		}
		for (int j = 0;j < step;j++)
		{
			if(i >= count) goto end;
			y += sign;
			i++;
		}
		sign = -sign;
		step++;
	}
end:
	std::cout<<"input:"<<input<<",x:"<<x<<",y:"<<y<<std::endl;
}

int main()
{
	for (int i = 1;i < 100; i++)
	{
		fun(i);
	}
	return 0;
}