#pragma once
#include <TString.h>
#include <TDataArray.h>


typedef struct parseResult
{
	TString exp;
	UCHAR expType;
	UINT begin, end;
}parseResult;

class IntLanguage
{
protected:
	IntLanguage();
	~IntLanguage();

public:

	static IntLanguage* getLanguage(TString& langName);

	UINT getBlockEnd(UINT blockBeginning);

protected:
	TString language;
	TString statementEnd;
	TString stringQuotes;
	TString mlStringQuotes;
	TDataArray<TString> mlCommentStart;
	TDataArray<TString> mlCommentEnd;
	TDataArray<TString> slComment;
};

