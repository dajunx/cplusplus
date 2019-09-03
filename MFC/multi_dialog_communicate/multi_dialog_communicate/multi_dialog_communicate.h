
// multi_dialog_communicate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cmulti_dialog_communicateApp:
// See multi_dialog_communicate.cpp for the implementation of this class
//

class Cmulti_dialog_communicateApp : public CWinApp
{
public:
	Cmulti_dialog_communicateApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cmulti_dialog_communicateApp theApp;