#pragma once
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
	ParseReader_(TFile*, Parser_*);
	~ParseReader_();

	virtual bool read(int*);

	virtual UCHAR* GetAnaGameType();

protected:
	TFile* tReader;
	Parser_* respond;
	//CStdioFile* fileActualReader;
	//bool supported;
	bool materials;
	bool usingTFile;
};