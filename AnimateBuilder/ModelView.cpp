// ModelView.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "ModelView.h"
#include "MainFrm.h"

// ModelView

IMPLEMENT_DYNCREATE(ModelView, CView)

ModelView::ModelView()
{

}

ModelView::~ModelView()
{
}

BEGIN_MESSAGE_MAP(ModelView, CView)
END_MESSAGE_MAP()


// ModelView drawing

void ModelView::OnDraw(CDC* pDC)
{
	ModelDoc* pDoc = dynamic_cast<ModelDoc*>(GetDocument());
	// TODO: add draw code here

	pDoc->OnDraw(pDC);
}


// ModelView diagnostics

#ifdef _DEBUG
void ModelView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void ModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
void ModelView::OnActivateView(BOOL bActivate, CView * pActivateView, CView * pDeactivateView)
{
	if (bActivate)
	{
		CMDIFrameWnd *frame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
		CMainFrame* sFrame = (CMainFrame*)frame;
		CMFCRibbonBar* ribbon = &sFrame->m_wndRibbonBar;
		ribbon->HideAllContextCategories();
		ribbon->ShowContextCategories(ID_CONTEXT5);
		ribbon->ActivateContextCategory(ID_CONTEXT5);
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactivateView);
}
#endif
#endif //_DEBUG


// ModelView message handlers
