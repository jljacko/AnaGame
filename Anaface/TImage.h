#pragma once
#include "TControl.h"

typedef struct TBitmap {
	IWICBitmap* wicBitmap;
	ID2D1Bitmap* d2dBitbap;
}TBitmap;

typedef struct PixelMark {
	UINT pixel;
	BYTE marker;
}PixelMarker;

class TImage : public TControl
{
public:
	TImage(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> styles);
	TImage(TImage&);
	TImage();
	~TImage();

	UINT onFill(TPoint& pixel, D2D1_COLOR_F& color, float tolerance);
	UINT setLinearGradient(TDataArray<D2D1_COLOR_F>& colors);
	UINT setLinearGradient(TDataArray<D2D1_GRADIENT_STOP>& colors);

	UINT setRadialGradient(TDataArray<D2D1_COLOR_F>& colors);
	UINT setRadialGradient(TDataArray<D2D1_GRADIENT_STOP>& colors);

	UINT rotate(TPoint& point);
	virtual void onDraw(TObject* obj) override;

	virtual void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>);

protected:
	D2D1_MATRIX_3X2_F rotation;
	TrecPointer<TContent> cover;
	TDataArray<TBitmap> images;
	UINT currentImage;
};

void MarkPixels(TPoint& startPoint, UINT idealPixel, float tolerance, TDataArray<TDataArray<PixelMark>>& pixels);
void ConvertD2D1ColorToUIntColor(D2D1_COLOR_F& d2dColor, UINT& uColor);