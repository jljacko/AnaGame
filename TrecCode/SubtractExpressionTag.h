#pragma once
#include "DoubleOpTag.h"
class SubtractExpressionTag : public DoubleOpTag
{
public:
	SubtractExpressionTag();
	~SubtractExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

