// shared_dll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "shared_dll.h"

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

// Cshared_dllApp

BEGIN_MESSAGE_MAP(Cshared_dllApp, CWinApp)
END_MESSAGE_MAP()


// Cshared_dllApp construction

Cshared_dllApp::Cshared_dllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cshared_dllApp object

Cshared_dllApp theApp;


// Cshared_dllApp initialization

BOOL Cshared_dllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//注意，函数前的extern "C"是不可少的，它指定按C语言约定来生成导出函数。不然，缺省情况下，C++编译器会生成冗长的函数修饰符，不能简单地用函数名来调用。

/*
extern "C" int Add(int d1,int d2){	return d1+d2; }
extern "C" int Sub(int d1,int d2){	return d1-d2; } 
extern "C" int Mul(int d1,int d2){	return d1*d2; }


extern "C" double Div(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return (double)d1/d2;
}

extern "C" int Mod(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return d1%d2;

}
*/

#define DllExport extern "C" _declspec(dllexport)

DllExport int Add(int d1,int d2){	return d1+d2; }
DllExport int Sub(int d1,int d2){	return d1-d2; } 
DllExport int Mul(int d1,int d2){	return d1*d2; } 

DllExport double Div(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return (double)d1/d2;
}

DllExport int Mod(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return d1%d2;
}

/*使用关键字__declspec(dllexport)导出函数
也可以不修改DEF文件，而在代码文件中直接用关键字__declspec(dllexport)和extern "C"来指定导出函数。
*/

/*
#define DllExport extern "C" _declspec(dllexport)

DllExport int Add(int d1,int d2){	return d1+d2; }
DllExport int Sub(int d1,int d2){	return d1-d2; } 
DllExport int Mul(int d1,int d2){	return d1*d2; } 

DllExport double Div(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return (double)d1/d2;
}

DllExport int Mod(int d1,int d2)
{
  if (d2 == 0)
  {
    AfxMessageBox(L"Divided by zero!");
    return 0;
  }
  return d1%d2;
}

*/

/**
 * 如下是使用说明：
 */

/*导出函数的方法
　　使用MFC创建DLL时，从项目中导出（export）函数到DLL文件的方法有：
使用模块定义文件(.def)。
使用 __declspec(dllexport) 关键字或其替代宏 AFX_EXT_CLASS。
这两种方法是互斥的，对每个函数只需用一种方法即可。另外，DEF文件只能用来导出函数，不能用于导出整个类。导出C++类，必须用 __declspec(dllexport) 关键字或其替代宏AFX_EXT_CLASS。
 
1．DEF文件
　　模块定义（module definition）文件（.def）是包含一个或多个描述DLL各种属性的模块语句的文本文件。DEF文件必须至少包含下列模块定义语句：
文件中的第一个语句必须是LIBRARY语句。此语句将.def文件标识为属于DLL。LIBRARY语句的后面是DLL的名称（缺省为DLL项目名）。链接器将此名称放到DLL的导入库中。
EXPORTS语句列出名称，可能的话还会列出DLL导出函数的序号值。通过在函数名的后面加上@符和一个数字，给函数分配序号值。当指定序号值时，序号值的范围必须是从1到N，其中N是DLL导出函数的个数。
即，DEF文件的格式为：（在这两个语句之间，还可以加上可选的描述语句：DESCRIPTION "库描述串"。分号;后的文本内容行为注释）
; 库名.def
LIBRARY 库名
EXPORTS
	函数名1		@1
	函数名2		@2
	……
	函数名n		@n
　　在使用MFC DLL向导创建MFC DLL项目时，VC会自动创建一个与项目同名但没有任何函数导出项的DEF文件（项目名.def），格式为：
; 项目名.def : 声明 DLL 的模块参数。
 
LIBRARY      "项目名"
 
EXPORTS
    ; 此处可以是显式导出
　　例如，项目名为RegDll的DEF文件（RegDll.def）的内容为：
; RegDll.def : 声明 DLL 的模块参数。
 
LIBRARY      "RegDll"
 
EXPORTS
　　　; 此处可以是显式导出
　　如果生成扩展DLL并使用.def文件导出，则将下列代码放在包含导出类的头文件的开头和结尾：
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA
// <你的头文件体>
#undef AFX_DATA
#define AFX_DATA
这些代码行确保内部使用的MFC变量或添加到类的变量是从扩展DLL导出（或导入）的。例如，当使用DECLARE_DYNAMIC派生类时，该宏扩展以将CRuntimeClass成员变量添加到类。省去这四行代码可能会导致不能正确编译或链接DLL，或在客户端应用程序链接到DLL时导致错误。
当生成DLL时，链接器使用.def文件创建导出(.exp)文件和导入库(.lib)文件。然后，链接器使用导出文件生成DLL文件。隐式链接到DLL的可执行文件在生成时链接到导入库。请注意，MFC本身就是使用.def文件从MFCx0.dll导出函数和类的。
*/