#pragma once
#include <TObject.h>
#include "Page.h"
class _TAP_DLL TWindow :	public TObject
{
public:
	TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins);

	int PrepareWindow();

	int CompileView(TString& file, TrecComPointer<ID2D1Factory1> fact, TrecPointer<EventHandler> eh);

	bool MovePageToTop(TrecPointer<Page> p);
	TString GetWinClass();
	TString GetWinName();
	// Action Listeners
	void Draw();
	void Draw(Page& draw);
	HWND GetWindowHandle();

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg bool OnDestroy();

protected:
	HWND parent, currentWindow;
	HINSTANCE windowInstance;
	int command;
	TrecPointer<Page> mainPage;
	TDataArray<TrecPointer<Page>> pages;
	TString name, winClass;
	TrecComPointer<ID2D1Factory1> directFactory;
};

