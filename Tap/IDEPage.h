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


class IDEPageHolder
{
	friend class IDEPage;
	friend class IDEPageHolder;
public:
	IDEPageHolder(TString name, TrecComPointer<ID2D1RenderTarget> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc);
	TrecPointer<Page> GetBasePage();
	TrecSubPointer<Page, IDEPage> GetPage();

	TString GetName();

	D2D1_RECT_F Draw();
	void ResetRenderer(TrecComPointer<ID2D1RenderTarget> rt);
	void Move(TPoint& moveBy);

protected:
	TrecSubPointer<Page, IDEPage> page;
	TrecPointer<TText> text;
	D2D1_RECT_F location;
};

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


	// Methods for the Drag version of the Page
	TString GetName();
	void SetNewParentPage(TrecPointer<Page> p);

	// Methods for Parent Pages
	void AddNewPage(TrecPointer<IDEPageHolder> pageHolder);
	void AddNewPage(TrecPointer<TInstance> ins, TrecPointer<TWindow> win, TString name);
	void RemovePage(TrecPointer<IDEPageHolder> pageHolder);

protected:

	TrecPointerSoft<Page> body;
	TrecPointerSoft<Page> basicConsole;
	TrecPointerSoft<Page> deepConsole;
	TrecPointerSoft<Page> upperRight;
	TrecPointerSoft<Page> lowerRight;
	TrecPointerSoft<Page> upperLeft;
	TrecPointerSoft<Page> lowerLeft;

	TDataArray<TrecPointer<Page>> pages;
	TrecPointer<Page> currentPage;

	ide_page_type type;
	UINT barSpace;
	page_move_mode moveMode;

	bool draw;
	TPoint curPoint;

	TString name;


private:
	void MouseMoveBody(TPoint& diff);
	void MouseMoveBasicConsole(TPoint& diff);
	void MouseMoveDeepConsole(TPoint& diff);
	void MouseMoveUpperRight(TPoint& diff);
	void MouseMoveLowerRight(TPoint& diff);
	void MouseMoveUpperLeft(TPoint& diff);
	void MouseMoveLowerLeft(TPoint& diff);
};

