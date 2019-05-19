#pragma once
#include <TObject.h>
#include <TString.h>
#include <TDataArray.h>
#include <TFile.h>
#include "TInterpretor.h"
#include "VariableContainer.h"

class TagCheck: public TObject
{
public:
	TagCheck();
	TagCheck(bool s, TString& e, UINT end, intVariable* iv);
	bool success;		// Was this an acceptable tag?
	TString error;		// If not successful, why was that
	UINT fileByteEnd;	// If successful, where does it end?
	intVariable* returnValue; // If the Tag was meant to return something
};

class TagMark: public TObject
{
public:
	TagMark();
	TagMark(bool it, TString& m, short ti);
	bool isTag;
	TString mark;
	short tagIndex;
};

class ExpressionProcess: public TObject
{
public:
	TString code;
	UINT tagProcess;
};

class ProcessedCode: public TObject
{
public:
	bool worked;
	TDataArray<ExpressionProcess> expressions;
};

class BNFTag :	public TObject
{
protected:
	BNFTag(TString& tagName);
	~BNFTag();

public:
	static BNFTag* GetTag(TString& tagName);
	static BNFTag* GetFunctionalTag(TString& tagName);

	void SetSyntaxString(TString& s);
	UINT CompileTag(TDataArray<BNFTag*>& tagList);

	virtual TagCheck ProcessTag(UINT statementStart, UINT tagStart, TFile& file, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags, UINT end = 0);
	virtual TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags);

	virtual TagCheck ProcessTag(TString& code, UINT codeStart, TFile& file, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags);

	virtual void addAttribute(TString& att, TString& val);

	TString GetTageName();

protected:
	TString name;
	TString rawSyntax;
	TDataArray<TDataArray<TagMark>> syntax;

	short findTagIndex(TDataArray<BNFTag*>&, TString& token);

	UINT CountRawTags(TDataArray<TagMark>& tags);
	int GetIndexOfToken(TString& code, TDataArray<TagMark>& tags, UINT token, UINT start = 0);
	ProcessedCode PreProcessLine(TString& code, UINT syntaxIndex);

};


TDataArray<BNFTag*>* setUpTagList(TFile& file);

UINT CompileIntLanguage(TDataArray<BNFTag*>& tags);

