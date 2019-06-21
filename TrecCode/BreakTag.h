#pragma once
#include "BNFTag.h"
class BreakTag :
	public BNFTag
{
public:
	BreakTag();
	~BreakTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

