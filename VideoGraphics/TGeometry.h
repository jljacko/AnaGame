#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <TDataArray.h>
#include "Structure2D.h"
class _VIDEO_GRAPHICS TGeometry :
	public TObject
{
public:
	TGeometry(TrecComPointer<ID2D1Factory1> fact, TDataArray<POINT_2D>& points);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, RECT_2D& r);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, ROUNDED_RECT_2D& r);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, ELLIPSE_2D& r);


	bool IsValid()const;

	bool GetArea(float& res);
	bool GetParameter(float& res);
	bool IsInside(BOOL& res, POINT_2D&);
	bool GetBounds(RECT_2D& bounds);
	TrecComPointer<ID2D1Geometry> GetUnderlyingGeometry();

protected:
	TrecComPointer<ID2D1Geometry> geo;
	bool valid;
};

