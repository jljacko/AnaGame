#pragma once
#include "TGadgetControl.h"

/*
* Class: TComboBox
* Purpose: Supports Anaface's ability to have drop down menus
* SuperClass: TGadgetControl - allows the combobox to have a feture within it
*/
class _ANAFACE_DLL TComboBox :
	public TGadgetControl
{
	friend TComboBox;
public:
	TComboBox(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta);
	~TComboBox();

	bool onCreate(RECT);
	void onDraw();
	void onDraw(ID2D1RenderTarget* rt);
	void addElement(TString);
	bool removeElement(TString);
	virtual void storeInTML(CArchive* ar, int childLevel,bool ov = true)override;

	bool GetExtensionStatus();

	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	void decrimentLocation();
	void FinalizeUpdate();

	virtual UCHAR* GetAnaGameType()override;

	void Resize(RECT r) override;
private:
	bool showExtended;
	bool prepShowExtended;
	TArray<TString> elements;
	int childHeight;

	TString defaultText;

	TrecComPointer<ID2D1SolidColorBrush> extendedBrush;
	D2D1_RECT_F extendedSpace;
	bool reacted;
	int boxLoc;
	D2D1_POINT_2F leftpoint, vertexPoint, rightPoint;
};

void _ANAFACE_DLL DrawActiveComboBox(ID2D1RenderTarget* rt);

_ANAFACE_DLL TComboBox*  GetActiveComboBox();

void _ANAFACE_DLL ResetComboBoxes();