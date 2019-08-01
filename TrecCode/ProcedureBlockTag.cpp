#include "stdafx.h"
#include "ProcedureBlockTag.h"

ProcedureBlockTag::ProcedureBlockTag(TString& name, BlockType bt, bool isAsync, bool isMethod, bool isExpression) :BNFTag(name)
{
	blockType = bt;
	details = 0;
	if (isAsync) details = details | 0b10000000;
	if (isMethod) details = details | 0b01000000;
	if (isExpression) details = details | 0b00100000;
}

ProcedureBlockTag::~ProcedureBlockTag()
{
}

TagCheck ProcedureBlockTag::ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	
	return TagCheck();
}

TagCheck ProcedureBlockTag::ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index)
{
	int varTagIndex = -1, paramTagIndex = -1;


	return TagCheck();
}
