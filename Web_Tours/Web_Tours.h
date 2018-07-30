
// Web_Tours.h : main header file for the Web_Tours application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWeb_ToursApp:
// See Web_Tours.cpp for the implementation of this class
//

class CWeb_ToursApp : public CWinApp
{
public:
	CWeb_ToursApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWeb_ToursApp theApp;
