
// Web_ToursDoc.h : interface of the CWeb_ToursDoc class
//
#include <TString.h>
#include <TrecComPointer.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include <HTML_Reader.h>
#include <HTMLParser.h>
#pragma once
#define NUMBER_OF_BROWSER_EVENTS 3
typedef struct Tab
{
	TrecPointer<TControl> webPage;
	TArray<TControl> tabHistory;
	TArray<TString> titleHistory;
	TString title;
	UINT index, currentHistory, uiHold;
}Tab;

class CWeb_ToursDoc;

typedef void (CWeb_ToursDoc::*WebBrowserHandlers)(TControl* tc, EventArgs ea);

class CWeb_ToursDoc : public CDocument
{
	friend class CWeb_ToursView;
protected: // create from serialization only
	CWeb_ToursDoc();
	DECLARE_DYNCREATE(CWeb_ToursDoc)

// Attributes
public:

// Operations
public:
	void setUpTControl(CDC*);
	void onDraw();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWeb_ToursDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	TrecPointer<TControl> rootControl;
	TrecPointer<TTextField> urlBar;

	TrecPointer<AnafaceUI> WebSpace;

	void SetBack(TControl* tc, EventArgs ea);
	void SetForward(TControl* tc, EventArgs ea);
	void OnSetTab(TControl* tc, EventArgs ea);

	bool Parse_URL(TString t);
protected:
	PWSTR localResources;
	TString webResources;
	CFile file;
	CArchive* archer;


	RECT area;
	CView* attachedView;
	TrecComPointer<ID2D1DCRenderTarget> rt;
	TrecComPointer<ID2D1Factory> factory;

	HWND windowHandle;

	TrecPointer<TArray<styleTable>> table;
	TDataArray<eventNameID> idMatch;
	WebBrowserHandlers BrowserFunctions[NUMBER_OF_BROWSER_EVENTS];
	TArray<Tab> tabs;

	//Tab tabs[10];
	int currentTab;

	void AddToCurrentTab(TrecPointer<TControl>& webPage, TString webTitle);
	void MessageHandler(TDataArray<EventID_Cred>* credie = nullptr);
	TDataArray<EventID_Cred> cred;
	bool uiCreated;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
