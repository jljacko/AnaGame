#pragma once
#include "TBrush.h"
#include <TFileShell.h>
#include <wincodec.h>
class _VIDEO_GRAPHICS TBitmapBrush : public TBrush
{
	friend class TrecPointerKey;
public:
	void NextFrame();
	void PrevFrame();

	bool IsValid();
	void SetLocation(RECT_2D& loc);
	UINT GetCurrentFrame();
	UINT GetFrameCount();
	~TBitmapBrush();

	virtual void FillRectangle(const RECT_2D& r)override;
	virtual void FillRoundedRectangle(const ROUNDED_RECT_2D& r)override;
	virtual void FillEllipse(const ELLIPSE_2D& r)override;
	virtual void FillGeometry(TrecPointer<TGeometry> geo)override;

protected:
	TBitmapBrush(TrecPointer<TFileShell> picture, TrecPointer<DrawingBoard> rt, RECT_2D& loc);
	bool valid;

	TDataArray < TrecComPointer<ID2D1Bitmap>> bitmaps;
	TDataArray < TrecComPointer<ID2D1Brush>> brushes;
	UINT currentFrame;

	IWICImagingFactory* imageFactory;
	IWICBitmapDecoder* decoder;
	IWICBitmapFrameDecode* frameDec;
	IWICFormatConverter* converter;

	virtual void RefreshBrush()override;
	RECT_2D location;
};

