#include "stdafx.h"
#include "ConditionalBlockTag.h"

ConditionalBlockTag::ConditionalBlockTag(TString& name, BlockType mode, bool repeat, bool three, bool doWhile, bool negate) :BNFTag(name)
{
	details = 0;
	if (repeat) details = 0b10000000;
	if (three) details = details | 0b01000000;
	if (negate) details = details | 0b00100000;
	if (doWhile) details = details | 0b00010000;
	blockMode = mode;
}

ConditionalBlockTag::~ConditionalBlockTag()
{

}

TagCheck ConditionalBlockTag::ProcessTag(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	TagCheck returnCheck(false, TString(), 0, nullptr);
	for (UINT c = 0; c < syntax.Size(); c++)
	{
		if (isThree())
			returnCheck = ProcessAsThree(bounds, codeStart, file, globalVariables, inter, lang, tags, c);
		else if(isDoWhile())
			returnCheck = ProcessasDoWhile(bounds, codeStart, file, globalVariables, inter, lang, tags, c);
		else returnCheck = ProcessRegular(bounds, codeStart, file, globalVariables, inter, lang, tags, c);

		if (returnCheck.success)
			return returnCheck;
	}
	return returnCheck;
}

bool ConditionalBlockTag::isLoop()
{
	return false;
}

bool ConditionalBlockTag::isThree()
{
	return false;
}

bool ConditionalBlockTag::isDoWhile()
{
	return false;
}

bool ConditionalBlockTag::isNegate()
{
	return false;
}

TagCheck ConditionalBlockTag::ProcessRegular(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index)
{
	auto curSyntax = &syntax[index];

	int expressionIndex = -1, blockIndex = -1;
	int expressionTagIndex = -1;
	for (UINT c = 0; c < curSyntax->Size(); c++)
	{
		if (curSyntax->at(c).isTag && tags[curSyntax->at(c).tagIndex]->GetTageName == TString(L"expression"))
		{
			expressionIndex = c;
			expressionTagIndex = curSyntax->at(c).tagIndex;
			break;
		}
	}
	if (expressionIndex == -1)
		return TagCheck(false, TString(L""), 0, nullptr);

	int blockTagIndex = -1;

	for (UINT c = 0; c < curSyntax->Size(); c++)
	{
		if (curSyntax->at(c).isTag && tags[curSyntax->at(c).tagIndex]->GetTageName == TString(L"block"))
		{
			blockIndex = c;
			blockTagIndex = curSyntax->at(c).tagIndex;
			break;
		}
	}
	if (blockIndex == -1)
		return TagCheck(false, TString(L""), 0, nullptr);

	int minExp = min(expressionIndex, blockIndex);
	int tokenLoc = -1;
	for (UINT c = 0; c < minExp; c++)
	{
		int ind = bounds.FindOutOfQuotes(curSyntax->at(c).mark, tokenLoc + 1);
		if (ind <= tokenLoc) {
			return TagCheck(false, TString(L""), 0, nullptr);
		}

		tokenLoc = ind;
	}

	UINT expressionStart, expressionEnd;
	if (tokenLoc == -1) {
		tokenLoc = expressionStart = 0;
	}
	else
	{
		expressionStart = tokenLoc + curSyntax->at(minExp - 1).mark.GetLength();
	}

	for (UINT c = minExp + 1; c < blockIndex; c++)
	{
		int ind = bounds.FindOutOfQuotes(curSyntax->at(c).mark, tokenLoc + 1);
		if (ind <= tokenLoc) {
			return TagCheck(false, TString(L""), 0, nullptr);
		}

		if (c == minExp)
			expressionEnd = ind;

		tokenLoc = ind;
	}

	UINT blockStart = tokenLoc + curSyntax->at(blockIndex - 1).mark.GetLength();

	auto blockTag = tags[blockTagIndex];
	
	TagCheck blockTagCheck = blockTag->ProcessTag(bounds, globalVariables, inter, lang, tags);

	if (!blockTagCheck.success)
		return blockTagCheck;

	TInterpretor* blockBody = dynamic_cast<TInterpretor*>(blockTagCheck.returnValue->value.object.get());

	if (!blockBody)
		return TagCheck(false, TString(L"Failure in retrieving Interpretor for conditional tag"), 0, nullptr);

	TString expBounds = bounds.SubString(expressionStart, expressionEnd);

	auto expressionTag = tags[expressionTagIndex];

	// Now make sure that if our conditional is an until or negative conditional, we addrees it as such (for perl users)
	bool negate = isNegate();

	while (true)
	{
		TagCheck expTag = expressionTag->ProcessTag(expBounds, expressionStart, file, globalVariables, inter, lang, tags);
		if (!expTag.success || !expTag.returnValue)
			return expTag;

		if (getVariablesBooleanValue(*expTag.returnValue) != negate)
		{
			TagCheck bodyCheck = (blockBody->Run(&inter));
			if (!bodyCheck.success)
				return bodyCheck;
		}
		else
			return expTag;

		if (!isLoop())
			return expTag;
	}
	

	return TagCheck(true, TString(), 0, nullptr);
}

TagCheck ConditionalBlockTag::ProcessAsThree(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index)
{
	return TagCheck();
}

TagCheck ConditionalBlockTag::ProcessasDoWhile(TString& bounds, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UCHAR index)
{
	auto curSyntax = &syntax[index];

	int expressionIndex = -1, blockIndex = -1;
	int expressionTagIndex = -1;
	for (UINT c = 0; c < curSyntax->Size(); c++)
	{
		if (curSyntax->at(c).isTag && tags[curSyntax->at(c).tagIndex]->GetTageName == TString(L"expression"))
		{
			expressionIndex = c;
			expressionTagIndex = curSyntax->at(c).tagIndex;
			break;
		}
	}
	if (expressionIndex == -1)
		return TagCheck(false, TString(L""), 0, nullptr);

	int blockTagIndex = -1;

	for (UINT c = 0; c < curSyntax->Size(); c++)
	{
		if (curSyntax->at(c).isTag && tags[curSyntax->at(c).tagIndex]->GetTageName == TString(L"block"))
		{
			blockIndex = c;
			blockTagIndex = curSyntax->at(c).tagIndex;
			break;
		}
	}
	if (blockIndex == -1)
		return TagCheck(false, TString(L""), 0, nullptr);

	int minExp = min(expressionIndex, blockIndex);
	int tokenLoc = -1;
	for (UINT c = 0; c < minExp; c++)
	{
		int ind = bounds.FindOutOfQuotes(curSyntax->at(c).mark, tokenLoc + 1);
		if (ind <= tokenLoc) {
			return TagCheck(false, TString(L""), 0, nullptr);
		}

		tokenLoc = ind;
	}
	
	UINT blockStart = tokenLoc + curSyntax->at(blockIndex - 1).mark.GetLength();

	auto blockTag = tags[blockTagIndex];

	TagCheck blockTagCheck = blockTag->ProcessTag(bounds, globalVariables, inter, lang, tags);

	if (!blockTagCheck.success)
		return blockTagCheck;

	TInterpretor* blockBody = dynamic_cast<TInterpretor*>(blockTagCheck.returnValue->value.object.get());

	if (!blockBody)
		return TagCheck(false, TString(L"Failure in retrieving Interpretor for conditional tag"), 0, nullptr);


	for (UINT c = minExp + 1; c < blockIndex; c++)
	{
		int ind = bounds.FindOutOfQuotes(curSyntax->at(c).mark, blockTagCheck.fileByteEnd);
		if (ind <= tokenLoc) {
			return TagCheck(false, TString(L""), 0, nullptr);
		}

		tokenLoc = ind;
	}

	



	TString expBounds = bounds.SubString(tokenLoc);

	auto expressionTag = tags[expressionTagIndex];

	// Now make sure that if our conditional is an until or negative conditional, we addrees it as such (for perl users)
	bool negate = isNegate();

	TagCheck expTag;
	do
	{
		TagCheck bodyCheck = (blockBody->Run(&inter));
		if (!bodyCheck.success)
			return bodyCheck;

		expTag = expressionTag->ProcessTag(expBounds, tokenLoc, file, globalVariables, inter, lang, tags);
		if (!expTag.success || !expTag.returnValue)
			return expTag;


	} while (getVariablesBooleanValue(*expTag.returnValue) != negate);


	return TagCheck(true, TString(), 0, nullptr);
}
