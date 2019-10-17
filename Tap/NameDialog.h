#pragma once
#include "TDialog.h"
class NameDialog :	public TDialog
{
public:
	NameDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption);
	~NameDialog();
	int CompileView(TrecComPointer<ID2D1Factory1> fact);
	bool OnDestroy() override;

	TrecPointer<TString> GetText();
private:
	TrecPointer<TControl> textField;
	TString caption;
	TrecPointer<TString> userInput;
};

