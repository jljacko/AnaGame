#include "TScrollBar.h"
#include "TControl.h"

#define BOX_SIZE 20
D2D1_COLOR_F end_box, body_box, middle_box_click, middle_box;
int* pointer = nullptr;
int value = 255;

TScrollBar::TScrollBar(TControl& control, ScrollOrient so)
{
	scrollAlignment = so;
	parent = &control;


	if (!pointer)
		EstablishScrollColors();
}

TScrollBar::~TScrollBar()
{
}

void TScrollBar::onDraw(ID2D1RenderTarget* target)
{
	if (!target || !parent) return;

	RECT location = parent->getLocation();

	ID2D1SolidColorBrush* brush = nullptr;
	HRESULT res = target->CreateSolidColorBrush(body_box, &brush);

	if (FAILED(res)) return;

	D2D1_RECT_F d_box = body_rect;
	target->FillRectangle(d_box, brush);

	brush->SetColor(end_box);
	if (scrollAlignment == so_horizontal)
	{
		d_box.right = d_box.left + BOX_SIZE;

		target->FillRectangle(d_box, brush);

		float move = (body_rect.right - body_rect.left) - BOX_SIZE;
		d_box.right += move;
		d_box.left += move;


	}
	else
	{
		d_box.bottom = d_box.top + BOX_SIZE;

		target->FillRectangle(d_box, brush);

		float move = (body_rect.bottom - body_rect.top) - BOX_SIZE;
		d_box.bottom += move;
	}
	target->FillRectangle(d_box, brush);

	if (onFocus)
		brush->SetColor(middle_box_click);
	else
		brush->SetColor(middle_box);

	target->FillRectangle(scroll_rect, brush);
	brush->Release();
}

void TScrollBar::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (isContained(&point, &body_rect))
	{
		onFocus = true;
		*mOut = positiveScroll;
		prevPoint = point;
	}
	else
		onFocus = false;
}

void TScrollBar::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	onFocus = false;
}

void TScrollBar::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (!onFocus) return;

	if (scrollAlignment == so_horizontal)
	{
		float move = MovedContent(point.x - prevPoint.x);
		parent->onScroll(move / widthFactor, 0);
	}
	else
	{
		float move = MovedContent(point.y - prevPoint.y);
		parent->onScroll(0, move / widthFactor);
	}
}

float TScrollBar::MovedContent(float degree)
{
	if (degree > 0.0f)
	{
		if (degree > diff2)
		{
			degree -= (degree - diff2);
		}
		diff2 -= degree;
		diff1 += degree;
	}
	else if (degree < 0.0f)
	{
		if (-degree > diff1)
			degree += (degree + diff1);
		diff2 -= degree;
		diff1 += degree;
	}

	return degree;
}

void TScrollBar::Refresh(RECT& location, RECT& area)
{
	convertCRectToD2DRect(&location, &body_rect);

	if (scrollAlignment == so_horizontal)
	{
		body_rect.top = body_rect.bottom - BOX_SIZE;

		diff1 = location.left - area.left;
		diff2 = area.right - location.right;

		float barSpace = body_rect.right - body_rect.left - (2 * BOX_SIZE);
		float areaSpace = area.right - area.left;
		float locationSpace = location.right - location.left;
		widthFactor = (locationSpace / areaSpace);

		scroll_rect.bottom = body_rect.bottom;
		scroll_rect.top = body_rect.top;

		scroll_rect.left = body_rect.left + BOX_SIZE + diff1 * widthFactor;
		scroll_rect.right = body_rect.right - BOX_SIZE - (diff2 * widthFactor);
	}
	else
	{
		body_rect.left = body_rect.right - BOX_SIZE;

		diff1 = location.top - area.top;
		diff2 = area.bottom - location.bottom;

		float barSpace = body_rect.bottom - body_rect.top - (2 * BOX_SIZE);
		float areaSpace = area.bottom - area.top;
		float locationSpace = location.bottom - location.top;
		widthFactor = (locationSpace / areaSpace);

		scroll_rect.left = body_rect.left;
		scroll_rect.right = body_rect.right;

		scroll_rect.top = body_rect.top + BOX_SIZE + diff1 * widthFactor;
		scroll_rect.bottom = body_rect.bottom - BOX_SIZE - (diff2 * widthFactor);
	}
}

void TScrollBar::EstablishScrollColors()
{
	body_box = D2D1::ColorF(0x00000011);
	middle_box = D2D1::ColorF(0x00000033);
	middle_box_click = D2D1::ColorF(0x00000099);
	end_box = D2D1::ColorF(0x00000066);
	pointer = &value;
}