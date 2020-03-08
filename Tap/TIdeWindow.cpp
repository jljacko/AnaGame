#include "TIdeWindow.h"
#include "IDEPage.h"
#include "TInstance.h"

#include <TFileShell.h>
#include <DirectoryInterface.h>


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
	
	TrecSubPointer<Page, IDEPage>* pages[] = {
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
		*pages[c] = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(static_cast<ide_page_type>(c), pageBarSpace, drawingBoard);
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
	focusPage.Nullify();
}

void TIdeWindow::OnMouseMove(UINT nFlags, TPoint point)
{ 
	if (locked) return;
	messageOutput output = negative;

	if (focusPage.Get())
	{
		focusPage.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, mainPage->GetArea()))
	{
		mainPage->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, body->GetArea()))
	{
		body.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, upperLeft->GetArea()))
	{
		upperLeft.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, upperRight->GetArea()))
	{
		upperRight.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, lowerLeft->GetArea()))
	{
		lowerLeft.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, lowerRight->GetArea()))
	{
		lowerRight.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, basicConsole->GetArea()))
	{
		basicConsole.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, deepConsole->GetArea()))
	{
		deepConsole.GetBase()->OnMouseMove(nFlags, point, &output);
		goto finish;
	}
	

finish:
	if (output == positiveContinueUpdate || output == positiveOverrideUpdate || output == negativeUpdate)
		Draw();
}

void TIdeWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput output = negative;
	if (isContained(point, mainPage->GetArea()))
	{
		mainPage->OnLButtonDown(nFlags, point, &output);
		goto finish;
	}

	if (isContained(point, body->GetArea()))
	{
		body.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = body;
		goto finish;
	}

	if (isContained(point, upperLeft->GetArea()))
	{
		upperLeft.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = upperLeft;
		goto finish;
	}

	if (isContained(point, upperRight->GetArea()))
	{
		upperRight.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = upperRight;
		goto finish;
	}

	if (isContained(point, lowerLeft->GetArea()))
	{
		lowerLeft.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = lowerLeft;
		goto finish;
	}

	if (isContained(point, lowerRight->GetArea()))
	{
		lowerRight.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = lowerRight;
		goto finish;
	}

	if (isContained(point, basicConsole->GetArea()))
	{
		basicConsole.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = basicConsole;
		goto finish;
	}

	if (isContained(point, deepConsole->GetArea()))
	{
		deepConsole.GetBase()->OnLButtonDown(nFlags, point, &output);
		focusPage = deepConsole;
		goto finish;
	}

finish:
	if (output == positiveContinueUpdate || output == positiveOverrideUpdate || output == negativeUpdate)
		Draw();
}

void TIdeWindow::AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict)
{
	if (!mainPage.Get())
		return;

	TrecPointer<EventHandler> pageHandler;
	TrecPointer<TFile> uiFile = TrecPointerKey::GetNewTrecPointer<TFile>();
	TrecPointer<TFileShell> fileShell;


	switch (pageType)
	{
	case anagame_page_code_explorer:

		break;
	case anagame_page_code_file:

		break;
	case anagame_page_command_prompt:
		uiFile->Open(GetDirectoryWithSlash(cd_Executable) + L"Resources\\IDEPrompt.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		break;
	case anagame_page_file_node:
		uiFile->Open(GetDirectoryWithSlash(cd_Executable) + L"Resources\\FileBrowser.tml", TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);
		fileShell = TFileShell::GetFileInfo(tmlLoc);
		break;
	case anagame_page_object_explorer:

		break;
	case anagame_page_custom:
		if (!handler.Get())
			return;
		pageHandler = handler;
		uiFile->Open(tmlLoc, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	}

	if (!uiFile->IsOpen())
		return;

	if (!pageHandler.Get())
		return;

	if (!name.GetSize())
		return;


	TrecSubPointer<Page, IDEPage> targetPage = body;
	switch (pageLoc)
	{
	case ide_page_type_basic_console:
		targetPage = basicConsole;
		break;
	case ide_page_type_deep_console:
		targetPage = deepConsole;
		break;
	case ide_page_type_lower_left:
		targetPage = lowerLeft;
		break;
	case ide_page_type_lower_right:
		targetPage = lowerRight;
		break;
	case ide_page_type_upper_left:
		targetPage = upperLeft;
		break;
	case ide_page_type_upper_right:
		targetPage = upperRight;
	}

	TrecPointer<Page> newPage = targetPage->AddNewPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), name, pageHandler);

	newPage->SetAnaface(uiFile, pageHandler);

	uiFile->Close();
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

	D2D1_RECT_F curArea = mainPage->GetArea();
	curArea.bottom = curArea.top + this->mainViewSpace;

	mainPage->SetArea(curArea);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);
	RECT lArea;
	GetClientRect(GetWindowHandle(), &lArea);
	curArea = convertRECTToD2DRectF(lArea);
	curArea.top += this->mainViewSpace;


	D2D1_RECT_F left = curArea;
	D2D1_RECT_F middle = curArea;
	D2D1_RECT_F right = curArea;
	D2D1_RECT_F bottom = curArea;

	int width = curArea.right - curArea.left;
	int height = curArea.bottom - curArea.top;

	left.right = width / 5;
	right.left = width - (width / 5);
	middle.left = left.right;
	middle.right = right.left;

	bottom.top = height * 2 / 3 + mainViewSpace;

	left.bottom = right.bottom = middle.bottom = bottom.top;

	body->SetArea(middle);
	upperLeft->SetArea(left);
	upperRight->SetArea(right);
	basicConsole->SetArea({ 0,0,0,0 });
	lowerLeft->SetArea({ 0,0,0,0 });
	lowerRight->SetArea({ 0,0,0,0 });
	deepConsole->SetArea(bottom);

	panelbrush = drawingBoard->GetBrush(TColor(D2D1::ColorF::BlueViolet));

	body->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperLeft->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	upperRight->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	basicConsole->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerLeft->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	lowerRight->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	deepConsole->SetResources(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));
	safeToDraw = 1;
	Draw();

	return 0;
}

void TIdeWindow::DrawOtherPages()
{
	if (body.Get())dynamic_cast<IDEPage*>(body.Get())->Draw(panelbrush, d3dEngine.Get());
	if (basicConsole.Get())dynamic_cast<IDEPage*>(basicConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (deepConsole.Get())dynamic_cast<IDEPage*>(deepConsole.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperLeft.Get())dynamic_cast<IDEPage*>(upperLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (upperRight.Get())dynamic_cast<IDEPage*>(upperRight.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerLeft.Get()) dynamic_cast<IDEPage*>(lowerLeft.Get())->Draw(panelbrush, d3dEngine.Get());
	if (lowerRight.Get())dynamic_cast<IDEPage*>(lowerRight.Get())->Draw(panelbrush, d3dEngine.Get());
}
