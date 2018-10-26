#pragma once
#include "TLayoutEx.h"
class TDataBind :
	public TLayoutEx
{
public:
	TDataBind(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TDataBind();

	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;
	void setData(TrecPointer<TArray<TObject>>& data);
	bool onCreate(RECT r) override;

protected:
	TrecPointer<TArray<TObject>> data;
	bool byRow;
};

