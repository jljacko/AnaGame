#pragma once
#include "DoubleOpTag.h"
class DivExpressionTag : public DoubleOpTag
{
public:
	DivExpressionTag();
	~DivExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

