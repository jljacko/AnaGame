#pragma once
#include "BNFTag.h"

/**
* Class: SimpleTag
* Purpose: Implements the Simple Statement tag, responsible for finding simple statements, which either end in a new line or a semi-colon
*/
class SimpleTag :	public BNFTag
{
public:
	SimpleTag();
	~SimpleTag();

	virtual TagCheck ProcessTag(UINT statementStart, UINT tagStart, TFile& file, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags, UINT end = 0);
	virtual TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

	virtual void addAttribute(TString& att, TString& val);

protected:
	TString enders;
	bool equateGreekQandSemiC;
};

