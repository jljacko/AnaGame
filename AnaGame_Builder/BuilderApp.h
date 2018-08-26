#pragma once
#include <AnafaceUI.h>
#include <AnafaceParser.h>
class BuilderApp
{
public:
	BuilderApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, HWND window);
	~BuilderApp();

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint, messageOutput& mo);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point, messageOutput& mo);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput& mo);

	virtual TrecPointer<ArenaEngine> GetArenaEngine();

	TrecPointer<TControl> GetMainPage();

	virtual RECT Get3DArea();
	virtual void OnSave();

protected:
	TrecPointer<AnafaceUI> mainUI, outputUI, explorerUI;
	TrecPointer<TControl> mainPage, outputPane, explorerPane;
	virtual void MessageHandler();
	virtual bool InitializeControls();
	TDataArray<eventNameID> handleList;
	TDataArray<EventID_Cred> cred;
	TString filePath;

	HWND windowHandle;
};

