#include "TGeometry.h"

TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, TDataArray<POINT_2D>& points)
{
	valid = false;
	if (!fact.Get())
		return;
	   
	TrecComPointer<ID2D1PathGeometry>::TrecComHolder pathHolder;
	fact->CreatePathGeometry(pathHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1PathGeometry>(pathHolder);


	ID2D1GeometrySink* sink = nullptr;

	if (FAILED(reinterpret_cast<ID2D1PathGeometry*>(geo.Get())->Open(&sink)) || points.Size() < 2)
		return;

	sink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);

	sink->AddLines(&points[1], points.Size() - 1);

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	sink->Close();

	sink->Release();
	valid = true;
	//geoType = geo_type_path;
}

TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, RECT_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1RectangleGeometry>::TrecComHolder rectHolder;
	fact->CreateRectangleGeometry(r, rectHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1RectangleGeometry>(rectHolder);
	valid = true;
	//geoType = geo_type_rect;
}

TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, ROUNDED_RECT_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1RoundedRectangleGeometry>::TrecComHolder rectHolder;
	fact->CreateRoundedRectangleGeometry(r, rectHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1RoundedRectangleGeometry>(rectHolder);
	valid = true;
	//geoType
}

TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, ELLIPSE_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1EllipseGeometry>::TrecComHolder circleHolder;

	fact->CreateEllipseGeometry(r, circleHolder.GetPointerAddress());
	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1EllipseGeometry>(circleHolder);
	valid = true;
}

TGeometry::~TGeometry()
{
	geo.Nullify();
}

bool TGeometry::IsValid() const
{
	return valid;
}

bool TGeometry::GetArea(float& res)
{
	if(!geo.Get())
		return false;
	return SUCCEEDED(geo->ComputeArea(nullptr, &res));
}

bool TGeometry::GetParameter(float& res)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->ComputeLength(nullptr, &res));
}

bool TGeometry::IsInside(BOOL& res, POINT_2D& p)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->FillContainsPoint(p,nullptr, &res));
}

bool TGeometry::GetBounds(RECT_2D& bounds)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->GetBounds(nullptr, &bounds));
}

TrecComPointer<ID2D1Geometry> TGeometry::GetUnderlyingGeometry()
{
	return geo;
}
