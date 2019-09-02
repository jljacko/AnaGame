#include "stdafx.h"
#include "ParseReader.h"


ParseReader::ParseReader(CArchive* ca, Parser* pa)
{
	reader = ca;
	respond = pa;
	fileActualReader = NULL;
	materials = true;
	supported = true;
}

ParseReader::~ParseReader()
{
}

bool ParseReader::read(int *)
{
	return false;
}
