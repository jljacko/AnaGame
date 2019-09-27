#include "TWindow.h"
#include "TInstance.h"

TWindow::TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins)
{
	currentWindow = CreateWindowW(winClass.GetConstantBuffer(),
		name.GetConstantBuffer(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, nullptr, ins->GetInstanceHandle(), nullptr);

	if (!currentWindow)
	{
		int lastError = GetLastError();
		throw L"Window ERROR! Failed to generate Window";
	}
	this->name.Set(name);
	this->parent = parent;
	this->winClass.Set(winClass);
	this->windowInstance = ins;
	this->command = commandShow;

	HDC dc = GetWindowDC(currentWindow);
	SetMapMode(dc, MM_LOENGLISH);

	locked = false;
}

TWindow::~TWindow()
{
}

int TWindow::PrepareWindow()
{
	if (!currentWindow)
		return 1;

	assert(windowInstance.Get());

	windowInstance->RegisterDialog(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	ShowWindow(currentWindow, command);
	UpdateWindow(currentWindow);



	return 0;
}

int TWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
{
	if (!windowInstance.Get())
		return -1;
	if (!currentWindow)
		return -2;
	TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	if (!aFile.Get() || !aFile->IsOpen())
		return 1;

	directFactory = windowInstance->GetFactory();

	mainPage = Page::GetWindowPage(windowInstance, currentWindow, eh);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);
	mainPage->Draw();

	return 0;
}

bool TWindow::MovePageToTop(TrecPointer<Page> p)
{
	if (!p.Get())
		return false;

	if (!pages.Size())
		return false;

	if (pages[0].Get() == p.Get())
		return true;

	UINT loc = 0;
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get() == p.Get())
		{
			loc = Rust;
			break;
		}
	}
	if (!loc)
		return false;

	for (int assign = loc; assign - 1 > -1; assign--)
	{
		pages[assign] = pages[assign - 1];
	}
	pages[0] = p;
	return true;
}

TString TWindow::GetWinClass()
{
	return winClass;
}

TString TWindow::GetWinName()
{
	return name;
}

void TWindow::Draw()
{
	if (mainPage.Get())
		mainPage->Draw();
}

void TWindow::Draw(Page& draw)
{
	draw.Draw();
}

void TWindow::OnRButtonUp(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonUp(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnRButtonUp(nFlags, point, &mOut);
}

void TWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDown(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonDown(nFlags, point, &mOut);
}

void TWindow::OnRButtonDown(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonDown(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnRButtonDown(nFlags, point, &mOut);
}
void TWindow::OnMouseMove(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnMouseMove(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnMouseMove(nFlags, point, &mOut);
}

void TWindow::OnLButtonDblClk(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDblClk(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonDblClk(nFlags, point, &mOut);
}

void TWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonUp(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonUp(nFlags, point, &mOut);
}

bool TWindow::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (locked) return false;
	messageOutput mOut = negative;

	bool returnable = false;

	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			returnable = pages[c]->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		returnable = mainPage->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);

	return returnable;
}

bool TWindow::OnDestroy()
{
	if(mainPage.Get())
		return mainPage->OnDestroy();
	return true;
}

TrecPointer<Page> TWindow::GetHandlePage(bool singleton)
{
	if (singleton && handlePage.Get())
		return handlePage;

	TrecPointer<Page> ret = Page::Get2DPage(windowInstance, GetWindowDC(currentWindow), TrecPointer<EventHandler>());

	if (!ret.Get())
		return ret;

	pages.push_back(ret);

	if (singleton)
		handlePage = ret;

	return ret;
}

TrecPointer<Page> TWindow::GetHandlePage(const TString& name)
{
	TrecPointer<Page> ret = keyPages.retrieveEntry(name);
	if (ret.Get())
		return ret;

	ret = GetHandlePage(false);

	if (ret.Get())
		keyPages.addEntry(name, ret);

	return ret;
}

TrecPointer<Page> TWindow::Get3DPage(bool singleton, TString& engineId)
{
	if(!engineId.GetSize() && singleton)
		return _3DPage;



	if (!engineId.GetSize())
		return TrecPointer<Page>();

	TrecPointer<ArenaEngine> engine = ArenaEngine::GetArenaEngine(engineId, currentWindow, windowInstance->GetInstanceHandle());

	return Get3DPage(singleton, engine);
}

TrecPointer<Page> TWindow::Get3DPage(bool singleton, TrecPointer<ArenaEngine> engine)
{
	if (singleton)
	{
		if (!engine.Get())
			return _3DPage;
		if (_3DPage.Get()) {
			if(_3DPage->GetArenaEngine().Get() == engine.Get())
				return _3DPage;
			return TrecPointer<Page>();
		}
	}

	TrecPointer<Page> ret = Page::Get3DPage(windowInstance, engine, TrecPointer<EventHandler>());

	pages.push_back(ret);

	if (singleton)
	{
		_3DPage = ret;
	}

	return ret;
}

void TWindow::LockWindow()
{
	locked = true;
}

void TWindow::UnlockWindow()
{
	locked = false;
}

void TWindow::SetSelf(TrecPointer<TWindow> win)
{
	if (this != win.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(win);
}

HWND TWindow::GetWindowHandle()
{
	return currentWindow;
}
