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

// AnimateBuilder.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "AnimateBuilder.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "AnimateBuilderDoc.h"
#include "AnimateBuilderView.h"
#include "SourceCodeDoc.h"
#include "SourceCodeView.h"
#include "AnafaceView.h"
#include "ModelView.h"

#include "FileNewDialog.h"
#include "BuilderDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimateBuilderApp

BEGIN_MESSAGE_MAP(CAnimateBuilderApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CAnimateBuilderApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CAnimateBuilderApp construction

CAnimateBuilderApp::CAnimateBuilderApp()
{
	m_bHiColorIcons = TRUE;

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
#endif


	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("AnimateBuilder.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CAnimateBuilderApp object

CAnimateBuilderApp theApp;


// CAnimateBuilderApp initialization

BOOL CAnimateBuilderApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	if (m_pDocManager)
		delete m_pDocManager;
	m_pDocManager = new BuilderDocManager(this);

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
//	CMultiDocTemplate* pDocTemplate;


	pDocCAnimate = new CMultiDocTemplate(IDR_AnimateBuilderCsc_,
		RUNTIME_CLASS(SourceCodeDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(SourceCodeView));
	if (!pDocCAnimate)
		return FALSE;
	AddDocTemplate(pDocCAnimate);

	pDocJAnimate = new CMultiDocTemplate(IDR_AnimateBuilderJsc_,
		RUNTIME_CLASS(SourceCodeDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(SourceCodeView));
	if (!pDocJAnimate)
		return FALSE;
	AddDocTemplate(pDocJAnimate);

	pDoc_AdaMate = new CMultiDocTemplate(IDR_AnimateBuilderAda,
		RUNTIME_CLASS(SourceCodeDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(SourceCodeView));
	if (!pDoc_AdaMate)
		return FALSE;
	AddDocTemplate(pDoc_AdaMate);

	pDoc_Anaface = new CMultiDocTemplate(IDR_AnimateBuilder_Ana,
		RUNTIME_CLASS(AnafaceDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(AnafaceView));
	if (!pDoc_Anaface)
		return FALSE;
	AddDocTemplate(pDoc_Anaface);

	pDoc_Arena = new CMultiDocTemplate(106,
		RUNTIME_CLASS(ModelDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(ModelView));
	if (!pDoc_Arena)
		return FALSE;
	AddDocTemplate(pDoc_Arena);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CAnimateBuilderApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CAnimateBuilderApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CAnimateBuilderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



// CAnimateBuilderApp customization load/save methods

void CAnimateBuilderApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CAnimateBuilderApp::LoadCustomState()
{
}

void CAnimateBuilderApp::SaveCustomState()
{
}

// CAnimateBuilderApp message handlers



