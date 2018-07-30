#include "stdafx.h"
#include "TDataArrayBase.h"
UCHAR TDataArrayType[] = { 1, 0b00100000 };

TDataArrayBase::TDataArrayBase()
{
}


TDataArrayBase::~TDataArrayBase()
{
}

/*
* Method: TDataArrayBase - GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type represenation
*/
UCHAR * TDataArrayBase::GetAnaGameType()
{
	return TDataArrayType;
}