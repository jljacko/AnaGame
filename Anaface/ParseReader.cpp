
#include "ParseReader.h"


ParseReader::ParseReader(TFile* ca, Parser* pa)
{
	reader = ca;
	respond = pa;
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
