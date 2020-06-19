
#include "ParseReader_.h"

UCHAR ParseReaderType[] = { 2, 0b10000000, 3 };


/*
* Method: ParseReader_::ParseReader_
* Purpose: Constructor
* Parameters: TFile* tf - the TFile to use
*			Parser_* par - the parser to call upon
* Returns: void
*/
ParseReader_::ParseReader_(TFile *tf, Parser_ *par)
{
	tReader = tf;
	respond = par;
	if (!tReader || !respond)
		materials = false;
	else
		materials = true;
	usingTFile = true;
}

/*
* Method: ParseReader_::~ParseReader_
* Purpose: Destructor
* Parameters: void
* Returns: void
*/
ParseReader_::~ParseReader_()
{
}

/*
* Method: ParseReader_::read
* Purpose: Reads the file in a predefined format
* Parameters: int* - the line number an error occurs
* Returns: bool - success
*/
bool ParseReader_::read(int *)
{
	return false;
}

/*
* Method: ParseReader_::GetAnaGameType
* Purpose: Retieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type ID format
*
* Note: DEPRICATED
*/
UCHAR * ParseReader_::GetAnaGameType()
{
	return ParseReaderType;
}
