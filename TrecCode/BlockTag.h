#pragma once
#include "BNFTag.h"
class BlockTag :
	public BNFTag
{
public:
	BlockTag(BlockType bt);
	~BlockTag();

	TagCheck ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

protected:
	BlockType blockType;
};

