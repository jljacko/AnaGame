#pragma once
#include "DoubleOpTag.h"
class DivExpressionTag : public DoubleOpTag
{
public:
	DivExpressionTag();
	~DivExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

