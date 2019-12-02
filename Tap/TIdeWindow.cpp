#include "TIdeWindow.h"

TIdeWindow::TIdeWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins): 
	TWindow(name, winClass, style | WS_MAXIMIZE, parent, commandShow, ins)
{

}
