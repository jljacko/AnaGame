#include "TBrush.h"
#include "DrawingBoard.h"
#include "TGeometry.h"

void TBrush::DrawRectangle(RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRectangle(r, brush.Get(), thickness);
	}
}

void TBrush::FillRectangle(RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRectangle(r, brush.Get());
	}
}

void TBrush::DrawRoundedRectangle(ROUNDED_RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRoundedRectangle(r, brush.Get(), thickness);
	}
}

void TBrush::FillRoundedRectangle(ROUNDED_RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRoundedRectangle(r, brush.Get());
	}
}

void TBrush::DrawEllipse(ELLIPSE_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawEllipse(r, brush.Get(), thickness);
	}
}

void TBrush::FillEllipse(ELLIPSE_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillEllipse(r, brush.Get());
	}
}

void TBrush::DrawGeometry(TrecPointer<TGeometry> geo, float thickness)
{
	if (Refresh() && geo.Get() && geo->IsValid())
	{
		currentRenderer->DrawGeometry(geo->GetUnderlyingGeometry().Get(), brush.Get(), thickness);
	}
}

void TBrush::FillGeometry(TrecPointer<TGeometry> geo)
{
	if (Refresh() && geo.Get() && geo->IsValid())
	{
		currentRenderer->FillGeometry(geo->GetUnderlyingGeometry().Get(), brush.Get());
	}
}

UINT TBrush::GetMaxColors()
{
	if (brushType == brush_type_solid)
		return 1;
	return gradients.GetGradientCount();
}

void TBrush::SetColor(const TColor& color, UINT index)
{
	if (brushType == brush_type_solid)
	{
		if (brush.Get())
			reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->SetColor(color.GetColor());
	}
	else
		gradients.SetColorAt(color, index);
}

brush_type TBrush::GetBrushType()
{
	return brushType;
}

TBrush::TBrush(const TColor& col, TrecPointer<DrawingBoard> rt)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder brushHolder;

	currentRenderer->CreateSolidColorBrush(col.GetColor(), brushHolder.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(brushHolder);
	brushType = brush_type_solid;
}

TBrush::TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2, float x, float y)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
	gradients = coll;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
	currentRenderer->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
		p1,
		p2,
		x, y),
		stopColl, radBrush.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);

	stopColl->Release();
	stopColl = nullptr;
	brushType = brush_type_radial;
}

TBrush::TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();
	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
	gradients = coll;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	currentRenderer->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		p1,
		p2),
		stopColl, linBrush.GetPointerAddress());

	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);

	stopColl->Release();
	stopColl = nullptr;
	brushType = brush_type_linear;
}

bool TBrush::Refresh()
{
	if (!currentRenderer.Get() || !board.Get())
		return false;

	if (currentRenderer.Get() == board->GetRenderer().Get())
		return true;
	RefreshBrush();
	return true;
}

void TBrush::RefreshBrush()
{
	if (!currentRenderer.Get() || !brush.Get())
		return;
	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder brushHolder;
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;

	POINT_2D p1, p2;
	float x, y;
	TColor col;

	ID2D1GradientStopCollection* stopColl = nullptr;
	auto rawGradients = gradients.GetRawCollection();
	currentRenderer->CreateGradientStopCollection(rawGradients.data(), rawGradients.Size(), &stopColl);

	switch (brushType)
	{
	case brush_type_solid:
		col.SetColor(reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->GetColor());
		brush.Delete();
		currentRenderer->CreateSolidColorBrush(col.GetColor(), brushHolder.GetPointerAddress());

		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(brushHolder);
		break;
	case brush_type_linear:
		p1 = reinterpret_cast<ID2D1LinearGradientBrush*>(brush.Get())->GetStartPoint();
		p2 = reinterpret_cast<ID2D1LinearGradientBrush*>(brush.Get())->GetEndPoint();
		brush.Delete();
		currentRenderer->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
			p1,
			p2),
			stopColl, linBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		break;
	case brush_type_radial:
		p1 = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetCenter();
		p2 = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetGradientOriginOffset();
		x = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetRadiusX();
		y = reinterpret_cast<ID2D1RadialGradientBrush*>(brush.Get())->GetRadiusY();

		brush.Delete();
		TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
		currentRenderer->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
			p1,
			p2,
			x, y),
			stopColl, radBrush.GetPointerAddress());

		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
	}
	stopColl->Release();
	stopColl = nullptr;


}

