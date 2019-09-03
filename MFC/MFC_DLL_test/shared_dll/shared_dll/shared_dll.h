// shared_dll.h : main header file for the shared_dll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cshared_dllApp
// See shared_dll.cpp for the implementation of this class
//

class Cshared_dllApp : public CWinApp
{
public:
	Cshared_dllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
