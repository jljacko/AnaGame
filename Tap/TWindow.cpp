#include "TWindow.h"

TWindow::TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins)
{
	currentWindow = CreateWindowW(winClass.GetConstantBuffer(),
		name.GetConstantBuffer(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, nullptr, ins, nullptr);

	if (!currentWindow)
		throw L"Window ERROR! Failed to generate Window";

	this->name.Set(name);
	this->parent = parent;
	this->winClass.Set(winClass);
	this->windowInstance = ins;
	this->command = commandShow;
}

int TWindow::PrepareWindow()
{
	if (!currentWindow)
		return 1;
	ShowWindow(currentWindow, command);
	UpdateWindow(currentWindow);
	return 0;
}

int TWindow::CompileView(TString& file, TrecComPointer<ID2D1Factory1> fact, TrecPointer<EventHandler> eh)
{
	if (!fact.Get())
		return -1;
	if (!currentWindow)
		return -2;
	TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	if (!aFile.Get() || !aFile->IsOpen())
		return 1;

	mainPage = Page::GetWindowPage(fact, currentWindow, eh);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);

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

HWND TWindow::GetWindowHandle()
{
	return currentWindow;
}
