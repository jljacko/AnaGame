#pragma once
#include "stdafx.h"
#include "ModelDoc.h"
#include <TArena.h>
// ModelView view

class ModelView : public CView
{
	DECLARE_DYNCREATE(ModelView)

protected:
	ModelView();           // protected constructor used by dynamic creation
	virtual ~ModelView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	afx_msg void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView) override;
protected:
	TrecPointer<TArena> arena;


	DECLARE_MESSAGE_MAP()
};


