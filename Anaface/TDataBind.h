#pragma once
#include "TLayoutEx.h"
class _ANAFACE_DLL TDataBind : public TControl
{
public:
	TDataBind(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TDataBind();

	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;
	void setData(TDataArrayBase* data);
	void setData(TArrayBase* data);
	bool onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d) override;
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons) override;
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr) override;
	void Resize(D2D1_RECT_F r);
protected:
	TDataArrayBase* dataRaw;
	TArrayBase* dataWrap;
	bool isStack;
	UINT widthHeight;
	TPoint mouseMovePoint;
};

