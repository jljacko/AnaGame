#include "TImage.h"

static D2D1_MATRIX_3X2_F identityMatrix = D2D1::Matrix3x2F::Identity();

TImage::TImage(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> styles): TControl(rt, styles, false)
{
	rotation = identityMatrix;
}

TImage::TImage(TImage& im) : TControl(im)
{
	rotation = identityMatrix;
}

TImage::TImage() :TControl()
{
	rotation = identityMatrix;
}

UINT TImage::onFill(CPoint& pixel, D2D1_COLOR_F& color, float tolerance)
{
	return 0;
}

UINT TImage::setLinearGradientR(D2D1_COLOR_F& color1, D2D1_COLOR_F& color2, float radians, CPoint& point)
{
	return 0;
}

UINT TImage::setLinearGradientD(D2D1_COLOR_F& color1, D2D1_COLOR_F& color2, float degrees, CPoint& point)
{
	return 0;
}

UINT TImage::rotate(CPoint& point)
{
	CPoint center((location.left + location.right) / 2, (location.bottom + location.top) / 2);
	double degrees = atan2(-(static_cast<double>(point.y) - center.y),
		(static_cast<double>(point.x) - center.y));
	rotation = D2D1::Matrix3x2F::Rotation(degrees,D2D1::Point2F(center.x, center.y));
	return 0;
}

void TImage::onDraw(TObject* obj)
{
	if (!renderTarget.get()) return;
	renderTarget->SetTransform(rotation);
	TControl::onDraw(obj);
	renderTarget->SetTransform(identityMatrix);
}
