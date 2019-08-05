#pragma once
#include <TObject.h>
#include <TString.h>
#include <TDataArray.h>
#include <TFile.h>
#include "TInterpretor.h"
#include "VariableContainer.h"
#include <Logger.h>

typedef enum BlockType
{
	block_tokens,
	block_indent,
	block_curly
}BlockType;

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
	static BNFTag* GetFunctionalTag(TString& tagName, BlockType bt);

	void SetSyntaxString(TString& s);
	UINT CompileTag(TDataArray<BNFTag*>& tagList);

	virtual TagCheck ProcessTag(UINT statementStart, UINT tagStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, TDataArray<BNFTag*>& tags, IntLanguage& lang, UINT end = 0);
	virtual TagCheck ProcessTag(TString& bounds, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

	virtual TagCheck ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags);

	virtual void addAttribute(TString& att, TString& val);

	TString GetTageName();

	static int GetIndexOfTag(TString& name, TDataArray<BNFTag*>& tags);

protected:
	TString name;
	TString rawSyntax;
	TDataArray<TDataArray<TagMark>> syntax;

	short findTagIndex(TDataArray<BNFTag*>&, TString& token);

	virtual TagCheck ProcessTag(TString& code, UINT codeStart, TrecPointer<TFile> file, VariableContainer& globalVariables, TInterpretor& inter, IntLanguage& lang, TDataArray<BNFTag*>& tags, UINT syntaxLevel, UINT syntaxStart);

	UINT CountRawTags(TDataArray<TagMark>& tags);
	int GetIndexOfToken(TString& code, TDataArray<TagMark>& tags, UINT token, UINT start = 0);
	ProcessedCode PreProcessLine(TString& code, UINT syntaxIndex);

};


TDataArray<BNFTag*>* setUpTagList(TFile& file, BlockType bt);

UINT CompileIntLanguage(TDataArray<BNFTag*>& tags);

bool getVariablesBooleanValue(intVariable& var);