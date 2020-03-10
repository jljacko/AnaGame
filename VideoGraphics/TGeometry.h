#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <TDataArray.h>
#include "Structure2D.h"

typedef enum geo_type
{
	geo_type_rect,
	geo_type_rounded_rect,
	geo_type_ellipse,
	geo_type_path
}geo_type;

class _VIDEO_GRAPHICS TGeometry :
	public TObject
{
public:
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const TDataArray<POINT_2D>& points);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const RECT_2D& r);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const ROUNDED_RECT_2D& r);
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const ELLIPSE_2D& r);

	virtual ~TGeometry();


	bool IsValid()const;

	bool GetArea(float& res);
	bool GetParameter(float& res);
	bool IsInside(BOOL& res, POINT_2D&);
	bool GetBounds(RECT_2D& bounds);
	TrecComPointer<ID2D1Geometry> GetUnderlyingGeometry();

protected:
	TrecComPointer<ID2D1Geometry> geo;
	bool valid;
	geo_type geoType;
};

