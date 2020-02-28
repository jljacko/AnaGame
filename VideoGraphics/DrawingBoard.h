#pragma once
#include <TObject.h>
#include <d2d1.h>
#include <TrecReference.h>
#include <Drawer.h>
#include "TWindowEngine.h"
#include "TBrush.h"
#include "TBitmapBrush.h"

class TWindow;

class _VIDEO_GRAPHICS DrawingBoard : public TObject
{
public:
	DrawingBoard(TrecComPointer<ID2D1Factory1> fact, HWND window);
	DrawingBoard(TrecComPointer<ID2D1Factory1> fact, TrecPointer<TWindowEngine> engine);

	void Set3D(TrecPointer<TWindowEngine> engine);
	void Resize(HWND window);

	TrecPointer<TBrush> GetBrush(const TColor& col);
	TrecPointer<TBrush> GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2, float x, float y);
	TrecPointer<TBrush> GetBrush(const TGradientStopCollection& coll, const POINT_2D& p1, const POINT_2D& p2);
	TrecSubPointer<TBrush, TBitmapBrush> GetBrush(TrecPointer<TFileShell> picture, RECT_2D& loc);


	TrecComPointer<ID2D1RenderTarget> GetRenderer();
	TrecComPointer<ID2D1GdiInteropRenderTarget> GetGdiRenderer();

	void SetSelf(TrecPointer<DrawingBoard> self);

	bool SetTransform(const TRANSFORM_2D& matrix);
	bool GetTransform(TRANSFORM_2D& matrix);

	// Methods handling layers
	void PopLayer();
	bool AddLayer(RECT_2D& ret);
	bool AddLayer(ELLIPSE_2D& ellipse);
	bool AddLayer(ROUNDED_RECT_2D& rRect);
	bool AddLayer(TDataArray<POINT_2D>& points);

	// Methods to Generate Geometries
	TrecPointer<TGeometry> GetGeometry(RECT_2D& ret);
	TrecPointer<TGeometry> GetGeometry(ELLIPSE_2D& ellipse);
	TrecPointer<TGeometry> GetGeometry(ROUNDED_RECT_2D& rRect);
	TrecPointer<TGeometry> GetGeometry(TDataArray<POINT_2D>& points);

	UINT GetLayerCount();

private:
	TrecComPointer<ID2D1Factory1> fact;

	TrecComPointer<ID2D1RenderTarget> renderer;
	TrecPointerSoft<DrawingBoard> self;
	bool is3D;

	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecPointer<TWindowEngine> engine;

	UINT layersPushed;
	TDataArray<TrecComPointer<ID2D1Layer>> layers;
	TDataArray<TrecPointer<TGeometry>> geometries;
};

