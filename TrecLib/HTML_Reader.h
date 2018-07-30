#pragma once
#include "ParseReader_.h"


/*
* Class: HTML_Reader
* Parser Reader optimized for HTML files
*/
class _TREC_LIB_DLL HTML_Reader :	public ParseReader_
{
	friend class HTMLParser;
public:
	HTML_Reader(CArchive*, Parser_*);
	HTML_Reader(TFile*, Parser_*);
	~HTML_Reader();

	bool read(int*)override;

	virtual UCHAR* GetAnaGameType();

private:
	bool charDeduced;
	bool usingWide;
	void DeduceCharType(unsigned char*);
	WCHAR ReturnWCharType(char);
	bool isWhiteSpace(WCHAR);
	bool DeduceToken(TString& t);
	bool openTaken;
	CArchive* GetArchive();
	void SubmitToken(TString);

	bool parseQuoteTokens(TrecPointer<TArray<TString>>& tokens);

	void endContentMode();
	bool contentMode;
	bool contentSpace;
	TrecPointer<TString> contentS;
};

