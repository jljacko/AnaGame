#include <Windows.h>
#include "TDialog.h"
#include "TML_Reader_.h"
#include "AnafaceParser.h"
#include <Windows.h>
#include <d2d1.h>

// #include <afxglobals.h>

TDialog::TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins) : 
	TWindow(name, winClass, style | WS_POPUP | WS_DLGFRAME, parent, commandShow, ins)
{
}
