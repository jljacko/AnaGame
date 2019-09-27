#include "NameDialog.h"
#include <DirectoryInterface.h>
#include "OkayHandler.h"


NameDialog::NameDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption):
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->caption.Set(caption);
}

NameDialog::~NameDialog()
{
}

int NameDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(cd_Executable);

	file.Append(L"Resources\\TextDialog.tml");

	TrecPointer<EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<EventHandler, OkayHandler>(windowInstance);

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;

	TrecPointer<TControl> control = mainPage->GetRootControl();
	TLayout* layout = dynamic_cast<TLayout*>(control.Get());

	assert(layout);

	textField = layout->GetLayoutChild(0, 0);

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	tf->SetText(caption);
	tf->LockText();

	textField = layout->GetLayoutChild(0, 1);

	tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	//tf->SetText(caption);
	tf->UnlockText();

	userInput = TrecPointerKey::GetNewTrecPointer<TString>();

	return 0;
}

bool NameDialog::OnDestroy()
{
	bool ret = TDialog::OnDestroy();

	if (!textField.Get() || !userInput.Get())
		return ret;

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	// Set the Pointer String so that clients of this dialog will still have the data collected
	// even after this dialog is destroyed
	userInput->Set(tf->GetText());

	return ret;
}

TrecPointer<TString> NameDialog::GetText()
{
	return userInput;
}
