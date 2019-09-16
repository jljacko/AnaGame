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
	virtual void Draw();

	// Event handlers for App specific controls and handers
	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut);

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
	TrecPointer<Page> drawer;

};

