#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>

using namespace std;

class test : public boost::enable_shared_from_this<test>
{
public:
	int i;
	typedef boost::shared_ptr<test> ptr_test;
	test(int x):i(x) {}
	static ptr_test get_instance(int tmp)
	{
		return ptr_test(new test(tmp));
	}
	void pri()
	{
		std::cout<<"i:"<<i<<std::endl;
	}
	~test() { i = 7;}
};

class service
{
public:
	void get_impl(int x)
	{
// 		static boost::shared_ptr<test> ptr_impl_test = boost::make_shared<test>(9);
// 		ptr_impl_test->pri();
		static boost::shared_ptr<test> ptr_impl_test = test::get_instance(x);
		ptr_impl_test->pri();
	}
};
int main()
{
	boost::shared_ptr<service> ptr_service = boost::make_shared<service>();
	ptr_service->get_impl(9);
	ptr_service.reset();

	ptr_service = boost::make_shared<service>();
	ptr_service->get_impl(10);
	return 0;
}