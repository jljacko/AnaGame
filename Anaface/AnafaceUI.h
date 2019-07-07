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
#include "TVideo.h"
#include "TContextMenu.h"
#include "TCanvas.h"
#include "TImage.h"

class AnafaceUI;
typedef struct _ANAFACE_DLL auiHold {
	TArray<TControl> children;
	RECT location = { 0,0,0,0 };
	TrecPointer<TControl> current = nullptr;
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



	bool switchView(UINT x);


	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
//	int loadFromTML(CArchive* ar);
	bool onCreate(RECT)override;
	int addControl(TrecPointer<TControl> control, TString tabName);
	int addControl(CArchive* arch);
	void setDontAddControl(TrecPointer<TControl> control);
	void onDraw(TObject* obj) override;
	TrecPointer<TControl> GetChildAt(UINT c);
	TrecPointer<TControl> GetCurrentChild();

	virtual UCHAR* GetAnaGameType() override;

	void AddNewTab(TString t);
	//void AddNewTabDefault(TString t);

	RECT GetControlArea();

	void Resize(RECT r)override;

private:
	TrecPointer<TControl> currentControl;
	TrecPointer<TLayoutEx> tabs;
	int tabHeight;
	HWND winHandle;
	UINT unknownTab;
};

