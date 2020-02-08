#include "TIdeWindow.h"
#include "IDEPage.h"
#include "TInstance.h"

#include <TFileShell.h>


TIdeWindow::TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, 
	TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace):
	TWindow(name, winClass, style | WS_MAXIMIZE, parent, commandShow, ins)
{
	this->mainViewSpace = mainViewSpace;
	this->pageBarSpace = pageBarSpace;
}

int TIdeWindow::PrepareWindow()
{
	//body = TrecPointerKey::GetNewSelfTrecPointerAlt<Page, IDEPage>(ide_page_type_body, pageBarSpace);
	
	TrecPointer<Page>* pages[] = {
		&body,
		&basicConsole,
		&deepConsole,
		&upperRight,
		&lowerRight,
		&upperLeft,
		&lowerLeft
	};

	for (UINT c = 0; c < ARRAYSIZE(pages); c++)
	{
		*pages[c] = TrecPointerKey::GetNewSelfTrecPointerAlt<Page, IDEPage>(static_cast<ide_page_type>(c), pageBarSpace);
	}

	for (UINT c = 0; c < ARRAYSIZE(pages); c++)
	{
		for (UINT rust = 0; rust < ARRAYSIZE(pages); rust++)
		{
			if (c != rust)
				dynamic_cast<IDEPage*>(pages[c]->Get())->SetLink(*pages[rust], static_cast<ide_page_type>(rust));
		}
	}


	return TWindow::PrepareWindow();
}

void TIdeWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
	dynamic_cast<IDEPage*>(body.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(basicConsole.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(deepConsole.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(upperRight.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(lowerRight.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(upperLeft.Get())->OnLButtonUp();
	dynamic_cast<IDEPage*>(lowerLeft.Get())->OnLButtonUp();
	TWindow::OnLButtonUp(nFlags, point);
}

void TIdeWindow::OnMouseMove(UINT nFlags, TPoint point)
{
}

void TIdeWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
}

void TIdeWindow::AddNewPage(anagame_page pageType, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict)
{
	if (!mainPage.Get())
		return;

	TrecPointer<EventHandler> pageHandler;
	TFile uiFile;
	TrecPointer<TFileShell> fileShell;


	switch (pageType)
	{
	case anagame_page_code_explorer:

		break;
	case anagame_page_code_file:

		break;
	case anagame_page_command_prompt:

		break;
	case anagame_page_file_node:

		break;
	case anagame_page_object_explorer:

		break;
	case anagame_page_custom:
		if (!handler.Get())
			return;
		pageHandler = handler;
		uiFile.Open(tmlLoc, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	}

	if (!uiFile.IsOpen())
		return;

	if (!pageHandler.Get())
		return;

	if (!name.GetSize())
		return;
}

int TIdeWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
{
	if (!windowInstance.Get())
		return -1;
	if (!currentWindow)
		return -2;
	TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	if (!aFile.Get() || !aFile->IsOpen())
		return 1;

	directFactory = windowInstance->GetFactory();

	mainPage = Page::GetWindowPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), eh);

	RECT curArea = mainPage->GetArea();
	curArea.bottom = curArea.top + this->mainViewSpace;

	mainPage->SetArea(curArea);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);

	GetClientRect(GetWindowHandle(), &curArea);
	curArea.top += this->mainViewSpace;

	RECT left = curArea;
	RECT middle = curArea;
	RECT right = curArea;
	RECT bottom = curArea;

	int width = curArea.right - curArea.left;
	int height = curArea.bottom - curArea.top;

	left.right = width / 5;
	right.left = width / 5;
	middle.left = left.right;
	middle.right = right.left;

	bottom.top = height / 3 + mainViewSpace;

	left.bottom = right.bottom = middle.bottom = bottom.top;

	body->SetArea(middle);
	upperLeft->SetArea(left);
	upperRight->SetArea(right);
	basicConsole->SetArea({ 0,0,0,0 });
	lowerLeft->SetArea({ 0,0,0,0 });
	lowerRight->SetArea({ 0,0,0,0 });
	deepConsole->SetArea(bottom);

	Draw();

	return 0;
}
