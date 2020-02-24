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

class TBrush : public TObject
{
	friend class TrecPointerKey;
	friend class DrawingBoard;
public:
	void DrawRectangle(RECT_2D& r, float thickness = 1.0f);
	void FillRectangle(RECT_2D& r);
	void DrawRoundedRectangle(ROUNDED_RECT_2D& r, float thickness = 1.0f);
	void FillRoundedRectangle(ROUNDED_RECT_2D& r);
	void DrawEllipse(ELLIPSE_2D& r, float thickness = 1.0f);
	void FillEllipse(ELLIPSE_2D& r);
	void DrawGeometry(TrecPointer<TGeometry> geo, float thickness = 1.0f);
	void FillGeometry(TrecPointer<TGeometry> geo);
	
	UINT GetMaxColors();

	void SetColor(const TColor& color, UINT index = 0);

	brush_type GetBrushType();

protected:
	brush_type brushType;

	TBrush(const TColor& col, TrecPointer<DrawingBoard> rt);
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2, float x, float y);
	TBrush(const TGradientStopCollection& coll, TrecPointer<DrawingBoard> rt, const POINT_2D& p1, const POINT_2D& p2);

	bool Refresh();
	void RefreshBrush();


	TrecComPointer<ID2D1Brush> brush;
	TrecComPointer<ID2D1RenderTarget> currentRenderer;
	TrecPointer<DrawingBoard> board;

	TGradientStopCollection gradients;
};

