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

void BuilderApp::MessageHandler()
{
}

bool BuilderApp::InitializeControls()
{
	return false;
}
