#include "TIdeWindow.h"
#include "IDEPage.h"
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

void TIdeWindow::AddNewPage(anagame_page pageType, TString name, TString tmlLoc, TrecPointer<EventHandler> handler)
{
}
