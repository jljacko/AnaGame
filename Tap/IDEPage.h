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
	IDEPageHolder(TString name, TrecPointer<DrawingBoard> rt, UINT barSpace, TrecPointer<EventHandler> handler, TrecPointer<TWindow> win, D2D1_RECT_F initLoc);
	TrecPointer<Page> GetBasePage();
	TrecSubPointer<Page, IDEPage> GetPage();

	TString GetName();

	D2D1_RECT_F GetLocation();
	D2D1_RECT_F SetLocation(const D2D1_RECT_F& newLoc);
	void Draw();
	void Move(TPoint& moveBy);

protected:
	void SetPage(TrecSubPointer<Page, IDEPage> p);
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
	IDEPage(ide_page_type type, UINT barSpace, TrecPointer<DrawingBoard> board);

	void SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window);
	void SetResources(TrecPointer<TInstance> in, TrecPointer<TWindow> window, TrecPointer<TWindowEngine> engine);

	void MoveBorder(float& magnitude, page_move_mode mode);


	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut) override;
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)override;
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut) override;
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut) override;
	afx_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine>) override;

public:
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp();

	void Draw(TrecPointer<TBrush> color, TWindowEngine* twe = nullptr);
	void SetLink(TrecSubPointer<Page, IDEPage> p, ide_page_type t);


	// Methods for the Drag version of the Page
	TString GetName();
	void SetNewParentPage(TrecPointer<Page> p);

	// Methods for Parent Pages
	void AddNewPage(TrecPointer<IDEPageHolder> pageHolder);
	TrecPointer<Page> AddNewPage(TrecPointer<TInstance> ins, TrecPointer<TWindow> win, TString name, TrecPointer<EventHandler> h);
	void RemovePage(TrecPointer<IDEPageHolder> pageHolder);

	bool IsDrawing();

protected:

	TrecSubPointer<Page, IDEPage> body;
	TrecSubPointer<Page, IDEPage> basicConsole;
	TrecSubPointer<Page, IDEPage> deepConsole;
	TrecSubPointer<Page, IDEPage> upperRight;
	TrecSubPointer<Page, IDEPage> lowerRight;
	TrecSubPointer<Page, IDEPage> upperLeft;
	TrecSubPointer<Page, IDEPage> lowerLeft;

	TDataArray<TrecPointer<IDEPageHolder>> pages;
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

