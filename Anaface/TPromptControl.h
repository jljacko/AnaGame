#pragma once
#include "TTextField.h"
#include <TShell.h>

class TPromptControl :
	public TTextField
{
public:
	TPromptControl(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND);
	~TPromptControl();

	virtual bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;
	virtual void onDraw(TObject* obj = nullptr) override;

	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	void SetText(TString);
protected:
	TString input;
	TShell shell;
	bool processRunning;
};

