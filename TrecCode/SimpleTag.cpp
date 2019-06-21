#include "stdafx.h"
#include "SimpleTag.h"


SimpleTag::SimpleTag() : BNFTag(TString(L"simple_statement"))
{
	equateGreekQandSemiC = true;
	rawSyntax = L"<small_statement>";
}


SimpleTag::~SimpleTag()
{
}

TagCheck SimpleTag::ProcessTag(UINT statementStart, UINT tagStart, TFile & file, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags, UINT end)
{
	return TagCheck();
}

TagCheck SimpleTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	int split = -1;

	for (UINT c = 0; c < enders.GetLength(); c++)
	{
		int sp = bounds.Find(enders[c]);
		if (split == -1 || sp < split)
			split = sp;
	}

	TagCheck ret;
	ret.fileByteEnd = 0;
	ret.returnValue = nullptr;
	ret.success = true;

	if (split == -1)
	{
		ret.error = L"No Stateent Terminator: valid terminators include '" + enders + L"'";
		ret.success = false;
		return ret;
	}

	TString smallStatement = bounds.SubString(0, split);

	if (!syntax.Size() || !syntax[0].Size())
	{
		ret.error = L"No Syntax available. Did you compile the language before running it?";
		ret.success = false;
		return ret;
	}

	if (syntax[0][0].tagIndex < 0)
	{
		ret.error = L"Statement could not be run. Make sure a <small_statement> syntax rule is included in the language's AG_BNF file?";
		ret.success = false;
		return ret;
	}

	try
	{
		return tags[syntax[0][0].tagIndex]->ProcessTag(smallStatement, globalVariables, inter, lang, tags);
	}
	catch (TString& t)
	{
		ret.error = t + TString(L"small_statement tag index exceeds tag collection");
		ret.success = false;
		return ret;
	}
	return TagCheck();
}

void SimpleTag::addAttribute(TString & att, TString & val)
{
	if (att == TString(L"GreekEqualsSemi"))
	{
		if (val.MakeLower() == TString(L"false"))
			equateGreekQandSemiC = false;
	}

	if (att == TString(L"StatementTerminator"))
		enders = val;


	// Save programmers the frustration of dealing with a Greek Question Mark 
	if (equateGreekQandSemiC && val.Find(L';') != -1 && val.Find(0x037E) == -1)
		enders.AppendChar(0x037E);
}
