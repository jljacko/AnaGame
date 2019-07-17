
// AnimateCentralDoc.cpp : implementation of the CAnimateCentralDoc class
//
#include <iostream>
#include <typeinfo.h>
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AnimateCentral.h"
#endif

#include "AnimateCentralDoc.h"
#include <d2d1.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>

#include <propkey.h>
#include <TDataArray.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAnimateCentralDoc

IMPLEMENT_DYNCREATE(CAnimateCentralDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnimateCentralDoc, CDocument)

END_MESSAGE_MAP()


// CAnimateCentralDoc construction/destruction

CAnimateCentralDoc::CAnimateCentralDoc()
{
	drawFlyout = nullptr;

	// TODO: add one-time construction code here
	D2D1_FACTORY_OPTIONS ops;

	ops.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	ID2D1Factory* f = nullptr;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ops, &f);
	factory = f;
	ASSERT(SUCCEEDED(hr));
	if (!SUCCEEDED(hr))
	{
		printf("Factory not Created\n");
		return;
	}
	ID2D1DCRenderTarget* tempDC = nullptr;
	hr = factory->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE),
		0,0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
		)), &tempDC);
	ASSERT(SUCCEEDED(hr));
	if (!SUCCEEDED(hr))
	{
		factory = NULL;
		printf("RenderTarget failed to generate\n");
		return;
	}
	rt = TrecComPointer<ID2D1DCRenderTarget>(tempDC);

	rootControl = NULL;
	//rootLayout = NULL;
	firstDraw = false;
	WindowSet = false;
	CWnd* win = AfxGetMainWnd();
	if (win)
	{
		windowHandle = win->GetSafeHwnd();
		WindowSet = true;
	}
}

CAnimateCentralDoc::~CAnimateCentralDoc()
{
	rootControl.Delete();
	rt.Delete();
	factory.Delete();
}

BOOL CAnimateCentralDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CAnimateCentralDoc serialization

void CAnimateCentralDoc::Serialize(CArchive& ar)
{
	if (!WindowSet)
	{
		CWnd* win = AfxGetMainWnd();
		if (win)
		{
			windowHandle = win->GetSafeHwnd();
			WindowSet = true;
		}
		else return;
	}
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		CFile* file = ar.GetFile();
		TString directory = file->GetFilePath();
		TString fileName = file->GetFileName();
		int ind = directory.Find(fileName, 0);
		if (ind > 0)
			directory.Delete(ind, fileName.GetLength());
		// TODO: add loading code here

		ID2D1RenderTarget* baseTarget_raw = dynamic_cast<ID2D1RenderTarget*>(rt.get());
		TrecComPointer<ID2D1RenderTarget> baseTarget;
		baseTarget = baseTarget_raw;

		AnafaceParser* aParse = new AnafaceParser(baseTarget,windowHandle,&directory);
		TML_Reader_* reader = new TML_Reader_(&ar, aParse);
		int readingError;
		if (!reader->read(&readingError))
		{
			readingError = readingError + 0;
		}
		else
		{
			rootControl.Delete();
			rootControl = aParse->getRootControl();
			ASSERT(rootControl.get());
			rootControl->onCreate(area);
			this->UpdateAllViews(NULL);
		}
		// End Parser testing code

		if (aParse)
			delete aParse;
		aParse = NULL;
		if (reader)
			delete reader;
		reader = NULL;
	}
}

void CAnimateCentralDoc::setView(CView * cv)
{
	attachedView = cv;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAnimateCentralDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAnimateCentralDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAnimateCentralDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAnimateCentralDoc diagnostics

#ifdef _DEBUG
void CAnimateCentralDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnimateCentralDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

void CAnimateCentralDoc::SetRightClick(CPoint p)
{
	rightClickPoint = p;
}

#endif //_DEBUG
void CAnimateCentralDoc::setUpTControl(CDC * pCD)
{	
	if (!rt.get() || !pCD)
		return;
	if (pCD)
	{
		windowHandle = pCD->GetWindow()->m_hWnd;
		WindowSet = true;
	}
	int c = 0;
	flush:
	UINT boundResult = pCD->GetBoundsRect(&area, 0);
	GetClientRect(WindowFromDC(pCD->m_hDC), &area);
	ID2D1RenderTarget* tempRt = rt.get();

	//if (typeid(tempRt) != typeid(ID2D1DCRenderTarget*))
	//	return;
	//ID2D1DCRenderTarget* tempDC = dynamic_cast<ID2D1DCRenderTarget*>(tempRt);

	HRESULT hr = rt->BindDC(pCD->GetSafeHdc(), &area);

	int er;
	if (!SUCCEEDED(hr))
	{
		er = GetLastError();
		c++;
		if (c < 5)
		{
			rt->Flush();
			goto flush;
		}
	}
//	ASSERT(SUCCEEDED(hr));
}

void CAnimateCentralDoc::onDraw()
{
	rt->BeginDraw();
	//rt->Clear();
	if (rootControl.get())
		rootControl->onDraw();
	TFlyout* fly = TFlyout::DoDraw();
	if(fly)
	{
		try
		{
			TContextMenu* cMenu = dynamic_cast<TContextMenu*>(fly);
			if (cMenu)
			{
				cMenu->onDraw(rightClickPoint);
			}
			else
				fly->onDraw();
		}
		catch (std::bad_cast b)
		{
			fly->onDraw();
		}
	}
	rt->EndDraw();
}

bool CAnimateCentralDoc::getFirstDraw()
{
	return firstDraw;
}

void CAnimateCentralDoc::setDrawn()
{
	firstDraw = false;
}

RECT CAnimateCentralDoc::getLocation()
{
	if (rootControl.get() == NULL)
		return RECT();
	else
		return rootControl->getLocation();
}

// CAnimateCentralDoc commands


