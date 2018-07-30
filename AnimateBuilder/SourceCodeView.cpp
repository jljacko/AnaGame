// SourceCodeView.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "SourceCodeView.h"


// SourceCodeView

IMPLEMENT_DYNCREATE(SourceCodeView, CRichEditView)

SourceCodeView::SourceCodeView()
{

}

SourceCodeView::~SourceCodeView()
{
}

SourceCodeDoc * SourceCodeView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SourceCodeDoc)));
	return (SourceCodeDoc*)m_pDocument;
}

BOOL SourceCodeView::PreCreateWindow(CREATESTRUCT & cs)
{
	return CRichEditView::PreCreateWindow(cs);
}

void SourceCodeView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	SetMargins(CRect(720, 720, 720, 720));
}

BOOL SourceCodeView::OnPreparePrinting(CPrintInfo * pInfo)
{
	return DoPreparePrinting(pInfo);
}

BEGIN_MESSAGE_MAP(SourceCodeView, CRichEditView)
END_MESSAGE_MAP()


// SourceCodeView diagnostics

#ifdef _DEBUG
void SourceCodeView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void SourceCodeView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
void SourceCodeView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
	CRichEditView::OnDestroy();
}
void SourceCodeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}
void SourceCodeView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
void SourceCodeView::OnContextMenu(CWnd * pWnd, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
void SourceCodeView::OnActivateView(BOOL bActivate, CView * pActivateView, CView * pDeactiveView)
{
	if (bActivate)
	{
		CMDIFrameWnd *frame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
		CMainFrame* sFrame = (CMainFrame*)frame;
		CMFCRibbonBar* ribbon = &sFrame->m_wndRibbonBar;
		ribbon->HideAllContextCategories();
		ribbon->ShowContextCategories(ID_CONTEXT2);
		ribbon->ActivateContextCategory(ID_CONTEXT2);
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
#endif
#endif //_DEBUG


// SourceCodeView message handlers
