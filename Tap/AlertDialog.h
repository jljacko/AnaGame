#pragma once
#include "TDialog.h"
class AlertDialog :	public TDialog
{
public:
	AlertDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption);
	~AlertDialog();
	int CompileView(TrecComPointer<ID2D1Factory1> fact);
private:
	TrecPointer<TControl> textField;
	TString caption;
};

