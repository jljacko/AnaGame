#pragma once
#include "BNFTag.h"
class BreakTag :
	public BNFTag
{
public:
	BreakTag();
	~BreakTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

