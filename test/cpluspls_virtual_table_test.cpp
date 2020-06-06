#include <iostream>
using namespace std;

//A simple class
class X
{
private:
	//Integer data member
		int m_nValue;
public:
	//Constructor with type integer as parameter
	X( int nVal )
	{
		m_nValue = nVal;
	}

	//virtual function with no parameter & no return type
	virtual void Func1( void )
	{
		std::cout << "Virtual Func1 Called!" << std::endl;
	}

	//virtual function with one integer parameter & no return type
	virtual void Func2( int nParam )
	{
		std::cout << "Virtual Func2 Called With Parameter Value: " << nParam << std::endl;
	}

	//virtual function with no parameter & integer return type
	virtual int Func3( void )
	{
		return this->m_nValue;
	}

	//Destructor
	virtual ~X( )
	{
	}

};

int main()
{
	//Creating object of class X as pointer and passing integer as parameter.
	X *pObj = new X( 123 );

	//Getting virtual table pointer of object pObj and assigning it to vfptr
	size_t *vfptr = *( reinterpret_cast< size_t** >( pObj ) );

	//Virtual Table is filled sequentially (from top to bottom), like
	//I have first declared Func1, so it will be on position '0'.
	//After that Func2, so it will be on position '1' on Virtual Table. And so on...
	//Now we will assign Function Addresses to its respective Function Pointers
	//that matches their declared function signature.
	//You can also use typedef for function pointers:
	//typedef void (__stdcall *FUNC1)( void );
	//FUNC1 Func1 = reinterpret_cast<FUNC1>(vfptr[0]);
	void (__stdcall *Func1)( void ) = (void (__stdcall *)(void))vfptr[0];
	void (__stdcall *Func2)( int nParam ) = (void (__stdcall *)(int))vfptr[1];
	int  (__stdcall *Func3)( void ) = (int (__stdcall *)(void))vfptr[2];

	//Before directly calling methods from virtual table, we first have to pass
	//this pointer.
	//Assigning this pointer to ECX register. VC++ compiler uses ECX register
	//to pass this pointer to its methods instead of pushing it on stack.
	__asm
	{
		mov ecx, pObj
	}

	//Calling Func1
	Func1( );

	//Again assigning this pointer. Hmm... Again assigning this pointer. Why?
	//Reason is EBX, ESI, EDI and EBP are preserved inside function call
	//and EAX, ECX, EDX are freely allowed to be overwritten. So there is
	//a fair chance that ECX register may have been overwritten so we are
	//again assigning this pointer to ECX register.
	__asm
	{
		mov ecx, pObj
	}

	//Calling Func2 and passing an integer as parameter.
	Func2( 123 );

	//Assigning this pointer. Same reason given above.
	__asm
	{
		mov ecx, pObj
	}

	//Calling Func3 and saving its return value in nRetVal.
	int nRetVal = Func3();

	std::cout << "Value returned by Func3: " << nRetVal << std::endl;
	return 0;
}