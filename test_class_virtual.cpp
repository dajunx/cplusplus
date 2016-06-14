#include <iostream>
#include <string.h>
using namespace std;

class Base1 {
public:
	virtual void f() { cout << "Base1::f" << endl; }
	virtual void g() { cout << "Base1::g" << endl; }
	virtual void h() { cout << "Base1::h" << endl; }

};

class Base2 {
public:
	virtual void f() { cout << "Base2::f" << endl; }
	virtual void g() { cout << "Base2::g" << endl; }
	virtual void h() { cout << "Base2::h" << endl; }
};

class derive : public Base1, public Base2 {
public:
	virtual void f() { cout << "derive::f" << endl; }
	virtual void g1() { cout << "derive::g" << endl; }
	virtual void h1() { cout << "derive::h" << endl; }

};

int main()
{
	typedef void(*Fun)(void);

	derive d;

	Fun pFun = NULL;

	((Fun)(*((int *)(*(int *)(&d)) + 0)))();  
	((Fun)(*((int *)(*(int *)(&d)) + 1)))();  
	((Fun)(*((int *)(*(int *)(&d)) + 2)))(); 
	// Invoke the first virtual function 
	pFun = (Fun)*((int*)*(int*)(&d));
	pFun();
	pFun = (Fun)*((int*)*(int*)(&d) + 1);
	pFun();
	pFun = (Fun)*((int*)*(int*)(&d) + 2);
	pFun();
	pFun = (Fun)*((int*)*(int*)(&d) + 3);
	pFun();
	pFun = (Fun)*((int*)*(int*)(&d) + 4);
	pFun();

	pFun = (Fun)*((int*)*((int*)(&d) +1 ));
	pFun();
	pFun = (Fun)*((int*)*((int*)(&d) +1 ) + 1);
	pFun();
	pFun = (Fun)*((int*)*((int*)(&d) +1 ) + 2);
	pFun();
	pFun = (Fun)*((int*)*((int*)(&d) +1 ) + 3);
	pFun();
	return 0;
}