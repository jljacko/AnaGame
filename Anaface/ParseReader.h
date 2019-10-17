#pragma once

#include "Parser.h"
class _ANAFACE_DLL ParseReader
{
public:
	ParseReader(TFile*, Parser*);
	~ParseReader();

	bool read(int*);

protected:
	TFile* reader;
	Parser* respond;
	bool supported;
	bool materials;
};

