#pragma once
#include "TFlyout.h"
class _ANAFACE_DLL TContextMenu :
	public TFlyout
{
public:
	TContextMenu(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TContextMenu();

	bool onCreate(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3d) override;
	bool addOption(TrecPointer<TString>);
	virtual void storeInTML(TFile* ar, int childLevel,bool)override;
	void onDraw(TPoint);
	virtual UCHAR* GetAnaGameType()override;
private:
	TrecPointer<TWindowEngine> winEngine;
	int childHeight;
};

