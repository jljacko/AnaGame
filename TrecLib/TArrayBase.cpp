#include "stdafx.h"
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
