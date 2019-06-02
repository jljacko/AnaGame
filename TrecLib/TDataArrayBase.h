#pragma once
#include "TObject.h"
class _TREC_LIB_DLL TDataArrayBase :
	public TObject
{
public:
	TDataArrayBase(UINT s, UINT c);
	~TDataArrayBase();

	virtual UCHAR* GetAnaGameType() override;
	UINT Size();
	virtual TObject* GetObjectAt(UINT loc) = 0;
protected:
	UINT size, capacity;
};

