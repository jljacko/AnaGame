#pragma once
#include "BNFTag.h"
class NullTag :	public BNFTag
{
public:
	NullTag();
	~NullTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

