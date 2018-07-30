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

// AnimateBuilderView.cpp : implementation of the CAnimateBuilderView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnimateBuilder.h"
#endif

#include "AnimateBuilderDoc.h"
#include "AnimateBuilderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimateBuilderView

IMPLEMENT_DYNCREATE(CAnimateBuilderView, CView)

BEGIN_MESSAGE_MAP(CAnimateBuilderView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAnimateBuilderView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CAnimateBuilderView construction/destruction

CAnimateBuilderView::CAnimateBuilderView()
{
	// TODO: add construction code here

}

CAnimateBuilderView::~CAnimateBuilderView()
{
}

BOOL CAnimateBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAnimateBuilderView drawing

void CAnimateBuilderView::OnDraw(CDC* /*pDC*/)
{
	CAnimateBuilderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CAnimateBuilderView printing


void CAnimateBuilderView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAnimateBuilderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAnimateBuilderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAnimateBuilderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CAnimateBuilderView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAnimateBuilderView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAnimateBuilderView diagnostics

#ifdef _DEBUG
void CAnimateBuilderView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimateBuilderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimateBuilderDoc* CAnimateBuilderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimateBuilderDoc)));
	return (CAnimateBuilderDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimateBuilderView message handlers
