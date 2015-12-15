#include "define.h"
#include "template_test.h"
#include <vector>
#include <string>
typedef unsigned int uint32_t;

void split_str_to_num(char* sourc_str, const char *delim, std::vector<uint32_t>& info)
{    
	char *token; 
	while((token = strtok(sourc_str, delim)) != NULL){
		info.push_back(atoi(token));
		sourc_str = NULL;
	}         
}

uint32_t transform_time(const std::string& str_time)
{
	std::vector<uint32_t> t_vec;
	char temp_c[100] = {0};
	memcpy(temp_c, str_time.c_str(), str_time.length());
	split_str_to_num(temp_c, "/", t_vec);
	int seconds = 0, order  = 0;
	for(std::vector<uint32_t>::reverse_iterator r_it = t_vec.rbegin(); r_it != t_vec.rend(); ++r_it, ++order)
	{
		seconds += (*r_it) * pow(60.0, order);
	}
	return seconds;
}

int main()
{
	transform_time("21:10:00");
	return 0;
};