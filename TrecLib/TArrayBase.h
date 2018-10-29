#pragma once
#include "TObject.h"
class _TREC_LIB_DLL TArrayBase :
	public TObject
{
public:
	TArrayBase();
	~TArrayBase();

	virtual UCHAR* GetAnaGameType()override;
	UINT Count();

	virtual TObject* GetObjectAt(UINT loc) = 0;

protected:
	UINT count = 0;
	BYTE level = 0;
};

