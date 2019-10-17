#pragma once
#include "ParseReader_.h"

/*
* Class: TML_Reader_
* Reads TML Files
*/
class _TREC_LIB_DLL TML_Reader_ :
	public ParseReader_
{
public:

	TML_Reader_(TFile*, Parser_*);
	~TML_Reader_();

	bool read(int*);

private:
	bool ReadString(TString& line);
};