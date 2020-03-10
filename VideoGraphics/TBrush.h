#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include "Structure2D.h"
#include "TGradientStopCollection.h"

class DrawingBoard;
class TColor;
class TGeometry;

typedef enum brush_type
{
	brush_type_solid,
	brush_type_linear,
	brush_type_radial,
	brush_type_bitmap
}brush_type;

class _VIDEO_GRAPHICS TBrush : public TObject
{
	friend class TrecPointerKey;
	friend class DrawingBoard;
public:
	void DrawRectangle(const RECT_2D& r, float thickness = 1.0f);
	virtual void FillRectangle(const RECT_2D& r);
	void DrawRoundedRectangle(const ROUNDED_RECT_2D& r, float thickness = 1.0f);
	virtual void FillRoundedRectangle(const ROUNDED_RECT_2D& r);
	void DrawEllipse(const ELLIPSE_2D& r, float thickness = 1.0f);
	virtual void FillEllipse(const ELLIPSE_2D& r);
	void DrawGeometry(TrecPointer<TGeometry> geo, float thickness = 1.0f);
	virtual void FillGeometry(TrecPointer<TGeometry> geo);

	void DrawLine(const POINT_2D& p1, const POINT_2D& p2, float thickness = 1.0f);
	
	UINT GetMaxColors();

	void SetColor(const TColor& color, UINT index = 0);
	TColor GetColor(UINT index = 0);

	brush_type GetBrushType();
	TrecComPointer<ID2D1Brush> GetUnderlyingBrush();

	virtual ~TBrush();

protected:
	brush_type brushType;

	TBrush(const TColor& col, TrecPointer<DrawingBoard> rt);
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2, float x, float y);
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2);

	TBrush(TrecPointer<DrawingBoard> rt);

	bool Refresh();
	virtual void RefreshBrush();


	TrecComPointer<ID2D1Brush> brush;
	TrecComPointer<ID2D1RenderTarget> currentRenderer;
	TrecPointer<DrawingBoard> board;

	TGradientStopCollection gradients;
};

