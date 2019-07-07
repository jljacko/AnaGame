#include "stdafx.h"
#include "BlockTag.h"

BlockTag::BlockTag(BlockType bt) : BNFTag(TString(L"block"))
{
	blockType = bt;
}

BlockTag::~BlockTag()
{
}

TagCheck BlockTag::ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	return TagCheck();
}
