
// AnimateCentralView.h : interface of the CAnimateCentralView class
//
#include "stdafx.h"
#include "AnimateCentralDoc.h"
#pragma once


class CAnimateCentralView : public CView
{
protected: // create from serialization only
	CAnimateCentralView();
	DECLARE_DYNCREATE(CAnimateCentralView)

// Attributes
public:
	CAnimateCentralDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CAnimateCentralView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool firstDraw;
	TDataArray<EventID_Cred> cred;
// Generated message map functions
protected:
	
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt,	UINT nFlags);

	DECLARE_MESSAGE_MAP()

	void doUpdate(messageOutput*);
	
};

#ifndef _DEBUG  // debug version in AnimateCentralView.cpp
inline CAnimateCentralDoc* CAnimateCentralView::GetDocument() const
   { return reinterpret_cast<CAnimateCentralDoc*>(m_pDocument); }
#endif

