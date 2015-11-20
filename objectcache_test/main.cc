#include<iostream>
#include "ObjectCache.hpp"
using namespace std;

class test_cache: public base::ObjectCache<int, int>
{
};

//define which one need cache
class test_obj: public base::EventableObject
{
public:
	void callback(const test_cache::ResultType& result, const test_cache::RequestSet& request)
	{
		test_cache::ResultType::const_iterator itr = result.begin();
		for(; itr!= result.end(); ++itr)
		{
			std::cout<<"cache:"<<itr->first<<":"<<itr->second<<std::endl;
		}
	}
};

int main()
{
	test_cache cache;
	test_obj obj;

	//configure the cache request
	test_cache::RequestSet request;
	request.insert(1);
	request.insert(2);
	request.insert(3);
	cache.add_callback(request,&obj,&test_obj::callback);

	int a = 1;
	int a_v = 2;
	// update cache
	cache.update(a,a_v,0);
	a = 2;
	cache.update(a,a_v,0);
	a = 3;
	cache.update(a,a_v,0);
	return 0;
}
