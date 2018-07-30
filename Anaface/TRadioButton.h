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

	bool onCreate(RECT);
	void onDraw();
	void onCreateClass();

	void storeInTML(CArchive* ar, int childLevel, bool overrideChildren = false) override;


	afx_msg void OnLButtonDown(UINT nFlags, CPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	virtual UCHAR* GetAnaGameType()override;

	void Resize(RECT r)override;

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

