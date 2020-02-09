#pragma once
#include "BNFTag.h"
class ConditionalBlockTag :
	public BNFTag
{
public:
	ConditionalBlockTag(TString&, BlockType mode, bool repeat, bool three, bool doWhile = false, bool negate = false);
	~ConditionalBlockTag();

	TagCheck ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

protected:
	UCHAR details;
	BlockType blockMode;
	bool isLoop();
	bool isThree();
	bool isDoWhile();
	bool isNegate();
	TagCheck ProcessRegular(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index);
	TagCheck ProcessAsThree(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index);
	TagCheck ProcessasDoWhile(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index);

};

