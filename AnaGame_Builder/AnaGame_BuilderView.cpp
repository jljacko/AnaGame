 
// AnaGame_BuilderView.cpp : implementation of the CAnaGameBuilderView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnaGame_Builder.h"
#endif

#include "AnaGame_BuilderDoc.h"
#include "AnaGame_BuilderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnaGameBuilderView

IMPLEMENT_DYNCREATE(CAnaGameBuilderView, CView)

BEGIN_MESSAGE_MAP(CAnaGameBuilderView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CAnaGameBuilderView construction/destruction

CAnaGameBuilderView::CAnaGameBuilderView()
{
	// TODO: add construction code here

}

CAnaGameBuilderView::~CAnaGameBuilderView()
{
}

BOOL CAnaGameBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

BOOL CAnaGameBuilderView::OnEraseBkgnd(CDC * pDC)
{
	return 0;
}

// CAnaGameBuilderView drawing

void CAnaGameBuilderView::OnDraw(CDC* pDC)
{
	CAnaGameBuilderDoc* pDoc = GetDocument();
	ASSERT_VALID((CObject*)pDoc);
	if (!pDoc)
		return;
	pDoc->OnDraw(pDC);
	// TODO: add draw code for native data here
}


// CAnaGameBuilderView printing

BOOL CAnaGameBuilderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAnaGameBuilderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAnaGameBuilderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAnaGameBuilderView diagnostics

void CAnaGameBuilderView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	OnContextMenu(this, point);
	//TControl* messageControl = thisDoc->rootControl;
}

void CAnaGameBuilderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	//TrecPointer<TControl> messageControl = thisDoc->rootControl;

	messageOutput mo = negative;

	thisDoc->OnLButtonDown(nFlags, point, mo);
	doUpdate(&mo);
}

void CAnaGameBuilderView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	//thisDoc->SetRightClick(point);
	//TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	thisDoc->OnRButtonDown(nFlags, point, mo);
	doUpdate(&mo);
}

void CAnaGameBuilderView::OnMouseMove(UINT nFlags, CPoint point)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;


	messageOutput mo = negative;

	thisDoc->OnMouseMove(nFlags, point, mo);
	doUpdate(&mo);
}

void CAnaGameBuilderView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CAnaGameBuilderView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	thisDoc->OnLButtonDblClk(nFlags, point, mo);
	doUpdate(&mo);
}

void CAnaGameBuilderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = negative;

	thisDoc->OnLButtonUp(nFlags, point, mo);
	doUpdate(&mo);

}

void CAnaGameBuilderView::doUpdate(messageOutput *mo)
{
	if (*mo == positiveContinueUpdate || *mo == positiveOverrideUpdate || *mo == negativeUpdate)
		GetDocument()->UpdateAllViews(NULL);
}

void CAnaGameBuilderView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CAnaGameBuilderDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	thisDoc->OnChar(nChar, nRepCnt, nFlags, mo);
	doUpdate(&mo);
}

void CAnaGameBuilderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_ESCAPE:
	case VK_HOME:
	case VK_RETURN:
	case VK_END:
		CAnaGameBuilderDoc * thisDoc = GetDocument();
		TrecPointer<TControl> messageControl = thisDoc->rootControl;
		//ClientToScreen(&point);
		messageOutput mo = positiveContinue;
		if (messageControl.get())
			messageControl->OnChar(false, nChar, nRepCnt, nFlags, &mo, cred);
		doUpdate(&mo);
	}
}

void CAnaGameBuilderView::OnSize(UINT nType, int cx, int cy)
{
	CAnaGameBuilderDoc * thisDoc = GetDocument();
	if (!thisDoc)
		return;
	thisDoc->OnSize(nType, cx, cy);
}


#ifdef _DEBUG
void CAnaGameBuilderView::AssertValid() const
{
	CView::AssertValid();
}

void CAnaGameBuilderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnaGameBuilderDoc* CAnaGameBuilderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnaGameBuilderDoc)));
	return (CAnaGameBuilderDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnaGameBuilderView message handlers
