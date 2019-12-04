#pragma once
#include "TLayout.h"

typedef enum appearCondition
{
	appear_unset = 0,
	appear_onClick = 1,      // user clicks on supervisor control
	appear_onHover = 2,      // user hovers over supervisor control
	appear_onClickOrHover = 3,
	appear_onRightClick = 4, // user right clicks on supervisor control
	appear_onEitherClick = 5,
	appear_onRClickOrHover = 6,
	appear_onEitherClickOrHover = 7,
	appear_onScript = 8      // an animate script triggered the appearence
}appearCondition;

class _ANAFACE_DLL TFlyout :	public TLayout
{
public:
	TFlyout(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	virtual ~TFlyout();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	void onDraw(TObject* obj = nullptr) override;
	bool Show(appearCondition);
	void Hide();
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	static TFlyout* DoDraw();
	virtual UCHAR* GetAnaGameType()override;
protected:
	appearCondition appearWhen;
	bool isShown;
	D2D1_RECT_F exitRect;
	int arrayLoc;
};


TDataArray<TFlyout*>* getFlyoutList();
