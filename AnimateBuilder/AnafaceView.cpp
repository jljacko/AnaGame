// AnafaceView.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "AnafaceView.h"


// AnafaceView

IMPLEMENT_DYNCREATE(AnafaceView, CScrollView)

AnafaceView::AnafaceView()
{

}

AnafaceView::~AnafaceView()
{
}


BEGIN_MESSAGE_MAP(AnafaceView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// AnafaceView drawing

void AnafaceView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void AnafaceView::OnActivateView(BOOL bActivate, CView * pActivateView, CView * pDeactivateView)
{
	if (bActivate)
	{
		CMDIFrameWnd *frame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
		CMainFrame* sFrame = (CMainFrame*)frame;
		CMFCRibbonBar* ribbon = &sFrame->m_wndRibbonBar;
		ribbon->HideAllContextCategories();
		ribbon->ShowContextCategories(ID_CONTEXT3);
		ribbon->ActivateContextCategory(ID_CONTEXT3);
	}
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactivateView);
}

void AnafaceView::OnMouseMove(UINT nFlags, CPoint point)
{
	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;

	AnafaceDoc* aDoc = NULL;
	if (typeid(*pDoc) == typeid(class AnafaceDoc))
		aDoc = (AnafaceDoc*)pDoc;
	if (aDoc)
	{
		TrecPointer<TControl> p = aDoc->returnRootControl();
		if (!p.get())
			return;
		messageOutput mo = negative;
		RECT area = aDoc->getArea();
		p->Builder_OnMouseMove(nFlags, point, p, area, &mo);
		if (mo == positiveContinueUpdate || mo == positiveOverrideUpdate || mo == negativeUpdate)
			aDoc->UpdateAllViews(nullptr);
	}
}

void AnafaceView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;

	AnafaceDoc* aDoc = NULL;
	if (typeid(*pDoc) == typeid(class AnafaceDoc))
		aDoc = (AnafaceDoc*)pDoc;
	if (aDoc)
	{
		TrecPointer<TControl> p = aDoc->returnRootControl();
		if (!p.get())
			return;
		p->Builder_OnLButtonUp(nFlags, point, p);

	}
}

void AnafaceView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDocument* pDoc = GetDocument();
	if (!pDoc)
		return;

	AnafaceDoc* aDoc = NULL;
	if (typeid(*pDoc) == typeid(class AnafaceDoc))
		aDoc = (AnafaceDoc*)pDoc;
	if (aDoc)
	{
		TrecPointer<TControl> p = aDoc->returnRootControl();
		if (!p.get())
			return;
		messageOutput mo = negative;
		TrecPointer<TControl> tcp = aDoc->returnFocusControl();
		p->Builder_OnLButtonDown(nFlags, point, tcp, &mo);
		if (mo == positiveContinueUpdate || mo == positiveOverrideUpdate || mo == negativeUpdate)
			aDoc->UpdateAllViews(nullptr);
	}
}

void AnafaceView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	if (!pDC || !pDoc)
		return;

	AnafaceDoc* aDoc = NULL;
	if (typeid(*pDoc) == typeid(class AnafaceDoc))
		aDoc = (AnafaceDoc*)pDoc;
	if (aDoc)
		aDoc->setUpTControl(pDC);

	aDoc->OnDraw();

	// TODO: add draw code here
}


// AnafaceView diagnostics

#ifdef _DEBUG
void AnafaceView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void AnafaceView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// AnafaceView message handlers
