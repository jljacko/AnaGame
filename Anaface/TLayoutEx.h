#pragma once
#include "TLayout.h"
class _ANAFACE_DLL TLayoutEx :	public TLayout
{
public:
	TLayoutEx(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta);
	~TLayoutEx();

	int RemoveChildWithPrejudice(UINT x, UINT y);
	int RemoveColumn(UINT c);
	int RemoveRow(UINT c);

	UINT AddRow(UINT size);
	UINT AddCol(UINT size);

	TrecPointer<TControl> RemoveChild(UINT x, UINT y);

	virtual bool onCreate(RECT)override;
	virtual void onDraw() override;
	virtual UCHAR* GetAnaGameType()override;
};

