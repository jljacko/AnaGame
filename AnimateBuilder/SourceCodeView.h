#pragma once
#include "stdafx.h"
#include <afxrich.h>
#include "SourceCodeDoc.h"
#include "MainFrm.h"

// SourceCodeView view

class SourceCodeView : public CRichEditView
{
	DECLARE_DYNCREATE(SourceCodeView)

protected:
	SourceCodeView();           // protected constructor used by dynamic creation
	virtual ~SourceCodeView();

public:
	SourceCodeDoc* GetDocument() const;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;
	DECLARE_MESSAGE_MAP()
};


