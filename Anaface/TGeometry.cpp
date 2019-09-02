
#include "TGeometry.h"


TGeometry::TGeometry()
{
	thickness = 1.0f;
	fill = false;
}

TGeometry::TGeometry(bool fill)
{
	this->fill = fill;
	thickness = 1.0f;
}


TGeometry::~TGeometry()
{
}

void TGeometry::setPoint(D2D1_POINT_2F, int)
{
}

void TGeometry::setWidth(int)
{
}

void TGeometry::setHeight(int)
{
}

void TGeometry::setClosure(bool)
{
}

D2D1_POINT_2F TGeometry::getPoint(int)
{
	return D2D1_POINT_2F();
}

int TGeometry::getHeight()
{
	return 0;
}

int TGeometry::getWidth()
{
	return 0;
}

void TGeometry::onCreate(RECT, bool)
{
}

bool TGeometry::getClosure()
{
	return false;
}

void TGeometry::onDraw(ID2D1Brush*, ID2D1DCRenderTarget*)
{
}

void TGeometry::setFill(bool f)
{
	fill = f;
}

float TGeometry::getThickness()
{
	return thickness;
}

void TGeometry::setThickness(float f)
{
	thickness = f;
}
