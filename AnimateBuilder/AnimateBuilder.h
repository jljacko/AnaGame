// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// AnimateBuilder.h : main header file for the AnimateBuilder application
//
#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAnimateBuilderApp:
// See AnimateBuilder.cpp for the implementation of this class
//

class CAnimateBuilderApp : public CWinAppEx
{
public:
	CAnimateBuilderApp();


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
//	afx_msg void OnFileNew();

	CMultiDocTemplate* pDocCAnimate; // for C-Animate Files (C++ derived Animate language)
	CMultiDocTemplate* pDocJAnimate; // for J_Animate Files (Java derived Animate Language)
	CMultiDocTemplate* pDoc_AdaMate; // For Adamate Files (Ada derived Animate Language)
	CMultiDocTemplate* pDoc_Anaface; // for creating UI files with Anaface
	CMultiDocTemplate* pDoc_Arena;
	DECLARE_MESSAGE_MAP()
};

extern CAnimateBuilderApp theApp;
