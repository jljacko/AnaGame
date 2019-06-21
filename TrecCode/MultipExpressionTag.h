#pragma once
#include "DoubleOpTag.h"
class MultipExpressionTag :	public DoubleOpTag
{
public:
	MultipExpressionTag();
	~MultipExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

