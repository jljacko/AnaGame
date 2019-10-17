#pragma once

#include <AnafaceUI.h>
#include <AnafaceParser.h>
#include "TInstance.h"
#include "EventHandler.h"

class _TAP_DLL MiniHandler: public EventHandler
{
public:
	MiniHandler(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance> i);
	virtual ~MiniHandler();

	virtual TrecPointer<TArenaEngine> GetArenaEngine();
	virtual bool InitializeControls();
	TrecPointer<Page> GetMainPage();

	void virtual Initialize(TrecPointer<Page> p)override;

	virtual RECT Get3DArea();
	virtual void OnSave();

	virtual void OnShow();
	virtual void onHide();
	TString GetFilePath();
	virtual void Draw();

	// Event handlers for App specific controls and handers
	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut)override;
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)override;

	void SetSelf(TrecPointer<MiniHandler> s);

protected:

	virtual void OnSave(TFile&);

	TrecPointer<TControl> mainUI, outputUI, explorerUI;
	TrecPointer<Page> mainPage, outputPane, explorerPane;
	TrecPointer<TControl> mainControl, outputControl, explorerControl;
	
	TDataArray<eventNameID> handleList;
	TDataArray<EventID_Cred> cred;
	TString filePath;
	TrecPointer<TInstance> instance;

	TrecPointer<TWindow> window;
	//TrecPointer<Page> drawer;

	TrecPointerSoft<MiniHandler> self;
};

