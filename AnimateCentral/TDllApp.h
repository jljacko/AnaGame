#pragma once
#include "TApp.h"
#include "AnagameMessage.h"

typedef UINT(*DRAW)();
typedef UINT(*MESSAGE)(CPoint& point, UINT flags, UINT character, UINT repitition, AnaGameMessageType messageType);

class TDllApp :	public TApp
{
public:
	TDllApp();
	~TDllApp();
	bool Initialize(TMap<TString>& properties, TString& directory);

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	HMODULE library;
	void ReleaseLibrary();

	void processMessage(CPoint& point, UINT flags, WCHAR character, UINT repitition, AnaGameMessageType messageType);

	DRAW onDraw;
	MESSAGE onMessage;
};

