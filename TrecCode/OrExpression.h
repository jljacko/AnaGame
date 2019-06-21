#pragma once
#include "DoubleOpTag.h"
class OrExpression :
	public DoubleOpTag
{
public:
	OrExpression();
	~OrExpression();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

