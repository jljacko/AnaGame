#pragma once
#include "BNFTag.h"
class TrueTag :
	public BNFTag
{
public:
	TrueTag();
	~TrueTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

