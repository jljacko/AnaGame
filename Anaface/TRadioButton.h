#pragma once
#include "TGadgetControl.h"

/*
* Class TRadioButton
* Purpose: Implements Anaface's version of the radio buttom
*/
class _ANAFACE_DLL TRadioButton :
	public TGadgetControl
{
	friend class TRadioButton;
public:
	TRadioButton(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta);
	~TRadioButton();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	void onDraw(TObject* obj = nullptr) override;
	void onCreateClass();

	void storeInTML(TFile* ar, int childLevel, bool overrideChildren = false) override;


	afx_msg void OnLButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	virtual UCHAR* GetAnaGameType()override;

	void Resize(D2D1_RECT_F& r)override;

private:
	//static TDataArray<TRadioButton*> otherButtons;
	int otherButtonLocation;
	TDataArray<TRadioButton*> otherSameButtons;
	bool isClicked;
	D2D1_ELLIPSE ellBut;
	TString* buttonClass;
	void addButton(TRadioButton*);
	bool onClick;
};

