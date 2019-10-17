#pragma once
#include "stdafx.h"
#include "Parser.h"
class _ANAFACE_DLL ParseReader
{
public:
	ParseReader(CArchive*, Parser*);
	~ParseReader();

	bool read(int*);

protected:
	CArchive* reader;
	Parser* respond;
	CStdioFile* fileActualReader;
	bool supported;
	bool materials;
};

