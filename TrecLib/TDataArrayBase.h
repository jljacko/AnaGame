#pragma once
#include "TObject.h"
class _TREC_LIB_DLL TDataArrayBase :
	public TObject
{
public:
	TDataArrayBase();
	~TDataArrayBase();

	virtual UCHAR* GetAnaGameType() override;
protected:
	UINT size, capacity;
};

