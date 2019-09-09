#pragma once
#include "Tap_dll.h"
#include "TObject.h"
#include <TString.h>
#include <AnafaceUI.h>
#include "AnafaceParser.h"
#include "TWindow.h"
#include <TrecReference.h>



class _TAP_DLL TDialog : public TWindow
{
public:
	TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins);
};

