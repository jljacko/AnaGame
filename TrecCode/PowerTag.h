#pragma once
#include "DoubleOpTag.h"
class PowerTag : public DoubleOpTag
{
public:
	PowerTag();
	~PowerTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

