#pragma once
#include "TType.h"
class TPrimitiveType :	public TType
{
public:
	TPrimitiveType(int size, BYTE type, TString& name);
	virtual int GetByteSize();
	virtual TrecPointer<TTypeMetadata> GetMetadata();
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&);
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&);

private:
	int size;
	BYTE type;
};

