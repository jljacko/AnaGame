#include "stdafx.h"
#include "ParseReader_.h"

UCHAR ParseReaderType[] = { 2, 0b10000000, 3 };

/*
* Method: (ParseReader_) (Constructor)
* Purpose: Sets up the parser to use
* Parameters: CArchive* car - the CArchive to use
*			Parser_* par - the parser to call upon
* Returns: void
* Note: Could be depreciated - CArchive is unique to MFC and CArchive could be replaced with TFile
*/
ParseReader_::ParseReader_(CArchive* car, Parser_* par)
{
	reader = car;
	respond = par;
	//fileActualReader = new CStdioFile();

	if (!car || !par)
		materials = false;
	else
		materials = true;
	usingTFile = false;
	tReader = nullptr;
}

/*
* Method: (ParseReader_) (Constructor)
* Purpose: Sets up the parser to use, using Anagame's built in TFile
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
	reader = nullptr;
}

/*
* Method: (ParseReader_) (Destructor)
* Purpose: Cleans up the parser
* Parameters: void
* Returns: void
*/
ParseReader_::~ParseReader_()
{
}

/*
* Method: ParseReader_ - read
* Purpose: Reads the file in a predefined format
* Parameters: int* - the line number an error occurs
* Returns: bool - success
*/
bool ParseReader_::read(int *)
{
	return false;
}

/*
* Method: ParseReader_ - GetAnaGameType
* Purpose: Retieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type ID format
*/
UCHAR * ParseReader_::GetAnaGameType()
{
	return ParseReaderType;
}
