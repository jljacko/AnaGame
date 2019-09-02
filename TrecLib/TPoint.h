#pragma once
#include "TObject.h"
#include "TrecLib.h"

class _TREC_LIB_DLL TPoint :
	public TObject
{
public:
	float x, y;

	TPoint();
	TPoint(float x, float y);
	TPoint(const TPoint& point);
	TPoint(const TPoint* point);

	TPoint operator=(const TPoint& point);
};

