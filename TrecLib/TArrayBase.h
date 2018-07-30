#pragma once
#include "TObject.h"
class _TREC_LIB_DLL TArrayBase :
	public TObject
{
public:
	TArrayBase();
	~TArrayBase();

	virtual UCHAR* GetAnaGameType()override;

protected:
	UINT count = 0;
	BYTE level = 0;
};

