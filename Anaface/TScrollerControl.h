#pragma once
#include "TControl.h"
class _ANAFACE_DLL TScrollerControl : public TControl
{
public:
	TScrollerControl(TrecPointer<DrawingBoard>, TrecPointer<TArray<styleTable>> styles);

	void onDraw(TObject* obj)override;

	void SetChildControl(TrecPointer<TControl> cont);

	virtual void Resize(D2D1_RECT_F&)override;

	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)override;
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)override;

	TrecPointer<TControl> GetChildControl();

protected:
	TrecPointer<TControl> childControl;
	void RefreshScroll();
	bool onScrollFocus;
};

