#pragma once
#include "DoubleOpTag.h"
class MultipExpressionTag :	public DoubleOpTag
{
public:
	MultipExpressionTag();
	~MultipExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

