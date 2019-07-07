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

TagCheck ConditionalBlockTag::ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	return TagCheck();
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
