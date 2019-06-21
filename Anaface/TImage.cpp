#include "TImage.h"

TImage::TImage(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> styles): TControl(rt, styles, false)
{
}

TImage::TImage(TImage& im) : TControl(im)
{
}

TImage::TImage() :TControl()
{
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
	return 0;
}
