#pragma once
#include "TType.h"

#define PRIMITIVE_TYPE BYTE
#define PT_BOOLEAN		0
#define PT_INTEGER		1
#define PT_UNSIGNED_INT	2
#define PT_DECIMAL		3
#define PT_ACSII		4
#define PT_UNICODE		5

class TPrimitiveType :	public TType
{
public:
	TPrimitiveType(int size, BYTE type, TString& name);
	virtual int GetByteSize();
	virtual code_type_cat GetTypeCategory();
	BYTE GetType();
	virtual TrecPointer<TTypeMetadata> GetMetadata();
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&);
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&);

private:
	int size;
	BYTE type;
};

