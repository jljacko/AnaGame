#pragma once
#include "stdafx.h"
#include "Parser_.h"
#include "TObject.h"
#include "TFile.h"

/*
* class ParseReader_
* Base Class for all Markup-language parsers
*/
class _TREC_LIB_DLL ParseReader_ : public TObject
{
public:
	ParseReader_(CArchive*, Parser_*);
	ParseReader_(TFile*, Parser_*);
	~ParseReader_();

	virtual bool read(int*);

	virtual UCHAR* GetAnaGameType();

protected:
	CArchive* reader;
	TFile* tReader;
	Parser_* respond;
	//CStdioFile* fileActualReader;
	//bool supported;
	bool materials;
	bool usingTFile;
};