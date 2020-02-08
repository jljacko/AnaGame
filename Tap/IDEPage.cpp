#include "IDEPage.h"
#include "TInstance.h"

IDEPage::IDEPage(ide_page_type type, UINT barSpace)
{
	this->type = type;
	this->barSpace = barSpace;

	moveMode = type == ide_page_type_drag ? page_move_mode_drag : page_move_mode_normal;
	curPoint.x = curPoint.y = 0.0f;
	draw = true;



}

void IDEPage::SetResources(TrecPointer<TInstance> in, TrecComPointer<ID2D1RenderTarget> render, TrecPointer<TWindow> window)
{
	this->windowHandle = window;
	instance = in;
	fact = in->GetFactory();

	deviceH = GetWindowDC(window->GetWindowHandle());
	instance = in;
	regRenderTarget = render;

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder paintHolder;
	regRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), paintHolder.GetPointerAddress());
	clearBursh = paintHolder.Extract();
	rt_type = render_target_hwnd;
}

void IDEPage::SetResources(TrecPointer<TInstance> in, TrecComPointer<ID2D1RenderTarget> render, TrecPointer<TWindow> window, TrecPointer<TWindowEngine> engine)
{
	SetResources(in, render, window, engine);
	
	if(engine.Get())
	rt_type = render_target_device_context;
}

void IDEPage::MoveBorder(float& magnitude, page_move_mode mode)
{
	switch (mode)
	{
	case page_move_mode_bottom:
		area.bottom += magnitude;
		if (area.bottom < area.top)
		{
			magnitude -= (area.top - area.bottom);
			area.bottom = area.top;
		}
		break;
	case page_move_mode_left:
		area.left += magnitude;
		if (area.right < area.left)
		{
			magnitude -= (area.right - area.left);
			area.left = area.right;
		}
		break;
	case page_move_mode_right:
		area.right += magnitude;
		if (area.right < area.left)
		{
			magnitude -= (area.right - area.left);
			area.right = area.left;
		}
		break;
	case page_move_mode_top:
		area.top += magnitude;
		if (area.bottom < area.top)
		{
			magnitude -= (area.top - area.bottom);
			area.top = area.bottom;
		}
	}

	if (area.left == area.right || area.top == area.bottom)
		draw = false;
	else
		draw = true;

	RECT topBorder = area;
	topBorder.top = topBorder.top + barSpace;

	if (currentPage.Get())
	{
		for (UINT Rust = 0; Rust < pages.Size(); Rust++)
			if (pages[Rust].Get() && pages[Rust]->GetPage().Get())
				pages[Rust]->GetPage()->SetArea(topBorder);
	}
	else if (rootControl.Get())
		rootControl->Resize(convertRECTToD2DRectF(topBorder));
}

void IDEPage::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (isContained(point, area))
	{
		RECT topBorder = area;
		topBorder.bottom = topBorder.top + barSpace;
		curPoint = point;


		if (moveMode == page_move_mode_drag)
			goto regular_click_mode;


		


		if (isContained(point, topBorder))
		{
			moveMode = page_move_mode_top;
			// To-Do: Set Cursor

			return;
		}

		if (area.bottom - point.y < 0.5)
		{
			moveMode = page_move_mode_bottom;
			// To-Do: Set Cursor

			return;
		}

		if (area.right - point.x < 0.5)
		{
			moveMode = page_move_mode_right;
			// To-Do: Set Cursor

			return;
		}

		if (point.x - area.left < 0.5)
		{
			moveMode = page_move_mode_left;
			// To-Do: Set Cursor
			
			return;
		}

		regular_click_mode:
		Page::OnLButtonDown(nFlags, point, mOut, eventAr);
	}
}

void IDEPage::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (moveMode == page_move_mode_normal)
		return Page::OnMouseMove(nFlags, point, mOut, eventAr);

	TPoint diff(point.x - curPoint.x, curPoint.y - point.y);

	switch (type)
	{
	case ide_page_type_body:
		MouseMoveBody(diff);
		break;
	case ide_page_type_basic_console:
		MouseMoveBasicConsole(diff);
		break;
	case ide_page_type_deep_console:
		MouseMoveDeepConsole(diff);
		break;
	case ide_page_type_lower_left:
		MouseMoveLowerLeft(diff);
		break;
	case ide_page_type_lower_right:
		MouseMoveLowerRight(diff);
		break;
	case ide_page_type_upper_left:
		MouseMoveUpperLeft(diff);
		break;
	case ide_page_type_upper_right:
		MouseMoveUpperRight(diff);

	}

	curPoint.x = point.x;
	curPoint.y = point.y;
}

void IDEPage::OnLButtonUp()
{
	curPoint.x = curPoint.y = 0.0f;
	moveMode = page_move_mode_normal;
}

void IDEPage::Draw(TrecComPointer<ID2D1SolidColorBrush> color, TWindowEngine* twe)
{
	RECT topBorder = area;
	topBorder.bottom = topBorder.top + barSpace;

	if (color.Get())
		regRenderTarget->FillRectangle(convertRECTToD2DRectF(topBorder), color.Get());
	if (type != ide_page_type_drag)
	{
		if (dynamic_cast<IDEPage*>(currentPage.Get()))
			dynamic_cast<IDEPage*>(currentPage.Get())->Draw(color, twe);
	}
	else
		Page::Draw(twe);
	if (color.Get())
		regRenderTarget->DrawRectangle(convertRECTToD2DRectF(area), color.Get(), 1.5F);
}

void IDEPage::SetSelf(TrecPointer<Page> self)
{
	if (self.Get() != this)
		throw L"Error! Self needs to actually be self!";
	switch (type)
	{
	case ide_page_type_body:
		body = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_basic_console:
		basicConsole = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_deep_console:
		deepConsole = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_upper_right:
		upperRight = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_lower_right:
		lowerRight = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_upper_left:
		upperLeft = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;
	case ide_page_type_lower_left:
		lowerLeft = TrecPointerKey::GetSoftPointerFromTrec<Page>(self);
		break;

	}
}

void IDEPage::SetLink(TrecPointer<Page> p, ide_page_type t)
{
	switch (t)
	{
	case ide_page_type_body:
		body = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_basic_console:
		basicConsole = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_deep_console:
		deepConsole = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_upper_right:
		upperRight = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_lower_right:
		lowerRight = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_upper_left:
		upperLeft = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;
	case ide_page_type_lower_left:
		lowerLeft = TrecPointerKey::GetSoftPointerFromTrec<Page>(p);
		break;

	}
}

TString IDEPage::GetName()
{
	return name;
}

void IDEPage::SetNewParentPage(TrecPointer<Page> p)
{
	currentPage = p;
}

void IDEPage::AddNewPage(TrecPointer<IDEPageHolder> pageHolder)
{
	if (!pageHolder.Get() || !pageHolder->GetPage().Get()) return;
	for (UINT c = 0; c < pages.Size(); c++)
	{
		if (pages[c].Get() == pageHolder.Get())
		{
			return;
		}
	}

	D2D1_RECT_F loc{ 0.0f,0.0f,0.0f,0.0f };
	if (pages.Size())
	{
		loc = pages[pages.Size() - 1]->GetLocation();
		if (loc.right > 0.0f)
		{
			D2D1_RECT_F l = pageHolder->GetLocation();
			float leftBounds = loc.right + (l.right - l.left);
			if (leftBounds > area.right)
				loc.bottom = loc.left = loc.right = loc.top = 0.0f;
			else
			{
				loc.left = loc.right;
				loc.right = area.right;
			}
		}
		else 
			loc.bottom = loc.left = loc.right = loc.top = 0.0f;
	}
	else
	{
		loc = convertRECTToD2DRectF(area);
		loc.bottom = loc.top + barSpace;
	}

	pageHolder->SetLocation(loc);
	pages.push_back(pageHolder);

	loc = convertRECTToD2DRectF(area);
	loc.top += barSpace;
	pageHolder->GetPage()->SetArea(convertD2DRectToRECT(loc));
}

void IDEPage::AddNewPage(TrecPointer<TInstance> ins, TrecPointer<TWindow> win, TString name, TrecPointer<EventHandler> h)
{
	TrecSubPointer<Page, IDEPage> newPage = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(ide_page_type_drag, 0);

	newPage->SetResources(ins, regRenderTarget, win, win->GetWindowEngine());
	RECT curArea = area;
	curArea.top += barSpace;
	newPage->SetArea(curArea);

	curArea = area;
	curArea.bottom = curArea.top + barSpace;


	for (UINT c = pages.Size() - 1; c < -1; c--)
	{
		if (pages[c].Get() && pages[c]->GetLocation().right)
		{
			curArea.left = pages[c]->GetLocation().right;
			break;
		}
	}
	

	TrecPointer<IDEPageHolder> newHolder = TrecPointerKey::GetNewTrecPointer<IDEPageHolder>(name, regRenderTarget, barSpace, h, win, convertRECTToD2DRectF(curArea));
	pages.push_back(newHolder);
}

void IDEPage::RemovePage(TrecPointer<IDEPageHolder> pageHolder)
{
	int index = -1;
	for (UINT C = 0; C < pages.Size(); C++)
	{
		if (pageHolder.Get() == pages[C].Get())
		{
			index = C;
			break;
		}
	}

	if (index == -1)
		return;

	D2D1_RECT_F holdLoc = pageHolder->GetLocation();
	float width = holdLoc.right - holdLoc.left;

	for (UINT i = index + 1; i < pages.Size(); i++)
	{
		holdLoc = pages[i]->GetLocation();
		holdLoc.left -= width;
		holdLoc.right -= width;
		pages[i]->SetLocation(holdLoc);
	}

	pages.RemoveAt(index);
}

void IDEPage::MouseMoveBody(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	RECT curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode_top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_left:
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode_right:
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	//default:
		
	}
}

void IDEPage::MouseMoveBasicConsole(TPoint& diff)
{
	switch (moveMode)
	{
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode_top);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_top:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode_right:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

void IDEPage::MouseMoveDeepConsole(TPoint& diff)
{
	if (moveMode == page_move_mode_top)
	{
		RECT curRect = { 0,0,0,0 };
		float yDiff = diff.y;

		// First, tackle the basic console and the body if necessary
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		if (yDiff != diff.y)
		{
			float newDiff = yDiff - diff.y;
			dynamic_cast<IDEPage*>(body.Get())->MoveBorder(newDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += newDiff;
			curRect.top += newDiff;
			basicConsole.Get()->SetArea(curRect);
			diff.y = yDiff;
		}


		// Now do the left side
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		if (yDiff != diff.y)
		{
			float newDiff = yDiff - diff.y;
			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(newDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += newDiff;
			curRect.top += newDiff;
			basicConsole.Get()->SetArea(curRect);
			diff.y = yDiff;
		}

		// Finally, the right side
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		if (yDiff != diff.y)
		{
			float newDiff = yDiff - diff.y;
			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(newDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += newDiff;
			curRect.top += newDiff;
			basicConsole.Get()->SetArea(curRect);
			diff.y = yDiff;
		}
		MoveBorder(diff.y, moveMode);
	}
}

void IDEPage::MouseMoveUpperRight(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	RECT curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode_top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(yDiff, page_move_mode_right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

void IDEPage::MouseMoveLowerRight(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	RECT curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode_top:
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_right);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_right);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(yDiff, page_move_mode_right);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_right);
			curRect = basicConsole.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(diff.x, page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode_top);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
}

void IDEPage::MouseMoveUpperLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	RECT curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.y, page_move_mode_top);
		if (before.y != diff.y)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the deep console down
			yDiff = before.y - diff.y;

			dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(yDiff, page_move_mode_top);
			curRect = basicConsole.Get()->GetArea();
			curRect.bottom += yDiff;
			curRect.top += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_left:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(yDiff, page_move_mode_left);
			curRect = body.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode_left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(diff.x, page_move_mode_right);
		MoveBorder(diff.x, moveMode);
		break;
	}
}

void IDEPage::MouseMoveLowerLeft(TPoint& diff)
{
	TPoint before = diff;
	float yDiff = 0.0f, xDiff = 0.0f;
	RECT curRect = { 0,0,0,0 };
	switch (moveMode)
	{
	case page_move_mode_top:
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	case page_move_mode_right:
		dynamic_cast<IDEPage*>(body.Get())->MoveBorder(diff.x, page_move_mode_left);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.x, page_move_mode_left);
		if (before.x != diff.x)
		{
			// In this case, we went down and tried to push it down. We should still attempt to push the left panels left
			yDiff = before.x - diff.x;

			dynamic_cast<IDEPage*>(upperRight.Get())->MoveBorder(yDiff, page_move_mode_left);
			curRect = body.Get()->GetArea();
			curRect.left += yDiff;
			curRect.right += yDiff;
			body.Get()->SetArea(curRect);

			dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(yDiff, page_move_mode_left);
			curRect = basicConsole.Get()->GetArea();
			curRect.right += yDiff;
			curRect.left += yDiff;
			basicConsole.Get()->SetArea(curRect);

			// Since the deep Console moved, we need to send the message to the lower side panels
			//dynamic_cast<IDEPage*>(lowerLeft.Get())->MoveBorder(yDiff, page_move_mode_bottom);
		}
		dynamic_cast<IDEPage*>(upperLeft.Get())->MoveBorder(diff.x, page_move_mode_left);
		MoveBorder(diff.x, moveMode);
		break;
	case page_move_mode_bottom:
		dynamic_cast<IDEPage*>(deepConsole.Get())->MoveBorder(diff.y, page_move_mode_top);
		dynamic_cast<IDEPage*>(basicConsole.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		dynamic_cast<IDEPage*>(lowerRight.Get())->MoveBorder(diff.y, page_move_mode_bottom);
		MoveBorder(diff.y, moveMode);
		break;
	}
}

IDEPageHolder::IDEPageHolder(TString name, TrecComPointer<ID2D1RenderTarget> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc)
{
	text = TrecPointerKey::GetNewTrecPointer<TText>(rt, nullptr);
	text->setColor(D2D1::ColorF::Black);
	text->setNewFontSize(12.0f);
	text->setCaption(name);
	text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	text->onCreate(initLoc);
	bool w;
	float minWidth = text->GetMinWidth(w);
	if (w)
	{
		initLoc.right = initLoc.left + minWidth;
		text->setNewLocation(convertD2DRectToRECT(initLoc));
	}
	location = initLoc;

	page = TrecPointerKey::GetNewSelfTrecSubPointer<Page, IDEPage>(ide_page_type_drag, 0);
	page->SetHandler(handler);
}

TrecPointer<Page> IDEPageHolder::GetBasePage()
{
	return TrecPointerKey::GetTrecPointerFromSub<Page, IDEPage>(page);
}

TrecSubPointer<Page, IDEPage> IDEPageHolder::GetPage()
{
	return page;
}

TString IDEPageHolder::GetName()
{
	if(!text.Get())
	return TString();
	return text->getCaption();
}

D2D1_RECT_F IDEPageHolder::GetLocation()
{
	return location;
}

D2D1_RECT_F IDEPageHolder::SetLocation(const D2D1_RECT_F& newLoc)
{
	location = newLoc;
	if (text.Get())
	{
		text->setNewLocation(convertD2DRectToRECT( newLoc));
		bool w;
		float width = text->GetMinWidth(w);
		location.right = location.left + width;

		text->setNewLocation(convertD2DRectToRECT(location));

	}
	return location;
}

void IDEPageHolder::Draw()
{
	if (text.Get())
		text->onDraw(location);
}

void IDEPageHolder::ResetRenderer(TrecComPointer<ID2D1RenderTarget> rt)
{
	if (text.Get())
	{
		text->SetNewRenderTarget(rt);
	}
}

void IDEPageHolder::Move(TPoint& moveBy)
{
	location.bottom += moveBy.y;
	location.top += moveBy.y;
	location.left += moveBy.x;
	location.right += moveBy.x;

	if (text.Get())
		text->setNewLocation(convertD2DRectToRECT(location));
}
