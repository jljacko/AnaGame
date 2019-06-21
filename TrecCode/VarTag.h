#pragma once
#include "BNFTag.h"
class VarTag :	public BNFTag
{
public:
	VarTag();
	~VarTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;

protected:
	bool create;
};

