
// Web_ToursView.cpp : implementation of the CWeb_ToursView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Web_Tours.h"
#endif

#include "Web_ToursDoc.h"
#include "Web_ToursView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWeb_ToursView

IMPLEMENT_DYNCREATE(CWeb_ToursView, CScrollView)

BEGIN_MESSAGE_MAP(CWeb_ToursView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()




// CWeb_ToursView construction/destruction

CWeb_ToursView::CWeb_ToursView()
{
	// TODO: add construction code here

}

CWeb_ToursView::~CWeb_ToursView()
{
}

BOOL CWeb_ToursView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CWeb_ToursView drawing

void CWeb_ToursView::OnDraw(CDC* pDC)
{
	CWeb_ToursDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//CWeb_ToursDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;



	// TODO: add draw code for native data here
	//	if (firstDraw)
	//	{
	pDoc->setUpTControl(pDC);
	//		firstDraw = false;
	//	}
	pDoc->onDraw();

}

void CWeb_ToursView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CWeb_ToursView printing

BOOL CWeb_ToursView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWeb_ToursView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWeb_ToursView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWeb_ToursView diagnostics

#ifdef _DEBUG
void CWeb_ToursView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWeb_ToursView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}






CWeb_ToursDoc* CWeb_ToursView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeb_ToursDoc)));
	return (CWeb_ToursDoc*)m_pDocument;
}
#endif //_DEBUG

void CWeb_ToursView::MessageHandler()
{
	CWeb_ToursDoc* pDoc = GetDocument();
	pDoc->MessageHandler(&cred);
	cred.RemoveAll();
}

void CWeb_ToursView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	OnContextMenu(this, point);
	//TControl* messageControl = thisDoc->rootControl;
}

void CWeb_ToursView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;


	messageOutput mo = negative;

	ScrollLButtonDown(nFlags, point, &mo);
	if (mo != negative)
	{
		doUpdate(&mo);
		return;
	}





	//ClientToScreen(&point);
	mo = negative;
	if (messageControl.get())
		messageControl->OnLButtonDown(nFlags, point, &mo, cred);

	ResetComboBoxes();
	doUpdate(&mo);
}

void CWeb_ToursView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	//thisDoc->SetRightClick(point);
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	if (messageControl.get())
		messageControl->OnRButtonDown(nFlags, point, &mo, cred);
	doUpdate(&mo);
}

void CWeb_ToursView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;


	messageOutput mo = negative;

	ScrollMouseMove(nFlags, point, &mo);
	if (mo != negative)
	{
		doUpdate(&mo);
		return;
	}

	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnMouseMove(nFlags, point, &mo, cred);
	}

	if (messageControl.get())
		messageControl->OnMouseMove(nFlags, point, &mo, cred);
	doUpdate(&mo);
}

void CWeb_ToursView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CWeb_ToursView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = positiveContinue;
	if (messageControl.get())
		messageControl->OnLButtonDblClk(nFlags, point, &mo, cred);
	doUpdate(&mo);
}

void CWeb_ToursView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	//ClientToScreen(&point);
	messageOutput mo = negative;

	ScrollLButtonUp(nFlags, point, &mo);
	if (mo != negative)
	{
		doUpdate(&mo);
		return;
	}
	mo = positiveContinue;
	TFlyout* fly = TFlyout::DoDraw();
	if (fly)
	{
		fly->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
		{
			fly->Hide();
			//thisDoc->SetRightClick(CPoint(0, 0));


		}
	}

	if (messageControl.get())
		messageControl->OnLButtonUp(nFlags, point, &mo, cred);
	doUpdate(&mo);

}

void CWeb_ToursView::doUpdate(messageOutput *mo)
{
	MessageHandler();
	if (*mo == positiveContinueUpdate || *mo == positiveOverrideUpdate || *mo == negativeUpdate)
		GetDocument()->UpdateAllViews(NULL);
}

void CWeb_ToursView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWeb_ToursDoc* thisDoc = GetDocument();
	TrecPointer<TControl> messageControl = thisDoc->rootControl;
	messageOutput mo = positiveContinue;
	if (thisDoc->urlBar.get() && thisDoc->urlBar->isOnFocus())
	{
		if (nChar == VK_RETURN)
			thisDoc->Parse_URL(thisDoc->urlBar->GetText());
		else
			thisDoc->urlBar->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
		doUpdate(&mo);
		return;
	}
	//ClientToScreen(&point);

	if (messageControl.get())
		messageControl->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
	doUpdate(&mo);
}

void CWeb_ToursView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_ESCAPE:
	case VK_HOME:
	case VK_RETURN:
	case VK_END:
		CWeb_ToursDoc* thisDoc = GetDocument();
		TrecPointer<TControl> messageControl = thisDoc->rootControl;
		//ClientToScreen(&point);
		messageOutput mo = positiveContinue;
		if (messageControl.get())
			messageControl->OnChar(false, nChar, nRepCnt, nFlags, &mo, cred);
		doUpdate(&mo);
	}
}


// CWeb_ToursView message handlers
