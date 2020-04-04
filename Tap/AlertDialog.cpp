#include "AlertDialog.h"
#include <DirectoryInterface.h>
#include "OkayHandler.h"

AlertDialog::AlertDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption) :
	TDialog(name, winClass, style, parent, commandShow, ins, mode)
{
	this->caption.Set(caption);
}

AlertDialog::~AlertDialog()
{
}

int AlertDialog::CompileView(TrecComPointer<ID2D1Factory1> fact)
{
	TString file = GetDirectoryWithSlash(CentralDirectories::cd_Executable);

	file.Append(L"Resources\\TextDialog.tml");

	TrecPointer<EventHandler> eh = TrecPointerKey::GetNewTrecPointerAlt<EventHandler, OkayHandler>(windowInstance);

	int returnable = TWindow::CompileView(file, eh);

	if (returnable)return returnable;

	if (!mainPage.Get())
		return 10;

	TrecPointer<TControl> control = mainPage->GetRootControl();
	TLayout* layout = dynamic_cast<TLayout*>(control.Get());

	assert (layout);

	textField = layout->GetLayoutChild(0, 0);

	TTextField* tf = dynamic_cast<TTextField*>(textField.Get());

	assert(tf);

	tf->SetText(caption);
	tf->LockText();

	return 0;
}
