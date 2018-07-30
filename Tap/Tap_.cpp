#include "stdafx.h"
#include "Tap_.h"


Tap_::Tap_()
{
	currentPage = NULL;
	activeInput = NULL;
}


Tap_::~Tap_()
{
}

int Tap_::load(CArchive * header)
{

	return 0;
}

int Tap_::run()
{
	return 0;
}

bool Tap_::onClick(CPoint)
{
	return false;
}

bool Tap_::onDblClick(CPoint)
{
	return false;
}

bool Tap_::onChar(WCHAR)
{
	return false;
}

bool Tap_::onMouseMove(CPoint)
{
	return false;
}
