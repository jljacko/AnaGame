#include "stdafx.h"
#include "BuilderApp.h"


BuilderApp::BuilderApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, HWND w)
{
	windowHandle = w;
	mainUI = m;
	outputUI = o;
	explorerPane = e;
}


BuilderApp::~BuilderApp()
{
}

void BuilderApp::OnRButtonUp(UINT nFlags, CPoint point, messageOutput & mo)
{
}

void BuilderApp::OnLButtonDown(UINT nFlags, CPoint point, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnLButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnLButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnLButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnRButtonDown(UINT nFlags, CPoint point, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnRButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnRButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnRButtonDown(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnMouseMove(UINT nFlags, CPoint point, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnMouseMove(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnMouseMove(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnMouseMove(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnLButtonDblClk(UINT nFlags, CPoint point, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnLButtonDblClk(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnLButtonDblClk(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnLButtonDblClk(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnLButtonUp(UINT nFlags, CPoint point, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnLButtonUp(nFlags, point, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput & mo)
{
	if (mainPage.get())
	{
		mainPage->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.get())
	{
		outputPane->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.get())
	{
		explorerPane->OnChar(true, nChar, nRepCnt, nFlags, &mo, cred);
		if (mo == negative || mo == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

TrecPointer<ArenaEngine> BuilderApp::GetArenaEngine()
{
	return TrecPointer<ArenaEngine>();
}

TrecPointer<TControl> BuilderApp::GetMainPage()
{
	return mainPage;
}

RECT BuilderApp::Get3DArea()
{
	return RECT{ 0,0,0,0 };
}

void BuilderApp::OnSave()
{
}

void BuilderApp::MessageHandler()
{
}

bool BuilderApp::InitializeControls()
{
	return false;
}
