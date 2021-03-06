#pragma once
#include "TControl.h"


/*
* Class TGadgetControl
* Purpose: To support controls with a button within them
*/
class _ANAFACE_DLL TGadgetControl :
	public TControl
{
public:
	TGadgetControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> ta, bool isGagetBase = true);
	virtual ~TGadgetControl();

	virtual void storeInTML(TFile* ar, int childLevel,bool ov = false) override;

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d)override;
	virtual UCHAR* GetAnaGameType()override;


	void Resize(D2D1_RECT_F& r) override;
protected:
	int bSize;
	RECT checker;

	TrecPointer<TBrush> brush;

	float thickness;

	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	D2D1_RECT_F DxLocation;
	bool isGadBase;
};

