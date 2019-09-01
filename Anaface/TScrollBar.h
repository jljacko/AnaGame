#pragma once
#include <TrecReference.h>
#include "Anaface.h"

#define SCROLL_ORIENTATION bool
#define SCROLL_VERTICAL    true
#define SCROLL_HORIZONTAL  false

class TControl;

enum messageOutput; // Defined in TControl.h, which will be included in the .cpp file

/*
* class TScrollBar
* Implements Anaface's scroll-bar so that controls can be scrolled to fit within a constrained space
*/
class TScrollBar
{
	friend class TScrollBar;
public:
	// expected to be called by the owning TControl
	TScrollBar(TControl&, SCROLL_ORIENTATION);
	~TScrollBar();

	void updateDraw();
	int moveUpLeft(int l);
	int moveDownRight(int l);

	// Align the proper sizes of the components
	void Refresh();


	// Messages that the Scroll Bar might respond to
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point, messageOutput* mOut);
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point, messageOutput* mOut);



private:
	void setUndrawable();


	TControl* parent;           // the TControl owning the Scroll Bar
	TrecComPointer<ID2D1RenderTarget> renderer; // Direct 2D screen to draw on
	SCROLL_ORIENTATION orient;  // Horizontal or Vertical
	bool fadeOnInactivity;      // Should it be shown all the time
	D2D1_RECT_F wholeBar,       // whole scroll bar
		tipBar1,                // Bar one, moves scroll in direction one when clicked
		tipBar2,                // Bar 2, moves scroll in other direction
		BodyBar,                // Bar in the middle
		wholeSnip;              // Because part of the scroll bar might be covered

	TScrollBar* childBar;       // Support for multilayered Scrolling
	bool onFocus;               // Only move the TControl if true
	int arrayLoc;


	// Data for calculations
	float ratio;
	int wholeBlank;
	int bodySize;
	
	// Maintaining messages
	CPoint mouseTracker;
	bool BodyClicked;
	bool backClicked;

	// Colors
	D2D1_COLOR_F outline,
		default_tip, hover_tip,
		default_back, hover_back,
		default_scroller, hover_scroller;

	TrecComPointer<ID2D1SolidColorBrush> scrollBrush;
};

void ZeroRect(RECT&);
void ZeroRect(D2D1_RECT_F&);
bool isZeroRect(RECT&);
bool isZeroRect(D2D1_RECT_F&);

_ANAFACE_DLL void ScrollLButtonDown(UINT nFlags, CPoint point, messageOutput * mOut);
_ANAFACE_DLL void ScrollLButtonUp(UINT nFlags, CPoint point, messageOutput * mOut);
_ANAFACE_DLL void ScrollMouseMove(UINT nFlags, CPoint point, messageOutput * mOut);