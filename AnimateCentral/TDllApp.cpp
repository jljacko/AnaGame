#include "stdafx.h"
#include "TDllApp.h"

TDllApp::TDllApp()
{
	library = NULL;
	onDraw = nullptr;
	onMessage = nullptr;
}

TDllApp::~TDllApp()
{
	ReleaseLibrary();
}

bool TDllApp::Initialize(TMap<TString>& properties, TString& directory)
{
    TString* prop = properties.retrieveEntry(TString(L"DirectoryPath")).get();

	if (!prop)
		return false;
	if (!SetDllDirectoryW(directory)) return false;

	library = LoadLibraryW(prop->GetBuffer());
	prop->ReleaseBuffer();

	if (!library)
		return false;

	onDraw = (DRAW)GetProcAddress(library, "OnDraw");
	onMessage = (MESSAGE)GetProcAddress(library, "OnMessage");

	if (onDraw && onMessage)
		return true;
	return false;
}

void TDllApp::OnRButtonUp(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseRUp);
}

void TDllApp::OnLButtonDown(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseLDown);
}

void TDllApp::OnRButtonDown(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseRDown);
}

void TDllApp::OnMouseMove(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseMove);
}

void TDllApp::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void TDllApp::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseLDoubleClick);
}

void TDllApp::OnLButtonUp(UINT nFlags, CPoint point)
{
	processMessage(point, nFlags, 0, 0, agmt_MouseLUp);
}

void TDllApp::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	processMessage(CPoint(0,0), nFlags, nChar, nRepCnt, agmt_MouseLDown);
}

void TDllApp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	processMessage(CPoint(0, 0), nFlags, nChar, nRepCnt, agmt_MouseLDown);
}


void TDllApp::OnSize(UINT nType, int cx, int cy)
{
	processMessage(CPoint(cx, cy), 0, 0, 0, agmt_WindowSize);
}

void TDllApp::ReleaseLibrary()
{
	if (library)
		FreeLibrary(library);

	library = NULL;
}

void TDllApp::processMessage(CPoint& point, UINT flags, WCHAR character, UINT repitition, AnaGameMessageType messageType)
{
	if (!onMessage)
		return;
	UINT messageResponse = onMessage(point, flags, character, repitition, messageType);
}
