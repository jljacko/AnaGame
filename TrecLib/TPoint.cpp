#include "TPoint.h"

TPoint::TPoint()
{
	x = y = 0.0f;
}

TPoint::TPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

TPoint::TPoint(const TPoint& point)
{
	x = point.x;
	y = point.y;
}

TPoint::TPoint(const TPoint* point)
{
	if (point)
	{
		x = point->x;
		y = point->y;
	}
	else
		x = y = 0.0f;
}

TPoint TPoint::operator=(const TPoint& point)
{
	x = point.x;
	y = point.y;
	return *this;
}
