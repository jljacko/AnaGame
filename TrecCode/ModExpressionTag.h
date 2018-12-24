#pragma once
#include "DoubleOpTag.h"
class ModExpressionTag :public DoubleOpTag
{
public:
	ModExpressionTag();
	~ModExpressionTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

