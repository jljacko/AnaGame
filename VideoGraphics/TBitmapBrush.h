#pragma once
#include "TBrush.h"
#include <TFileShell.h>
#include <wincodec.h>
class TBitmapBrush : public TBrush
{
public:
	void NextFrame();
	void PrevFrame();

	bool IsValid();
	void SetLocation(RECT_2D& loc);
	UINT GetCurrentFrame();
	~TBitmapBrush();

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

