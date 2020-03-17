#include "TScrollerControl.h"

TScrollerControl::TScrollerControl(TrecPointer<DrawingBoard> rt, TrecPointer<TArray<styleTable>> styles) :TControl( rt, styles)
{
	onScrollFocus = false;
}

void TScrollerControl::onDraw(TObject* obj)
{
	if (!isActive || !childControl.Get())
		return;

	if ((location.bottom - location.top) < 1 || (location.right - location.left) < 1)
		return;



	RefreshScroll();


	if (vScroll || hScroll)
	{

		D2D1_RECT_F clipRect = location;
		clipRect.bottom -= GetScrollbarBoxSize();
		clipRect.right -= GetScrollbarBoxSize();

		// Need to generate a geometry
		drawingBoard->AddLayer(clipRect);

		if (vScroll)
			vScroll->Refresh(clipRect, childControl->getLocation());
		if (hScroll)
			hScroll->Refresh(clipRect, childControl->getLocation());
	}
	childControl->onDraw(obj);

	if (vScroll || hScroll)
	{
		drawingBoard->PopLayer();
	}
	if (vScroll)
		vScroll->onDraw(drawingBoard->GetRenderer().Get());
	if (hScroll)
		hScroll->onDraw(drawingBoard->GetRenderer().Get());

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
	RefreshScroll();
}

void TScrollerControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	if (!isContained(&point, &location))
		return;

	if (vScroll && vScroll->OnLButtonDown(nFlags, point, mOut))
	{
		onScrollFocus = true;
		return;
	}

	if (hScroll && hScroll->OnLButtonDown(nFlags, point, mOut))
	{
		onScrollFocus = true;
		return;
	}
}

void TScrollerControl::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (onScrollFocus)
	{
		if (vScroll)
			vScroll->OnMouseMove(nFlags, point, mOut);
		if (hScroll)
			hScroll->OnMouseMove(nFlags, point, mOut);
		return;
	}

	if (childControl.Get())
		childControl->OnMouseMove(nFlags, point, mOut, eventAr);
}

void TScrollerControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (vScroll)
		vScroll->OnLButtonUp(nFlags, point, mOut);
	if (hScroll)
		hScroll->OnLButtonUp(nFlags, point, mOut);

	if (childControl.Get())
		childControl->OnLButtonUp(nFlags, point, mOut, eventAr);
	onScrollFocus = false;
}

TrecPointer<TControl> TScrollerControl::GetChildControl()
{
	return childControl;
}

void TScrollerControl::RefreshScroll()
{
	if (!childControl.Get())
		return;
	D2D1_RECT_F childLocation = childControl->getLocation();
	if (!vScroll && ((location.bottom - location.top) < (childLocation.bottom - childLocation.top)))
	{
		// We need to create the Vertical ScrollBar
		vScroll = new TScrollBar(*this, ScrollOrient::so_vertical);
	}

	if (!hScroll && ((location.right - location.left) < (childLocation.right - childLocation.left)))
	{
		// We need to create the horizontal ScrollBar
		hScroll = new TScrollBar(*this, ScrollOrient::so_horizontal);
	}

	//if (vScroll && ((location.bottom - location.top) > (childLocation.bottom - childLocation.top)))
	//{

	//}


	D2D1_RECT_F clipRect = location;
	if(vScroll)
		clipRect.bottom -= GetScrollbarBoxSize();
	if(hScroll)
		clipRect.right -= GetScrollbarBoxSize();
	
	if (vScroll)
		vScroll->Refresh(clipRect, childLocation);
	if (hScroll)
		hScroll->Refresh(clipRect, childLocation);
}
