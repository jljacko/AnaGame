#pragma once
#include "DoubleOpTag.h"
class AndExpression : public DoubleOpTag
{
public:
	AndExpression();
	~AndExpression();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

