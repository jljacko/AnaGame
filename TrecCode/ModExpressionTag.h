#pragma once
#include "DoubleOpTag.h"
class ModExpressionTag :public DoubleOpTag
{
public:
	ModExpressionTag();
	~ModExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;
};

