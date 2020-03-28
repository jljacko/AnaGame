#include "TBrush.h"
#include "DrawingBoard.h"
#include "TGeometry.h"
#include <atltrace.h>

void TBrush::DrawRectangle(const RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRectangle(r, brush.Get(), thickness);
	}
}

void TBrush::FillRectangle(const RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRectangle(r, brush.Get());
	}
}

void TBrush::DrawRoundedRectangle(const ROUNDED_RECT_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawRoundedRectangle(r, brush.Get(), thickness);
	}
}

void TBrush::FillRoundedRectangle(const ROUNDED_RECT_2D& r)
{
	if (Refresh())
	{
		currentRenderer->FillRoundedRectangle(r, brush.Get());
	}
}

void TBrush::DrawEllipse(const ELLIPSE_2D& r, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawEllipse(r, brush.Get(), thickness);
	}
}

void TBrush::FillEllipse(const ELLIPSE_2D& r)
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

void TBrush::DrawLine(const POINT_2D& p1, const POINT_2D& p2, float thickness)
{
	if (Refresh())
	{
		currentRenderer->DrawLine(p1, p2, brush.Get(), thickness);
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
	switch (brushType)
	{
	case brush_type_solid:
		ATLTRACE(L"SOLID BRUSH TYPE\n");
		break;
	case brush_type_linear:
		ATLTRACE(L"LINEAR BRUSH TYPE\n");
		break;
	case brush_type_radial:
		ATLTRACE(L"RADIAL BRUSH TYPE\n");
		break;
	case brush_type_bitmap:
		ATLTRACE(L"BITMAP BRUSH TYPE\n");
		break;
	default:
		ATLTRACE(L"OTHER BRUSH TYPE\n");
	}


	if (brushType == brush_type_solid)
	{
		
		if (brush.Get())
		{
			reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->SetColor(color.GetColor());
			ATLTRACE(L"Set color!");
		}
		else
			ATLTRACE(L"COLD NOT GET BRUSH!");
	}
	else
	{
		gradients.SetColorAt(color, index);
	}
}

TColor TBrush::GetColor(UINT index)
{
	if (brushType == brush_type_solid)
	{
		TColor col;
		if (brush.Get())
			col.SetColor(reinterpret_cast<ID2D1SolidColorBrush*>(brush.Get())->GetColor());
		return col;
	}
	return gradients.GetColorAt(index);
}

brush_type TBrush::GetBrushType()
{
	return brushType;
}

TrecComPointer<ID2D1Brush> TBrush::GetUnderlyingBrush()
{
	if(Refresh())
		return brush;
	return TrecComPointer<ID2D1Brush>();
}

TBrush::~TBrush()
{
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

	gradients.AddGradient(TGradientStop(col, 0.0f));
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

TBrush::TBrush(TrecPointer<DrawingBoard> rt)
{
	if (!rt.Get())
		throw L"ERROR! Needed Render Target to Not Be Null";

	currentRenderer = rt->GetRenderer();

	if (!currentRenderer.Get())
		throw L"Error! Drawing Board had a NULL Render Target";

	board = rt;
}

bool TBrush::Refresh()
{
	if (!board.Get() || !brush.Get())
		return false;

	if (currentRenderer.Get() && currentRenderer.Get() == board->GetRenderer().Get())
		return true;
	currentRenderer = board->GetRenderer();
	RefreshBrush();
	return currentRenderer.Get() != nullptr;
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

