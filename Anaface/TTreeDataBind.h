#pragma once
#include "TControl.h"
#include <TObjectNode.h>

/*
 * class TTreeDataBind
 * Purpose: Support for depicting a tree-based system on Anaface
 */
class _ANAFACE_DLL TTreeDataBind :
	public TControl
{
public:
	TTreeDataBind(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> ta);
	~TTreeDataBind();

	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;
	bool onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d) override;
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons) override;
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;

	void SetNode(TrecPointer<TObjectNode> newNode);
	D2D1_RECT_F getLocation()override;
	void Resize(D2D1_RECT_F& r)override;
protected:
	TrecPointer<TObjectNode> mainNode;

	bool isNodeSelected, isTickSelected;
	UINT nodeSelected;

	TrecPointer<TBrush> outerBrush;
	TrecPointer<TBrush> innerBrush;
};

