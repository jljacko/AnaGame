
#include "TArrayBase.h"

UCHAR TArrayType[] = { 1, 0b01000000 };

TArrayBase::TArrayBase()
{
}


TArrayBase::~TArrayBase()
{
}

UCHAR * TArrayBase::GetAnaGameType()
{
	return TArrayType;
}

/*
 * Method: TArrayBase - count
 * Purpose: Retrieves the number of elements in the TArray
 * Parameters: void
 * Returns: UINT - the current element count
 */
UINT TArrayBase::Count()
{
	return count;
}
