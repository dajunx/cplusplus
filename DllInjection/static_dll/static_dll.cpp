// static_dll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "static_dll.h"
#include <process.h> // _beginthreadex head file
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// Cstatic_dllApp

BEGIN_MESSAGE_MAP(Cstatic_dllApp, CWinApp)
END_MESSAGE_MAP()


// Cstatic_dllApp construction

Cstatic_dllApp::Cstatic_dllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cstatic_dllApp object

Cstatic_dllApp theApp;


// Cstatic_dllApp initialization

BOOL Cstatic_dllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" { int _afxForceUSRDLL; }

#define DllExport extern "C" _declspec(dllexport)

DllExport void normal_write() {
  std::fstream file("filter_words.txt",
    std::fstream::in | std::fstream::out | std::fstream::app);
  file << "static_dll"<<std::endl;
  file.close();
}


DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  for (int i = 0; i < 100; ++i) {
    
    normal_write();
  
    Sleep(5000);
  }

  //ss <<"process -> static dll, i:"<<0<<std::endl;
  //normal_write(ss.str());

  return 0L;
}

// 加载dll的时候会调用好几次???
// https://bbs.pediy.com/thread-148376.htm
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  //MessageBox(NULL, _T("test"), _T(""), MB_OK);
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    {
      MessageBox(NULL,TEXT("DLL加载成功！"),TEXT("提示"),MB_OK);
      
      int ThreadInputData = 0;
      HANDLE thread1 = CreateThread(NULL, 0, ThreadProc, &ThreadInputData, 0, NULL);
    }
    break;
  case DLL_THREAD_ATTACH:
    //MessageBox(NULL,TEXT("DLL 线程 attach ！"),TEXT("提示"),MB_OK);
    break;
  case DLL_THREAD_DETACH:
    //MessageBox(NULL,TEXT("DLL 线程卸载！"),TEXT("提示"),MB_OK);
    break;
  case DLL_PROCESS_DETACH:
    MessageBox(NULL,TEXT("DLL卸载成功！"),TEXT("提示"),MB_OK);
    break;
  }

  return true;
}