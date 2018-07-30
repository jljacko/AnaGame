#pragma once
#include "TFlyout.h"
class _ANAFACE_DLL TContextMenu :
	public TFlyout
{
public:
	TContextMenu(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TContextMenu();

	bool onCreate(RECT location) override;
	bool addOption(TrecPointer<TString>);
	virtual void storeInTML(CArchive* ar, int childLevel,bool)override;
	void onDraw(CPoint);
	virtual UCHAR* GetAnaGameType()override;
private:
	int childHeight;
};

