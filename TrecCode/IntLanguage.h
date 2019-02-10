#pragma once

#include <TString.h>
#include <TDataArray.h>
#include "BNFTag.h"
#include "TrecCode.h"

class _TREC_CODE_DLL IntLanguage
{
	friend class TInterpretor;
public:

	static IntLanguage* getLanguage(TString& langName);
	
protected:
	IntLanguage();
	~IntLanguage();

	TDataArray<BNFTag*>* tagList;


	TString language;
	TString statementEnd;
	TString singleComment;				// Comments that end when the next line begins
	TString startComment, endComment;   // Multi-line comments;
	TString blockMarks;					// How TrecCode know how a block is defined
	TDataArray<TString> string, multiLineString;
};

