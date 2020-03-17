#include "SourceCodeApp2.h"

SourceCodeApp2::SourceCodeApp2(TrecSubPointer<TWindow, TIdeWindow> win): MiniApp(win)
{
}

void SourceCodeApp2::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
}

UINT SourceCodeApp2::Initialize()
{
	if (!win.Get() || !instance.Get())
		return 1;

	codeHandler = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, TCodeHandler>(instance);

	if (!codeHandler.Get())
		return 2;

	commandHandler = TrecPointerKey::GetNewSelfTrecSubPointer<EventHandler, TerminalHandler>(instance);
	if (!commandHandler.Get())
		return 3;

	win->AddNewPage(anagame_page_code_file, ide_page_type_body, TString(), TString(), TrecPointerKey::GetTrecPointerFromSub<EventHandler, TCodeHandler>(codeHandler));
	win->AddNewPage(anagame_page_command_prompt, ide_page_type_basic_console, TString(), TString(), TrecPointerKey::GetTrecPointerFromSub < EventHandler, TerminalHandler>(commandHandler));

	return 0;
}
