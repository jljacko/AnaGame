
#include "AnafaceUI.h"
#include "TControl.h"


#define BAR_WIDTH 20

namespace
{

	TDataArray<TScrollBar*> scrollBarList;
}

/*
* Method: (TScrollBar) (Constructor)
* Purpose: Sets up the Scroll Bar
* Parameters: TControl& tc - the TControl to bind to
*			SCROLL_ORIENTATION so - the orientation of the Scroll Bar
* Returns: void
*/
TScrollBar::TScrollBar(TControl& tc,SCROLL_ORIENTATION so)
{
	parent = &tc;
	orient = so;
	renderer = parent->getRenderTarget();
	if (!scrollBrush.Get() && renderer.Get())
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder sb;
		renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), sb.GetPointerAddress());
		scrollBrush = sb.Extract();
	}



	ratio = wholeBlank = bodySize = 0;

	// Maintaining messages
	mouseTracker = TPoint( 0,0 );
	BodyClicked = backClicked = false;

	childBar = nullptr;

	// Color layout
	outline = D2D1::ColorF(D2D1::ColorF::Black);
	default_tip = D2D1::ColorF(0.5f, 0.0f, 0.1f, 1.0f);
	hover_tip = D2D1::ColorF(0.5f, 0.0f, 0.1f, 0.6f);
	default_back = D2D1::ColorF(0.0f, 0.2f, 0.0f, 1.0f);
	hover_back = D2D1::ColorF(0.0f, 0.2f, 0.0f, 0.6f);
	default_scroller = D2D1::ColorF(0.0f, 0.2f, 1.0f, 1.0f);
	hover_scroller = D2D1::ColorF(0.0f, 0.2f, 1.0f, 0.6f);

	// Add to ArrayList

	scrollBarList.push_back(this);
}

/*
* Method: (TScrollBar) (Destructor)
* Purpose: Cleans up the Scroll Bar
* Parameters: void
* Returns: void
*/
TScrollBar::~TScrollBar()
{

	for (int c = 0; c < scrollBarList.Size();c++)
	{
		if (scrollBarList.at(c) == this)
		{
			scrollBarList.RemoveAt(c);
			break;
		}
	}

	if (!scrollBarList.Size())
		scrollBrush.Nullify();
}

/*
* Method: TScrollBar - updateDraw
* Purpose: Draws out the scroll bar
* Parameters: void
* Returns: void
*/
void TScrollBar::updateDraw()
{	
	Refresh(); // Set the Proper Alignment


	if (!isZeroRect(wholeBar))
	{
		scrollBrush->SetColor(default_back);
		renderer->FillRectangle(wholeBar,scrollBrush.Get());

		scrollBrush->SetColor(outline);
		renderer->DrawRectangle(tipBar1, scrollBrush.Get());
	}
	if (!isZeroRect(BodyBar))
	{
		scrollBrush->SetColor(default_scroller);
		renderer->FillRectangle(BodyBar, scrollBrush.Get());

		scrollBrush->SetColor(outline);
		renderer->DrawRectangle(BodyBar,scrollBrush.Get());
	}
	if (!isZeroRect(tipBar1))
	{
		scrollBrush->SetColor(default_tip);
		renderer->FillRectangle(tipBar1, scrollBrush.Get());

		scrollBrush->SetColor(outline);
		renderer->DrawRectangle(tipBar1, scrollBrush.Get());
	}

	if (!isZeroRect(tipBar2))
	{
		scrollBrush->SetColor(default_tip);
		renderer->FillRectangle(tipBar2, scrollBrush.Get());

		scrollBrush->SetColor(outline);
		renderer->DrawRectangle(tipBar2, scrollBrush.Get());
	}
	
}

/*
* Method: TScrollBar - moveUpLeft
* Purpose: Moves up or left, depending on the Scroll Bars alignment
* Parameters: int l - amount to move it by
* Returns: int - how much it was actually moved
*/
int TScrollBar::moveUpLeft(int l)
{
	if (!parent)
		return 0;
	float spaceLeft = 0.0f;
	if (orient) // Verticle
	{
		spaceLeft = BodyBar.top - tipBar1.bottom;
		if (spaceLeft < l && spaceLeft > 0)
		{
			parent->onScroll(0, (spaceLeft * ratio));
			return spaceLeft;
		}
		else if(spaceLeft > 0)
			parent->onScroll(0, (l * ratio));
		return l;
	}
	else // horizontal
	{
		spaceLeft = BodyBar.left - tipBar1.right;
		if (spaceLeft < l && spaceLeft > 0)
		{
			parent->onScroll((spaceLeft * ratio), 0);
			return spaceLeft;
		}
		else if(spaceLeft > 0)
			parent->onScroll((l * ratio), 0);
		return l;
	}
	return 0;
}

/*
* Method: TScrollBar - moveDownRight
* Purpose: Moves down or right, depending on the Scroll bars alignment
* Parameters: int l - amount to move it by
* Returns: int - how much it was actually moved
*/
int TScrollBar::moveDownRight(int l)
{
	if (!parent)
		return 0;
	int spaceLeft = 0;
	if (orient) // Verticle
	{
		spaceLeft = tipBar2.top - BodyBar.bottom;
		if (spaceLeft < l && spaceLeft > 0)
		{
			parent->onScroll(0, -(spaceLeft * ratio));
			return spaceLeft;
		}
		else if(spaceLeft > 0)
			parent->onScroll(0, -(l * ratio));
		return l;
	}
	else // horizontal
	{
		spaceLeft = tipBar2.left - BodyBar.right;
		if (spaceLeft < l && spaceLeft > 0)
		{
			parent->onScroll(-(spaceLeft * ratio), 0);
			return spaceLeft;
		}
		else if(spaceLeft > 0)
			parent->onScroll(-(l * ratio), 0);
		return l;
	}
	return 0;
	//return 0;
}

/*
* Method: TScrollBar - Refresh
* Purpose: Sets the current location of the scroll-bar
* Parameters: void
* Returns: void
*/
void TScrollBar::Refresh()
{
	if (!parent || isSnipZero(parent->getSnip()))
	{
		setUndrawable();
		return;
	}

	RECT loc = parent->getLocation(), sni = parent->getSnip();

	if (orient) // Vertical
	{
		// First, figure out if the scroll Bar will be drawn and how wide
		int barWidth = BAR_WIDTH;// -(loc.left - sni.left);
		if (barWidth <= 0)
			return;

		// Now figure out how to align it
		int locHeight = loc.bottom - loc.top;
		int sniHeight = sni.bottom - sni.top;

		if (!locHeight || sniHeight < 50)
		{
			setUndrawable();
			return;
		}
		float contRatio = static_cast<float>(locHeight) / static_cast<float>(sniHeight);

		if (contRatio <= 1.0f)
		{
			setUndrawable();
			return;
		}

		int scrollSpace = sniHeight - (2 * BAR_WIDTH);
		int scrollerHeight = scrollSpace / contRatio;

		ratio = static_cast<float>(locHeight) / static_cast<float>(scrollSpace);

		int locDif = sni.top - loc.top;

		// Begin Setting the Scroll Bars Rectangles
		wholeBar.top = sni.top;
		wholeBar.bottom = sni.bottom;
		wholeBar.right = sni.right;
		wholeBar.left = wholeBar.right - barWidth;

		tipBar1.top = sni.top;
		tipBar1.bottom = tipBar1.top + BAR_WIDTH;
		tipBar1.right = sni.right;
		tipBar1.left = tipBar1.right - barWidth;

		tipBar2.bottom = sni.bottom;
		tipBar2.top = tipBar2.bottom - BAR_WIDTH;
		tipBar2.right = sni.right;
		tipBar2.left = tipBar2.right - barWidth;

		// Here comes the hard part, where does the srollable peice go?
		BodyBar.top = tipBar1.bottom + (static_cast<float>(locDif) / ratio);
		BodyBar.bottom = BodyBar.top + scrollerHeight;
		BodyBar.right = sni.right;
		BodyBar.left = BodyBar.right - barWidth;
	}
	else        // Horizontal
	{
		// First, figure out if the scroll Bar will be drawn and how tall
		int barHeight = BAR_WIDTH;// -(loc.top - sni.top);
		if (barHeight <= 0)
			return;

		// Now figure out how to align it
		int locWidth = loc.right - loc.left;
		int sniWidth = sni.right - sni.left;

		if (!locWidth || sniWidth < 50)
		{
			setUndrawable();
			return;
		}
		float contRatio = static_cast<float>(locWidth) / static_cast<float>(sniWidth);

		if (contRatio <= 1.0f)
		{
			setUndrawable();
			return;
		}

		int scrollSpace = sniWidth - (2 * BAR_WIDTH);
		int scrollerHeight = scrollSpace / contRatio;

		ratio = static_cast<float>(locWidth) / static_cast<float>(scrollSpace);

		int locDif = sni.left - loc.left;

		// Begin Setting the Scroll Bars Rectangles
		wholeBar.bottom = sni.bottom;
		wholeBar.top = wholeBar.bottom - barHeight;
		wholeBar.left = sni.left;
		wholeBar.right = sni.right;

		tipBar1.bottom = sni.bottom;
		tipBar1.top = tipBar1.bottom - barHeight;
		tipBar1.left = sni.left;
		tipBar1.right = tipBar1.left + BAR_WIDTH;

		tipBar2.bottom = sni.bottom;
		tipBar2.top = tipBar2.bottom - barHeight;
		tipBar2.right = sni.right;
		tipBar2.left = tipBar2.right - BAR_WIDTH;

		// Here comes the hard part, where does the srollable peice go?
		BodyBar.left = tipBar1.left + (static_cast<float>(locDif) / ratio);
		BodyBar.right = BodyBar.left + scrollerHeight;
		BodyBar.bottom = sni.bottom;
		BodyBar.top = BodyBar.bottom - barHeight;
	}

}

/*
* Method: TScrollBar - OnLButtonUp
* Purpose: Frees the Scroll Bar upon user release
* Parameters: UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void TScrollBar::OnLButtonUp(UINT nFlags, TPoint point, messageOutput * mOut)
{
	mouseTracker = { 0,0 };
	BodyClicked = false;
	backClicked = false;
}

/*
* Method: TScrollBar - OnLButtonDown
* Purpose: Allows the TScroll-Bar to respond to User clicks
* Parameters: UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void TScrollBar::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut)
{
	if (!isContained(&point, &wholeBar))
	{
		*mOut = negative;
		return;
	}

	mouseTracker = point;
	backClicked = true;
	BodyClicked = false;
	if (isContained(&point, &tipBar1))
	{
		moveUpLeft(20);
		
	}
	else if (isContained(&point, &tipBar2))
	{
		moveDownRight(20);
	}
	else if (isContained(&point, &BodyBar))
	{
		BodyClicked = true;
	}
	else
	{
		if (orient) // Vertical
		{
			if (point.y > BodyBar.top)         // move up
			{
				moveUpLeft(30);
			}
			else if (point.y < BodyBar.bottom) // move down
			{
				moveDownRight(30);
			}
		}
		else        // horizontal
		{
			if (point.x > BodyBar.right)         // move right
			{
				moveDownRight(30);
			}
			else if (point.x < BodyBar.left)     // move left
			{
				moveUpLeft(30);
			}
		}
	}

	if (backClicked && !BodyClicked)
	{
		// We are moving, need to update scroll and Control, thus update the view
		*mOut = positiveOverrideUpdate;
	}
}

/*
* Method: TScrollBar - OnMouseMove
* Purpose: Allows the TScrollBar to respond to the mouse-movement
* Parameters: UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void TScrollBar::OnMouseMove(UINT nFlags, TPoint point, messageOutput * mOut)
{
	if (BodyClicked)
	{
		int difference = 0;
		if (orient)   // vertical
		{
			difference = point.y - mouseTracker.y;

			// if difference is positive, then the scroller is moving down
			//  and the control is being moved up. In this case, we need
			//  to make sure that the bottom of scroller is higher than the
			//  top of the bottom tip bar.

			// if difference is negative, then the scroller is moving up and the
			//  control is being moved down. In this case, we need to make sure that
			//  the top of the scroller is lower than the bottom of the top tip bar


			if (difference > 0 && BodyBar.bottom < tipBar2.top)
				moveDownRight(difference);
			else if(BodyBar.top > tipBar1.bottom)
				moveUpLeft(-difference);
		}
		else          // horizontal
		{
			difference = point.x - mouseTracker.x;

			// If difference is positive, than the scroller is being moved right and the 
			//  control is being moved left. In this case, we need to make sure the right
			//  of the scroller is left to the left of the tipbar 2.

			// If difference is negative, than the scroller is being moved left and the
			//  control is being moved right. In this case, we need to make sure th left
			//  of the scroller is right to the right of the tipbar 1 

			if (difference > 0 && BodyBar.right < tipBar2.left)
				moveDownRight(difference);
			else if(BodyBar.left > tipBar1.right)
				moveUpLeft(-difference);
		}
		mouseTracker = point;

		// Update View
		*mOut = positiveOverrideUpdate;
	}
}

/*
* Method: TScrollBar - setUndrawable
* Purpose: Makes the Scroll-Bar undrawable in case it goes out of screen
* Parameters: void
* Returns: void
*/
void TScrollBar::setUndrawable()
{
	ZeroRect(wholeBar);
	ZeroRect(tipBar1);
	ZeroRect(tipBar2);
	ZeroRect(BodyBar);
}



/*
* Function: ZeroRect
* Purpose: Sets an MFC rectangle to zero
* Parameters: RECT& r - the Rectangle to "zero"
* Returns: void
*/
void ZeroRect(RECT& r)
{
	r = { 0,0,0,0 };
}

/*
* Function: ZeroRect
* Purpose: Sets a Direct2D rectangle to zero
* Parameters: D2D1_RECT_F& r - the Rectangle to "zero"
* Returns: void
*/
void ZeroRect(D2D1_RECT_F& r)
{
	r = { 0.0f, 0.0f, 0.0f, 0.0f };
}

/*
* Function: isZeroRect
* Purpose: Reports whether a rectangle (MFC) is "zeroed"
* Parameters: RECT& r - the Rectangle to examine
* Returns: bool - whether the rectangle is truly just 0's
*/
bool isZeroRect(RECT& r)
{
	return !r.bottom && !r.left && !r.right && !r.top;
}

/*
* Function: isZeroRect
* Purpose: Reports whether a rectangle (Direct2D) is "zeroed"
* Parameters: D2D1_RECT_F& r - the Rectangle to examine
* Returns: bool - whether the rectangle is truly just 0's
*/
bool isZeroRect(D2D1_RECT_F& r)
{
	return !r.bottom && !r.left && !r.right && !r.top;
}

/*
* Function: ScrollLButtonDown
* Purpose: Allows Scroll-bars to be selected by the user
* Parameters: UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void ScrollLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut)
{
	for (int c = 0; c < scrollBarList.Size();c++)
	{
		TScrollBar* scroller = scrollBarList.at(c);
		if (scroller)
			scroller->OnLButtonDown(nFlags, point, mOut);
		if (*mOut != negative && *mOut != negativeUpdate)
			break;
	}
}

/*
* Function: ScrollLButtonUp
* Purpose: Allows Clicked Scroll-bars to release them selves
* Parameters: UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void ScrollLButtonUp(UINT nFlags, TPoint point, messageOutput * mOut)
{
	for (int c = 0; c < scrollBarList.Size();c++)
	{
		TScrollBar* scroller = scrollBarList.at(c);
		if (scroller)
			scroller->OnLButtonUp(nFlags, point, mOut);
		if (*mOut != negative && *mOut == negativeUpdate)
			break;
	}
}

/*
* Function: ScrollMouseMove
* Purpose: Allows a scroll-bar to know when the mouse is moving
* Parameters:UINT nFlags - Details provided by Windows 
*				TPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?)
* Returns: void
*/
void ScrollMouseMove(UINT nFlags, TPoint point, messageOutput * mOut)
{
	for (int c = 0; c < scrollBarList.Size();c++)
	{
		TScrollBar* scroller = scrollBarList.at(c);
		if (scroller)
			scroller->OnMouseMove(nFlags, point, mOut);
		if (*mOut != negative && *mOut != negativeUpdate)
			break;
	}
}
