#pragma once
#include <TObject.h>
#include <d2d1.h>
#include <TrecReference.h>
#include <Drawer.h>
#include "TWindowEngine.h"

class TWindow;

class DrawingBoard : public TObject
{
public:
	DrawingBoard(TrecComPointer<ID2D1Factory1> fact, HWND window);
	DrawingBoard(TrecComPointer<ID2D1Factory1> fact, TrecPointer<TWindowEngine> engine);

	void Set3D(TrecPointer<TWindowEngine> engine);




	TrecComPointer<ID2D1RenderTarget> GetRenderer();

	void SetSelf(TrecPointer<DrawingBoard> self);
private:
	TrecComPointer<ID2D1RenderTarget> renderer;
	TrecPointerSoft<DrawingBoard> self;
	bool is3D;

	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecPointer<TWindowEngine> engine;
};
