#pragma once
#include "BNFTag.h"
class ProcedureBlockTag :
	public BNFTag
{
public:
	ProcedureBlockTag(TString& name, BlockType bt, bool isAsync, bool isMethod, bool isExpression);
	~ProcedureBlockTag();

	TagCheck ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

protected:
	/*
		7 = async (1 if true)
		6 = method
		5 = isExpression
	*/
	UCHAR details;
	BlockType blockType;

	TagCheck ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index);
};

