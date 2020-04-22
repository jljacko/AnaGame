#pragma once
#include <d2d1.h>
#include <TObject.h>
#include "Anaface.h"
#include <TPoint.h>

class TControl;

typedef enum class ScrollOrient
{
	so_vertical,
	so_horizontal
}ScrollOrient;

/* Allows individual Controls to communicate with the message engine that called it*/
typedef enum class messageOutput
{
	negative,
	negativeUpdate,
	positiveOverride,
	positiveContinue,
	positiveOverrideUpdate,
	positiveContinueUpdate,
	positiveScroll
}messageOutput;

class _ANAFACE_DLL TScrollBar : public TObject
{
public:
	TScrollBar(TControl& control, ScrollOrient so);
	~TScrollBar();

	void onDraw(ID2D1RenderTarget* target);

	virtual bool OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);

	float MovedContent(float degree);

	void Refresh(const D2D1_RECT_F& location, const D2D1_RECT_F& area);

private: // Static attributes, styles that should be shared across all Scroll Bars

	static void EstablishScrollColors();

	D2D1_RECT_F body_rect, scroll_rect;

	TControl* parent;
	TPoint prevPoint;
	bool onFocus, drawable;
	float diff1, diff2, widthFactor;
	ScrollOrient scrollAlignment;
};

UINT GetScrollbarBoxSize();