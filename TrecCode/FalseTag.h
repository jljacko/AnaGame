#pragma once
#include "BNFTag.h"
class FalseTag :
	public BNFTag
{
public:
	FalseTag();
	~FalseTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

