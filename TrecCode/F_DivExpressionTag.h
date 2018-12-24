#pragma once
#include "DoubleOpTag.h"
class F_DivExpressionTag :	public DoubleOpTag
{
public:
	F_DivExpressionTag();
	~F_DivExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

