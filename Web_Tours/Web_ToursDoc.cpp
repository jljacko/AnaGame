
// Web_ToursDoc.cpp : implementation of the CWeb_ToursDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Web_Tours.h"
#endif

#include "Web_ToursDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWeb_ToursDoc

// Anaface Level Event Hanlders
TString ev_Forward = L"OnForward";
TString ev_Backwars = L"OnBackwards";
TString ev_TabSwitch = L"OnSwitchTab";

IMPLEMENT_DYNCREATE(CWeb_ToursDoc, CDocument)

BEGIN_MESSAGE_MAP(CWeb_ToursDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CWeb_ToursDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CWeb_ToursDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()


// CWeb_ToursDoc construction/destruction

CWeb_ToursDoc::CWeb_ToursDoc()
{
	// TODO: add one-time construction code here
	uiCreated = false;

	for (UINT c = 0; c < NUMBER_OF_BROWSER_EVENTS; c++)
		BrowserFunctions[c] = nullptr;

	BrowserFunctions[0] = &CWeb_ToursDoc::SetBack;
	BrowserFunctions[1] = &CWeb_ToursDoc::SetForward;
	BrowserFunctions[2] = &CWeb_ToursDoc::OnSetTab;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = ev_Backwars;
	idMatch.push_back(enid);

	enid.eventID = 1;
	enid.name = ev_Forward;
	idMatch.push_back(enid);

	enid.eventID = 2;
	enid.name = ev_TabSwitch;
	idMatch.push_back(enid);

	currentTab = 0;
	
		HRESULT res = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localResources);
		ASSERT(SUCCEEDED(res));
		webResources = localResources;
		webResources += L"\\AnaGame\\Web-Tours";
		TString interfaceFileStr;
		interfaceFileStr += webResources;
		interfaceFileStr += L"\\BrowserInterface.tml.txt";
		ASSERT(file.Open(interfaceFileStr , CFile::modeReadWrite));
		
		archer = new CArchive(&file, CArchive::load);
		
		D2D1_FACTORY_OPTIONS ops;
		
		ops.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
		ID2D1Factory* f = nullptr;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ops, &f);
		factory = f;
		ASSERT(SUCCEEDED(hr));
		if (!SUCCEEDED(hr))
		{
			TRACE("Factory not Created\n");
			return;
		}
		ID2D1DCRenderTarget* tempDC = nullptr;
		hr = factory->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
			0, 0,
			D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
			D2D1_FEATURE_LEVEL_DEFAULT
		)), &tempDC);
		ASSERT(SUCCEEDED(hr));
		if (!SUCCEEDED(hr))
		{
			factory = NULL;
			TRACE("RenderTarget failed to generate\n");
			return;
		}
		rt = TrecComPointer<ID2D1DCRenderTarget>(tempDC);

		//rootControl = NULL;
		//rootLayout = NULL;
		//firstDraw = false;
		//WindowSet = false;
		CWnd* win = AfxGetMainWnd();
		if (win)
		{
			windowHandle = win->GetSafeHwnd();
			//WindowSet = true;
		}


		//CFile* file = ar.GetFile();
		TString directory = file.GetFilePath();
		TString fileName = file.GetFileName();
		int ind = directory.Find(fileName, 0);
		if (ind > 0)
			directory.Delete(ind, fileName.GetLength());
		// TODO: add loading code here

		ID2D1RenderTarget* baseTarget_raw = dynamic_cast<ID2D1RenderTarget*>(rt.get());
		TrecComPointer<ID2D1RenderTarget> baseTarget;
		baseTarget = baseTarget_raw;

		AnafaceParser* aParse = new AnafaceParser(baseTarget, windowHandle, &directory);
		TML_Reader_* reader = new TML_Reader_(archer, aParse);
		aParse->setEventSystem(idMatch);
		int readingError;
		if (!reader->read(&readingError))
		{
			readingError = readingError + 0;
		}
		else
		{
//			rootControl.Delete();
			rootControl = aParse->getRootControl();
//			ASSERT(rootControl.get());
//			rootControl->onCreate(area);
//			this->UpdateAllViews(NULL);
		}
		// End Parser testing code

		if (aParse)
			delete aParse;
		aParse = NULL;
		if (reader)
		{
			delete reader;
		}
		reader = NULL;
		archer->Close();
		delete archer;
		archer = nullptr;
}

CWeb_ToursDoc::~CWeb_ToursDoc()
{
	file.Close();
}

void CWeb_ToursDoc::setUpTControl(CDC *pCD)
{
	if (!rt.get() || !pCD)
		return;
	if (pCD)
	{
		windowHandle = pCD->GetWindow()->m_hWnd;
		//WindowSet = true;
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
	if (!uiCreated && rootControl.get())
	{
		rootControl->onCreate(area);

		TrecPointer<TLayout> layout = dynamic_cast<TLayout*>(rootControl.get());
		WebSpace = layout->GetLayoutChild(0, 2);

		TrecPointer<TLayout> tabLayout = dynamic_cast<TLayout*>(layout->GetLayoutChild(0, 1).get());
		
		TControl* possibleTab = tabLayout->GetLayoutChild(0,0).get();

		for (UINT C = 1; possibleTab; C++)
		{
			Tab* tab = new Tab;
			tab->currentHistory = 0;
			tab->index = C - 1;
			tabs.Add(tab);
			possibleTab = tabLayout->GetLayoutChild(C, 0).get();
		}

		layout = layout->GetLayoutChild(0, 0);
		urlBar = layout->GetLayoutChild(3, 0);

		uiCreated = true;
	}
	//	ASSERT(SUCCEEDED(hr));
}

void CWeb_ToursDoc::onDraw()
{

	rt->BeginDraw();
	D2D1_COLOR_F clearColor = D2D1::ColorF(D2D1::ColorF::White);
	rt->Clear(clearColor);
	//rt->Clear();
	if (rootControl.get())
		rootControl->onDraw();
	rt->EndDraw();
}

BOOL CWeb_ToursDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWeb_ToursDoc serialization

void CWeb_ToursDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void CWeb_ToursDoc::SetBack(TControl* tc, EventArgs ea)
{
	if (currentTab >= 0 && currentTab < tabs.Count())
	{
		TrecPointer<Tab> curTab = tabs.ElementAt(currentTab);
		if (!curTab.get())
			return;
		if(curTab->currentHistory > 1)
			curTab->currentHistory--;
		curTab->title = curTab->titleHistory.ElementAt(curTab->currentHistory - 1).get();
		curTab->webPage = curTab->tabHistory.ElementAt(curTab->currentHistory - 1);
		if (!WebSpace.get())
			return;
		WebSpace->setDontAddControl(curTab->webPage);

		SetWindowTextW(windowHandle, curTab->title.GetString());
	}
}

void CWeb_ToursDoc::SetForward(TControl* tc, EventArgs ea)
{
	if (currentTab >= 0 && currentTab < tabs.Count())
	{
		TrecPointer<Tab> curTab = tabs.ElementAt(currentTab);
		if (!curTab.get())
			return;
		if(curTab->currentHistory < curTab->tabHistory.Count())
			curTab->currentHistory++;
		curTab->title = curTab->titleHistory.ElementAt(curTab->currentHistory - 1).get();
		curTab->webPage = curTab->tabHistory.ElementAt(curTab->currentHistory - 1);
		if (!WebSpace.get())
			return;
		WebSpace->setDontAddControl(curTab->webPage);
		SetWindowTextW(windowHandle, curTab->title.GetString());
	}

}

void CWeb_ToursDoc::OnSetTab(TControl * tc, EventArgs ea)
{
}

bool CWeb_ToursDoc::Parse_URL(TString t)
{
	TString file;
	WCHAR* path = nullptr;
	bool localDrive = false;
	if (!t.Find(L"Desktop:"))
	{
		localDrive = true;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, 0, &path)))
			file = path;
		else return false;
	}
	else if (!t.Find(L"Documents:"))
	{
		localDrive = true;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, 0, &path)))
			file = path;
		else return false;
	}
	else if (!t.Find(L"Pictures:"))
	{
		localDrive = true;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Pictures, 0, 0, &path)))
			file = path;
		else return false;
	}
	else if (!t.Find(L"Videos:"))
	{
		localDrive = true;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Videos, 0, 0, &path)))
			file = path;
		else return false;
	}
	else if (!t.Find(L"Downloads:"))
	{
		localDrive = true;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, 0, &path)))
			file = path;
		else return false;
	}
	else
	{
		localDrive = false;
		// TO-DO: Assume url is online and add socket functionality. But for now, return false
		//	as feature is not yet implemented
		return false;
	}

	if (localDrive)
	{
		TrecPointer<TArray<TString>> peices = t.split(TString(L":"));
		if (peices->Count() < 2)
			return false;
		file = file + TString(L"\\");
		file + peices->ElementAt(1).get();
		for (UINT c = 2; c < peices->Count(); c++)
		{
			file += L":";
			file + peices->ElementAt(c).get();
		}
	}

	CFile fileObj;
	if(!fileObj.Open(file, CFile::modeRead))
		return false;
	CArchive arch(&fileObj, CArchive::load);

	TString fileName = fileObj.GetFileName();
	TString fileDirect = fileObj.GetFilePath();

	int ind = fileDirect.Find(fileName, 0);
	if (ind > 0)
		fileDirect.Delete(ind, fileName.GetLength());

	TrecComPointer<ID2D1RenderTarget> tempRT = rt.get();
	TrecPointer<TControl> page = nullptr;

	int error = 0;

	if (fileName.Find(L".tml") != -1)
	{
		AnafaceParser* parser = new AnafaceParser(tempRT, windowHandle, fileDirect);
		TML_Reader_ *reader = new TML_Reader_(&arch, parser);

		if (!reader->read(&error))
		{
			// To-Do: error

			delete parser;
			delete reader;
			return false;
		}
		page = parser->getRootControl();
	}
	else if (fileName.Find(L".html") != -1 || fileName.Find(L".htm") != -1 || fileName.Find(L"xhtml") != -1)
	{
		HTMLParser* parser = new HTMLParser(tempRT, windowHandle, fileDirect, localDrive);
		HTML_Reader* reader = new HTML_Reader(&arch, parser);

		if (!reader->read(&error))
		{
			// To-Do: error

			delete parser;
			delete reader;
			return false;
		}
		
		page = parser->GetRootControl();
		if (!page.get() || !WebSpace.get())
			return false;
		page->onCreate(WebSpace->getLocation());

		page->ShrinkHeight();

		TString title = parser->GetTitle();
		AddToCurrentTab(page, title);
	}

	return true;
}

void CWeb_ToursDoc::AddToCurrentTab(TrecPointer<TControl>& webPage, TString webTitle)
{
	if (currentTab > -1 && currentTab < tabs.Count())
	{
		tabs.ElementAt(currentTab)->currentHistory++;
		tabs.ElementAt(currentTab)->tabHistory.Add(webPage);
		tabs.ElementAt(currentTab)->title = webTitle;
		tabs.ElementAt(currentTab)->titleHistory.Add(new TString(webTitle));
		tabs.ElementAt(currentTab)->webPage = webPage;
		tabs.ElementAt(currentTab)->uiHold = WebSpace->addControl(webPage, webTitle);
		WebSpace->setDontAddControl(webPage);
		SetWindowTextW(windowHandle, webTitle);
	}
}

void CWeb_ToursDoc::MessageHandler(TDataArray<EventID_Cred>* credie)
{
	TDataArray<EventID_Cred>* currentCred = credie;
	if (!credie)
		credie = &cred;
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (int c = 0; c < credie->Size(); c++)
	{
		tc = credie->at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < NUMBER_OF_BROWSER_EVENTS)
		{
			// call method
			if (BrowserFunctions[e_id])
				(this->*BrowserFunctions[e_id])(tc, ea);
		}
	}

	cred.RemoveAll();
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWeb_ToursDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CWeb_ToursDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}


void CWeb_ToursDoc::SetSearchContent(const CString& value)
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

// CWeb_ToursDoc diagnostics

#ifdef _DEBUG
void CWeb_ToursDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWeb_ToursDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG


// CWeb_ToursDoc commands
