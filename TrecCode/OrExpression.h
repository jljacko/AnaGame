#pragma once
#include "DoubleOpTag.h"
class OrExpression :
	public DoubleOpTag
{
public:
	OrExpression();
	~OrExpression();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

