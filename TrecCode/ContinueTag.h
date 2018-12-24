#pragma once
#include "BNFTag.h"
class ContinueTag :	public BNFTag
{
public:
	ContinueTag();
	~ContinueTag();

	TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags) override;
};

