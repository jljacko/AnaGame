#pragma once
#include "Page.h"
#include "TIdeWindow.h"

typedef enum page_move_mode
{
	page_move_mode_normal,
	page_move_mode_drag,
	page_move_mode_top,
	page_move_mode_bottom,
	page_move_mode_left,
	page_move_mode_right
}page_move_mode;

class _TAP_DLL IDEPage :
	public Page
{
	friend class TIdeWindow;
	friend class TrecPointerKey;
protected:
	IDEPage(ide_page_type type, UINT barSpace);

	void MoveBorder(float& magnitude, page_move_mode mode);

public:
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp();

	void Draw(TrecComPointer<ID2D1SolidColorBrush> color, TWindowEngine* twe = nullptr);
	void SetSelf(TrecPointer<Page>);
	void SetLink(TrecPointer<Page> p, ide_page_type t);

protected:

	TrecPointerSoft<Page> body;
	TrecPointerSoft<Page> basicConsole;
	TrecPointerSoft<Page> deepConsole;
	TrecPointerSoft<Page> upperRight;
	TrecPointerSoft<Page> lowerRight;
	TrecPointerSoft<Page> upperLeft;
	TrecPointerSoft<Page> lowerLeft;

	ide_page_type type;
	UINT barSpace;
	page_move_mode moveMode;

	bool draw;
	TPoint curPoint;


private:
	void MouseMoveBody(TPoint& diff);
	void MouseMoveBasicConsole(TPoint& diff);
	void MouseMoveDeepConsole(TPoint& diff);
	void MouseMoveUpperRight(TPoint& diff);
	void MouseMoveLowerRight(TPoint& diff);
	void MouseMoveUpperLeft(TPoint& diff);
	void MouseMoveLowerLeft(TPoint& diff);
};

