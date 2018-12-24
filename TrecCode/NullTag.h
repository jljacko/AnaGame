#pragma once
#include "BNFTag.h"
class NullTag :	public BNFTag
{
public:
	NullTag();
	~NullTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

