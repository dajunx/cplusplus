#include <iostream>
#include <string>
#include <map>
using namespace std;

struct player
{
	player() { }
	player(int key_, int a_, int b_)
		:key(key_), a(a_), b(b_) { }
	bool operator<(const player& a)const
	{
		return this->key < a.key;
	}
	int get() {return key;}
public:
	int key;
	int a;
	int b;
};

template<class _Ty>
struct less2
{
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{
		return _Left->get() < _Right->get();
	}
};

int main(int argCount,char ** args)
{
	map<player*, int, less2<player*> > test1; // 解决 指针类型key 用户自定义排序规则
	map<player*, int> test2; // 指针类型的key ,用户自定义排序规则无效
	map<player, int> test3; // 非指针类型的key, 用户自定义排序规则有效

	test1.insert(make_pair( (new player(9,2,1)),5 ) );
	test1.insert(make_pair( (new player(2,2,1)),5 ) );

	test2.insert(make_pair( (new player(9,2,1)),5 ) );
	test2.insert(make_pair( (new player(2,2,1)),5 ) );

	test3.insert(make_pair( *(new player(9,2,1)),5 ) );
	test3.insert(make_pair( *(new player(2,2,1)),5 ) );

	return 0;
};
