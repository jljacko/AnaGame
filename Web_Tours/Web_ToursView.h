
// Web_ToursView.h : interface of the CWeb_ToursView class
//
#include "Web_ToursDoc.h"
#pragma once







class CWeb_ToursView : public CScrollView
{
protected: // create from serialization only
	CWeb_ToursView();
	DECLARE_DYNCREATE(CWeb_ToursView)

// Attributes
public:
	CWeb_ToursDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWeb_ToursView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif





protected:
	TDataArray<EventID_Cred> cred;
// Generated message map functions
protected:



	void MessageHandler();

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
	void doUpdate(messageOutput*);
};

#ifndef _DEBUG  // debug version in Web_ToursView.cpp
inline CWeb_ToursDoc* CWeb_ToursView::GetDocument() const
   { return reinterpret_cast<CWeb_ToursDoc*>(m_pDocument); }
#endif

