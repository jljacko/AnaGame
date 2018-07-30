#pragma once
#include "stdafx.h"
#include "AnafaceDoc.h"
#include "MainFrm.h"

// AnafaceView view

class AnafaceView : public CScrollView
{
	DECLARE_DYNCREATE(AnafaceView)

protected:
	AnafaceView();           // protected constructor used by dynamic creation
	virtual ~AnafaceView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	afx_msg void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView) override;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};


