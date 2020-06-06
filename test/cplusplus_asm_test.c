#include "define.h"
int a;
void * address;

void * buildCode()
{
	char* code = (char*)malloc(16);
	char* pMov = code;
	char* pJmp = code+10;
	char* pAddress;
	//mov gi,18
	pMov[0] = 0xc7;
	pMov[1] = 0x05;
	pAddress = pMov + 2;
	*((int*)pAddress) = (int)&a;
	*((int*)(pAddress + 4)) = 18;
	//jump address
	pJmp[0] = 0xff;
	pJmp[1] = 0x25;
	*((int*)(&pJmp[2])) = (int)&address;
	return code;
}

int main()
{
	void * code = buildCode();
	_asm{
		mov address, offset _dl1
	}

	a = 12;
	printf("the a value %x.", &a);
	_asm jmp code			// 执行我们自己构建的代码
	a = 13;

_dl1:
	printf("the a value %x.", &a);
	return 0;
};
