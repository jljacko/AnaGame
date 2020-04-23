#pragma once
#include "TWindow.h"
#include "TEnvironment.h"


typedef enum class ide_page_type
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


typedef enum class anagame_page
{
	anagame_page_custom,
	anagame_page_file_node,
	anagame_page_command_prompt,
	anagame_page_code_explorer,
	anagame_page_object_explorer,
	anagame_page_code_file,
	anagame_page_arena,
	anagame_page_camera
};

class IDEPage;
class IDEPageHolder;
class MiniApp;

class _TAP_DLL TIdeWindow :
	public TWindow
{
public:
	TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, UINT mainViewSpace, UINT pageBarSpace);
	virtual int PrepareWindow()override;
	void OnLButtonUp(UINT nFlags, TPoint point);
	void OnMouseMove(UINT nFlags, TPoint point)override;
	void OnLButtonDown(UINT nFlags, TPoint point)override;

	void AddNewMiniApp(TrecPointer<MiniApp> app);

	TrecSubPointer<Page, IDEPage> AddNewPage(anagame_page pageType, ide_page_type pageLoc, TString name, TString tmlLoc, TrecPointer<EventHandler> handler, bool pageTypeStrict = false);
	TrecSubPointer<Page, IDEPage> AddPage(anagame_page pageType, ide_page_type pageLoc, TString name);


	int CompileView(TString& file, TrecPointer<EventHandler> eh)override;

	void SetCurrentHolder(TrecPointer<IDEPageHolder> holder);

	void SetEnvironment(TrecPointer<TEnvironment> env);
	TrecPointer<TFileShell> GetEnvironmentDirectory();

protected:
	UINT pageBarSpace;
	UINT mainViewSpace;


	TrecPointer<IDEPageHolder> currentHolder;

	TrecPointer<TEnvironment> environment;


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

	TDataArray<TrecPointer<MiniApp>> apps;
};

