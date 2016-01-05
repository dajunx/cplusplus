#include "define.h"
#include <string.h>

int get_range_rank_low(int& value,int span)
{
	if(value == 0)
		return value;
	else if ( value <= 50)
	{
		value --;
		return value;
	} 
	else
	{
		int distance = span+(value%10);
		value = value*distance/100;
		return value;
	}
}

int get_range_rank_high(int& value,int span)
{
	if(value == 0)
		return value;
	else if ( value <= 50)
	{
		value ++;
		return value;
	} 
	else
	{
		int distance = span+(value%10);
		value = value*100/distance;
		return value;
	}
}

int main(int argCount,char ** args)
{
	int current_rank = 5,high_rank = 5;
	for (int i = 1;i < 100;i++)
	{
		system("cls");
		current_rank = i,high_rank = i;
		cout<<get_range_rank_low(current_rank,90)<<endl;
		cout<<get_range_rank_low(current_rank,90)<<endl;
		cout<<get_range_rank_low(current_rank,90)<<endl;
		cout<<get_range_rank_low(current_rank,80)<<endl;
		cout<<get_range_rank_low(current_rank,70)<<endl;

		cout<<get_range_rank_high(high_rank,90)<<endl;
		cout<<get_range_rank_high(high_rank,90)<<endl;
		cout<<get_range_rank_high(high_rank,90)<<endl;
		cout<<get_range_rank_high(high_rank,90)<<endl;
	}
	
	return 0;
};
