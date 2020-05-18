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


	if (vScroll.Get() || hScroll.Get())
	{

		D2D1_RECT_F clipRect = location;
		clipRect.bottom -= GetScrollbarBoxSize();
		clipRect.right -= GetScrollbarBoxSize();

		// Need to generate a geometry
		drawingBoard->AddLayer(clipRect);

		if (vScroll.Get())
			vScroll->Refresh(clipRect, childControl->getLocation());
		if (hScroll.Get())
			hScroll->Refresh(clipRect, childControl->getLocation());
	}
	childControl->onDraw(obj);

	if (vScroll.Get() || hScroll.Get())
	{
		drawingBoard->PopLayer();
	}
	if (vScroll.Get())
		vScroll->onDraw(drawingBoard->GetRenderer().Get());
	if (hScroll.Get())
		hScroll->onDraw(drawingBoard->GetRenderer().Get());

}

void TScrollerControl::SetChildControl(TrecPointer<TControl> cont)
{
	childControl = cont;
	children.Clear();
	TControl::addChild(cont);
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

	if (vScroll.Get() && vScroll->OnLButtonDown(nFlags, point, mOut))
	{
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;

		eventAr.push_back(EventID_Cred(R_Message_Type::On_Click, this, vScroll));
		return;
	}


	if (hScroll.Get() && hScroll->OnLButtonDown(nFlags, point, mOut))
	{
		resetArgs();
		args.eventType = R_Message_Type::On_Select_Scroller;
		args.point = point;
		args.methodID = -1;
		args.isClick = args.isLeftClick = true;
		args.control = nullptr;

		eventAr.push_back(EventID_Cred(R_Message_Type::On_Click, this, hScroll));
		return;
	}

	if (childControl.Get())
		childControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
}

void TScrollerControl::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (onScrollFocus)
	{
		if (vScroll.Get())
			vScroll->OnMouseMove(nFlags, point, mOut);
		if (hScroll.Get())
			hScroll->OnMouseMove(nFlags, point, mOut);
		return;
	}

	if (childControl.Get())
		childControl->OnMouseMove(nFlags, point, mOut, eventAr);
}

void TScrollerControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (vScroll.Get())
		vScroll->OnLButtonUp(nFlags, point, mOut);
	if (hScroll.Get())
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
	if (!vScroll.Get() && ((location.bottom - location.top) < (childLocation.bottom - childLocation.top)))
	{
		// We need to create the Vertical ScrollBar
		vScroll =  TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_vertical);
	}

	if (!hScroll.Get() && ((location.right - location.left) < (childLocation.right - childLocation.left)))
	{
		// We need to create the horizontal ScrollBar
		hScroll =  TrecPointerKey::GetNewTrecPointer<TScrollBar>(*this, ScrollOrient::so_horizontal);
	}

	//if (vScroll && ((location.bottom - location.top) > (childLocation.bottom - childLocation.top)))
	//{

	//}


	D2D1_RECT_F clipRect = location;
	if(vScroll.Get())
		clipRect.bottom -= GetScrollbarBoxSize();
	if(hScroll.Get())
		clipRect.right -= GetScrollbarBoxSize();
	
	if (vScroll.Get())
		vScroll->Refresh(clipRect, childLocation);
	if (hScroll.Get())
		hScroll->Refresh(clipRect, childLocation);
}
