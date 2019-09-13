#pragma once

#include <AnafaceUI.h>
#include <AnafaceParser.h>
#include <TInstance.h>

class BuilderApp
{
public:
	BuilderApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance>);
	virtual ~BuilderApp();

	virtual TrecPointer<ArenaEngine> GetArenaEngine();

	TrecPointer<TControl> GetMainPage();

	virtual RECT Get3DArea();
	virtual void OnSave();

	virtual void OnShow();
	virtual void onHide();
	TString GetFilePath();

protected:
	TrecPointer<TControl> mainUI, outputUI, explorerUI;
	TrecPointer<TControl> mainPage, outputPane, explorerPane;
	virtual void MessageHandler();
	virtual bool InitializeControls();
	TDataArray<eventNameID> handleList;
	TDataArray<EventID_Cred> cred;
	TString filePath;
	TrecPointer<TInstance> instance;

	TrecPointer<TWindow> window;
};

