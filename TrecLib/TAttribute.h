#pragma once
#include "TType.h"
#include "TMap.h"
#include "TTypeMetadata.h"

class TAttribute :
	public TType
{
public:
	TAttribute(TString& name);

	virtual short GetDetails() = 0;
	virtual TrecPointer<TType> GetType() = 0;
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&)override;
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&)override;

protected:
	TMap<TTypeMetadata> metadata;
};

