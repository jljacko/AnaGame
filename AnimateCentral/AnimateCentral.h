
// AnimateCentral.h : main header file for the AnimateCentral application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAnimateCentralApp:
// See AnimateCentral.cpp for the implementation of this class
//

class CAnimateCentralApp : public CWinAppEx
{
public:
	CAnimateCentralApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnimateCentralApp theApp;
