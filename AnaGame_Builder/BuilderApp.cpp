#include "BuilderApp.h"

BuilderApp::BuilderApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i)
{
	instance = i;
	mainUI = m;
	outputUI = o;
	explorerUI = e;

	window = instance->GetMainWindow();
}

BuilderApp::~BuilderApp()
{
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
	return RECT();
}

void BuilderApp::OnSave()
{
}

void BuilderApp::OnShow()
{
}

void BuilderApp::onHide()
{
}

TString BuilderApp::GetFilePath()
{
	return filePath;
}

void BuilderApp::Draw()
{
	if (!drawer.Get() || drawer->GetRenderTarget().Get())
		return;
	drawer->GetRenderTarget()->BeginDraw();
	if (mainPage.Get())
		mainPage->onDraw();
	if (outputPane.Get())
		outputPane->onDraw();
	if (explorerPane.Get())
		explorerPane->onDraw();
	drawer->GetRenderTarget()->EndDraw();
}

void BuilderApp::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnRButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnRButtonDown(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnMouseMove(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnMouseMove(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnMouseMove(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonDblClk(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

void BuilderApp::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnLButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnLButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnLButtonUp(nFlags, point, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return;
		MessageHandler();
		return;
	}
}

bool BuilderApp::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	if (mainPage.Get())
	{
		mainPage->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont2;
		MessageHandler();
		return true;
	}

cont2:
	if (outputPane.Get())
	{
		outputPane->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			goto cont3;
		MessageHandler();
		return true;
	}

cont3:
	if (explorerPane.Get())
	{
		explorerPane->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, cred);
		if (*mOut == negative || *mOut == negativeUpdate)
			return false;
		MessageHandler();
		return true;
	}
	return false;
}

void BuilderApp::MessageHandler()
{
}

bool BuilderApp::InitializeControls()
{
	return false;
}
