#include "TScrollerControl.h"

TScrollerControl::TScrollerControl(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> styles) :TControl( rt, styles)
{
}

void TScrollerControl::onDraw(TObject* obj)
{
	if (!isActive || !childControl.Get())
		return;

	if ((location.bottom - location.top) < 1 || (location.right - location.left) < 1)
		return;

	ID2D1Layer* layer = nullptr;

	if (vScroll || hScroll)
	{
		renderTarget->CreateLayer(&layer);

		D2D1_RECT_F clipRect = location;
		clipRect.bottom -= GetScrollbarBoxSize();
		clipRect.right -= GetScrollbarBoxSize();

		renderTarget->PushLayer(D2D1::LayerParameters(clipRect), layer);

		if (vScroll)
			vScroll->Refresh(clipRect, childControl->getLocation());
		if (hScroll)
			hScroll->Refresh(clipRect, childControl->getLocation());
	}
	childControl->onDraw(obj);

	if (layer)
	{
		renderTarget->PopLayer();
		layer->Release();
	}
	layer = nullptr;
	if (vScroll)
		vScroll->onDraw(renderTarget.Get());
	if (hScroll)
		hScroll->onDraw(renderTarget.Get());

}

void TScrollerControl::SetChildControl(TrecPointer<TControl> cont)
{
	childControl = cont;
	children.Clear();
	children.Add(cont);
}

void TScrollerControl::Resize(D2D1_RECT_F& loc)
{
	location = loc;
	D2D1_RECT_F clipRect = location;
	clipRect.bottom -= GetScrollbarBoxSize();
	clipRect.right -= GetScrollbarBoxSize();

	if (vScroll)
		vScroll->Refresh(clipRect, childControl->getLocation());
	if (hScroll)
		hScroll->Refresh(clipRect, childControl->getLocation());
}
