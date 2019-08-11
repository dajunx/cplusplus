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

//ע�⣬����ǰ��extern "C"�ǲ����ٵģ���ָ����C����Լ�������ɵ�����������Ȼ��ȱʡ����£�C++�������������߳��ĺ������η������ܼ򵥵��ú����������á�

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

/*ʹ�ùؼ���__declspec(dllexport)��������
Ҳ���Բ��޸�DEF�ļ������ڴ����ļ���ֱ���ùؼ���__declspec(dllexport)��extern "C"��ָ������������
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
 * ������ʹ��˵����
 */

/*���������ķ���
����ʹ��MFC����DLLʱ������Ŀ�е�����export��������DLL�ļ��ķ����У�
ʹ��ģ�鶨���ļ�(.def)��
ʹ�� __declspec(dllexport) �ؼ��ֻ�������� AFX_EXT_CLASS��
�����ַ����ǻ���ģ���ÿ������ֻ����һ�ַ������ɡ����⣬DEF�ļ�ֻ�����������������������ڵ��������ࡣ����C++�࣬������ __declspec(dllexport) �ؼ��ֻ��������AFX_EXT_CLASS��
 
1��DEF�ļ�
����ģ�鶨�壨module definition���ļ���.def���ǰ���һ����������DLL�������Ե�ģ�������ı��ļ���DEF�ļ��������ٰ�������ģ�鶨����䣺
�ļ��еĵ�һ����������LIBRARY��䡣����佫.def�ļ���ʶΪ����DLL��LIBRARY���ĺ�����DLL�����ƣ�ȱʡΪDLL��Ŀ�������������������Ʒŵ�DLL�ĵ�����С�
EXPORTS����г����ƣ����ܵĻ������г�DLL�������������ֵ��ͨ���ں������ĺ������@����һ�����֣��������������ֵ����ָ�����ֵʱ�����ֵ�ķ�Χ�����Ǵ�1��N������N��DLL���������ĸ�����
����DEF�ļ��ĸ�ʽΪ���������������֮�䣬�����Լ��Ͽ�ѡ��������䣺DESCRIPTION "��������"���ֺ�;����ı�������Ϊע�ͣ�
; ����.def
LIBRARY ����
EXPORTS
	������1		@1
	������2		@2
	����
	������n		@n
������ʹ��MFC DLL�򵼴���MFC DLL��Ŀʱ��VC���Զ�����һ������Ŀͬ����û���κκ����������DEF�ļ�����Ŀ��.def������ʽΪ��
; ��Ŀ��.def : ���� DLL ��ģ�������
 
LIBRARY      "��Ŀ��"
 
EXPORTS
    ; �˴���������ʽ����
�������磬��Ŀ��ΪRegDll��DEF�ļ���RegDll.def��������Ϊ��
; RegDll.def : ���� DLL ��ģ�������
 
LIBRARY      "RegDll"
 
EXPORTS
������; �˴���������ʽ����
�������������չDLL��ʹ��.def�ļ������������д�����ڰ����������ͷ�ļ��Ŀ�ͷ�ͽ�β��
#undef AFX_DATA
#define AFX_DATA AFX_EXT_DATA
// <���ͷ�ļ���>
#undef AFX_DATA
#define AFX_DATA
��Щ������ȷ���ڲ�ʹ�õ�MFC��������ӵ���ı����Ǵ���չDLL���������룩�ġ����磬��ʹ��DECLARE_DYNAMIC������ʱ���ú���չ�Խ�CRuntimeClass��Ա������ӵ��ࡣʡȥ�����д�����ܻᵼ�²�����ȷ���������DLL�����ڿͻ���Ӧ�ó������ӵ�DLLʱ���´���
������DLLʱ��������ʹ��.def�ļ���������(.exp)�ļ��͵����(.lib)�ļ���Ȼ��������ʹ�õ����ļ�����DLL�ļ�����ʽ���ӵ�DLL�Ŀ�ִ���ļ�������ʱ���ӵ�����⡣��ע�⣬MFC�������ʹ��.def�ļ���MFCx0.dll������������ġ�
*/