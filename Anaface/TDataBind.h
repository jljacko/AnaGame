#pragma once
#include "TLayoutEx.h"
class _ANAFACE_DLL TDataBind : public TLayoutEx
{
public:
	TDataBind(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TDataBind();

	virtual void onDraw(TObject* obj = nullptr) override;
	virtual UCHAR* GetAnaGameType()override;
	void setData(TDataArrayBase* data);
	void setData(TArrayBase* data);
	bool onCreate(RECT r, TrecPointer<TWindowEngine> d3d) override;

protected:
	TDataArrayBase* dataRaw;
	TArrayBase* dataWrap;
	bool byRow;
};

