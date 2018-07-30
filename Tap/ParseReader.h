#pragma once
#include "stdafx.h"
#include "Parser.h"
class _TAP_DLL ParseReader
{
public:
	ParseReader(CArchive*, Parser*);
	~ParseReader();

	virtual bool read(int*);

protected:
	CArchive* reader;
	Parser* respond;
	//CStdioFile* fileActualReader;
	bool supported;
	bool materials;
};