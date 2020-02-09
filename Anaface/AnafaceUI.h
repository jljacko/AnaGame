#pragma once
#include "TControl.h"
#include "TLayoutEx.h"
#include "TTextField.h"
#include "TCheckBox.h"
#include "TRadioButton.h"
#include "TComboBox.h"
#include "TCheckBox.h"
#include "TFlyout.h"
#include "TArena.h"
//#include "TVideo.h"
#include "TContextMenu.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TPromptControl.h"
#include "TTreeDataBind.h"

class AnafaceUI;
typedef struct _ANAFACE_DLL auiHold {
	TArray<TControl> children;
	RECT location = { 0,0,0,0 };
	TrecPointer<TControl> current;
	TArray<TString> source;
}auiHold;

typedef struct tempHold
{
	TrecPointer<TString> file;
	int index;
}tempHold;

/*
* class AnafaceUI
* Purpose: enables holding multiple controls in the same space, one at a time.
*	Use for different pages
*/
class _ANAFACE_DLL AnafaceUI : public TControl
{
public:
	AnafaceUI(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND);
	virtual ~AnafaceUI();

	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	//afx_msg void OnContextMenu(CWnd* pWnd, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);


	bool switchView(UINT x);

	virtual void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget> rt);

	// afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
//	int loadFromTML(CArchive* ar);
	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	int addControl(TrecPointer<TControl> control, TString tabName);
	int addControl(TFile* arch);
	void setDontAddControl(TrecPointer<TControl> control);
	void onDraw(TObject* obj) override;
	TrecPointer<TControl> GetChildAt(UINT c);
	TrecPointer<TControl> GetCurrentChild();

	virtual UCHAR* GetAnaGameType() override;

	void AddNewTab(TString t);
	//void AddNewTabDefault(TString t);

	D2D1_RECT_F GetControlArea();

	void Resize(D2D1_RECT_F r)override;

private:
	TrecPointer<TControl> currentControl;
	TrecPointer<TControl> tabs_base;

	TrecPointer<TWindowEngine> windowEngine;

	TLayoutEx* tabs;
	int tabHeight;
	HWND winHandle;
	UINT unknownTab;
	bool proceed;
};

