// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#pragma pack(push)
#pragma pack(1)
#ifndef _WIN64
struct JmpCode
{
private:
	const BYTE jmp;
	DWORD address;

public:
	JmpCode(DWORD srcAddr, DWORD dstAddr)
		: jmp(0xE9)
	{
		setAddress(srcAddr, dstAddr);
	}

	void setAddress(DWORD srcAddr, DWORD dstAddr)
	{
		address = dstAddr - srcAddr - sizeof(JmpCode);
	}
};
#else
struct JmpCode
{
private:
	BYTE jmp[6];
	uintptr_t address;

public:
	JmpCode(uintptr_t srcAddr, uintptr_t dstAddr)
	{
		static const BYTE JMP[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
		memcpy(jmp, JMP, sizeof(jmp));
		setAddress(srcAddr, dstAddr);
	}

	void setAddress(uintptr_t srcAddr, uintptr_t dstAddr)
	{
		address = dstAddr;
	}
};
#endif
#pragma pack(pop)

void normal_write(std::string& str_input) {
	std::fstream file("filter_words.txt",
		std::fstream::in | std::fstream::out | std::fstream::app);
	file << str_input;
	file.close();
}

void hook(void* originalFunction, void* hookFunction, BYTE* oldCode)
{
	JmpCode code((uintptr_t)originalFunction, (uintptr_t)hookFunction);
	DWORD oldProtect, oldProtect2;
	VirtualProtect(originalFunction, sizeof(code), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(oldCode, originalFunction, sizeof(code));
	memcpy(originalFunction, &code, sizeof(code));
	VirtualProtect(originalFunction, sizeof(code), oldProtect, &oldProtect2);
}


void unhook(void* originalFunction, BYTE* oldCode)
{
	DWORD oldProtect, oldProtect2;
	VirtualProtect(originalFunction, sizeof(JmpCode), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(originalFunction, oldCode, sizeof(JmpCode));
	VirtualProtect(originalFunction, sizeof(JmpCode), oldProtect, &oldProtect2);
}


BYTE terminateProcessOldCode[sizeof(JmpCode)];

BOOL WINAPI MyTerminateProcess(HANDLE hProcess, UINT uExitCode)
{	
	std::stringstream ss;
	for (int i = 0; i < 1; ++i) {
	
		ss << "ע��ĺ�����, ִ�д���:" << i << std::endl;
		normal_write(ss.str());
		ss.str("");
	
		Sleep(1000);
	}

	//return FALSE; // ��ֹ��������
	return true; // ��ֹ��������
}


// DLL�����ء�ж��ʱ����
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	TCHAR processPath[MAX_PATH];
	TCHAR msg[MAX_PATH + 20];

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		GetModuleFileName(GetModuleHandle(NULL), processPath, MAX_PATH);
		_tcscpy_s(msg, _T("ע���˽��� "));
		_tcscat_s(msg, processPath);
		MessageBox(NULL, msg, _T(""), MB_OK);

		//hook(GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "TerminateProcess"), MyTerminateProcess, terminateProcessOldCode);
		//hook(GetProcAddress(GetModuleHandle(_T("D:\\github\\cplusplus\\MFC_DLL_test - ����\\test_dll\\Debug\\test_dll.exe")), "ThreadProc"), MyTerminateProcess, terminateProcessOldCode);
		HMODULE test = GetModuleHandle(_T("static_dll.dll"));
		//FARPROC ptr = GetProcAddress(test, "ThreadProc");
		FARPROC ptr = GetProcAddress(test, "normal_write");
		//ptr = GetProcAddress(test, "normal_write");
		hook(ptr, MyTerminateProcess, terminateProcessOldCode);
	}
		break;

	case DLL_PROCESS_DETACH:
		MessageBox(NULL, _T("DLLж����"), _T(""), MB_OK);

		// ж��ʱ�ǵ�unhook
		//unhook(GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "TerminateProcess"), terminateProcessOldCode);
		unhook(GetProcAddress(GetModuleHandle(_T("static_dll.dll")), "normal_write"), terminateProcessOldCode);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

