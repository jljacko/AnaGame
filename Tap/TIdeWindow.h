#pragma once
#include "TWindow.h"

typedef enum ide_page_type
{
	ide_page_type_body,
	ide_page_type_basic_console,
	ide_page_type_deep_console,
	ide_page_type_upper_right,
	ide_page_type_lower_right,
	ide_page_type_upper_left,
	ide_page_type_lower_left,
	ide_page_type_drag
}ide_page_type;


typedef enum anagame_page
{
	anagame_page_custom,
	anagame_page_file_node,
	anagame_page_command_prompt,
	anagame_page_code_explorer,
	anagame_page_object_explorer,
	anagame_page_code_file
};

class IDEPage;


class _TAP_DLL TIdeWindow :
	public TWindow
{
public:
	TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace);
	virtual int PrepareWindow()override;
	void OnLButtonUp(UINT nFlags, TPoint point);
	void OnMouseMove(UINT nFlags, TPoint point)override;
	void OnLButtonDown(UINT nFlags, TPoint point)override;

	void AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict = false);

	int CompileView(TString& file, TrecPointer<EventHandler> eh)override;

protected:
	UINT pageBarSpace;
	UINT mainViewSpace;

	virtual void DrawOtherPages()override;

	TrecSubPointer<Page, IDEPage> body;
	TrecSubPointer<Page, IDEPage> basicConsole;
	TrecSubPointer<Page, IDEPage> deepConsole;
	TrecSubPointer<Page, IDEPage> upperRight;
	TrecSubPointer<Page, IDEPage> lowerRight;
	TrecSubPointer<Page, IDEPage> upperLeft;
	TrecSubPointer<Page, IDEPage> lowerLeft;

	TrecSubPointer<Page, IDEPage> focusPage;

	TrecPointer<TBrush> panelbrush;
};

