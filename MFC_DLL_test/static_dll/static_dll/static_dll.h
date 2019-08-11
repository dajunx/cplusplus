// static_dll.h : main header file for the static_dll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cstatic_dllApp
// See static_dll.cpp for the implementation of this class
//

class Cstatic_dllApp : public CWinApp
{
public:
	Cstatic_dllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
