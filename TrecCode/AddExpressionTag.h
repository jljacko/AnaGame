#pragma once
#include "DoubleOpTag.h"
class AddExpressionTag :
	public DoubleOpTag
{
public:
	AddExpressionTag();
	~AddExpressionTag();

	virtual TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags) override;

	virtual void addAttribute(TString& att, TString& val) override;

protected:
	USHORT cases;
};

