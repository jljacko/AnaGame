#pragma once
#include "BNFTag.h"
class ContinueTag :	public BNFTag
{
public:
	ContinueTag();
	~ContinueTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

