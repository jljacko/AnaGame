#pragma once
#include "TControl.h"
class TImage : public TControl
{
public:
	TImage(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> styles);
	TImage(TImage&);
	TImage();

	UINT onFill(CPoint& pixel, D2D1_COLOR_F& color, float tolerance);
	UINT setLinearGradientR(D2D1_COLOR_F& color1, D2D1_COLOR_F& color2, float radians, CPoint& point);
	UINT setLinearGradientD(D2D1_COLOR_F& color1, D2D1_COLOR_F& color2, float degrees, CPoint& point);

	UINT rotate(CPoint& point);
	virtual void onDraw(TObject* obj) override;

protected:
	D2D1_MATRIX_3X2_F rotation;
};

