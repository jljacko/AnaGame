#include <Windows.h>
#include "TDialog.h"
#include "TML_Reader_.h"
#include "AnafaceParser.h"
#include <Windows.h>
#include <d2d1.h>
#include "TInstance.h"

#include "AlertDialog.h"
#include "NameDialog.h"

// #include <afxglobals.h>

TDialog::TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode) :
	TWindow(name, winClass, style | WS_POPUP | WS_DLGFRAME, parent, commandShow, ins)
{
	dialogMode = mode;
	breakLoop = false;
}

TDialog::~TDialog()
{
}

int TDialog::PrepareWindow()
{
	if (!currentWindow)
		return 1;
	if (parent)
	{
		if (windowInstance.Get() && dialogMode == TDialogMode::dialog_mode_soft_modal)
		{
			windowInstance->LockWindow(parent);
		}
		else if (dialogMode == TDialogMode::dialog_mode_hard_model)
		{
			EnableWindow(parent, FALSE);
		}
	}
	return TWindow::PrepareWindow();
}

bool TDialog::OnDestroy()
{
	bool returnable = TWindow::OnDestroy();
	if (returnable && parent)
	{
		if (windowInstance.Get() && dialogMode == TDialogMode::dialog_mode_soft_modal)
		{
			windowInstance->UnlockWindow(parent);
		}
		else if (dialogMode == TDialogMode::dialog_mode_hard_model)
		{
			EnableWindow(parent, TRUE);
		}
	}
	
	return breakLoop = returnable;
}

void TDialog::Run()
{
	MSG msg;
	while (!breakLoop && GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void ActivateAlertDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption)
{
	//  UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode, TString& caption;
	TString name(L"Alert");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;
	// HWND parent
	// 0
	// ins
	// dialog_mode_hard_modal

	TrecPointer<TWindow> dialog = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, AlertDialog>(name, winClass, style, parent, 10, ins, TDialogMode::dialog_mode_hard_model, caption);
	dynamic_cast<AlertDialog*>(dialog.Get())->CompileView(ins->GetFactory());

	dialog->PrepareWindow();

	dynamic_cast<AlertDialog*>(dialog.Get())->Run();
}

TString ActivateNameDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption)
{
	TString name(L"Alert");
	TString winClass(L"Dialog");
	UINT style = WS_OVERLAPPEDWINDOW;
	// HWND parent
	// 0
	// ins
	// dialog_mode_hard_modal


	TrecPointer<TWindow> dialog = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, NameDialog>(name, winClass, style, parent, 10, ins, TDialogMode::dialog_mode_hard_model, caption);
	dynamic_cast<NameDialog*>(dialog.Get())->CompileView(ins->GetFactory());

	TrecPointer<TString> sharedData = dynamic_cast<NameDialog*>(dialog.Get())->GetText();

	dialog->PrepareWindow();

	dynamic_cast<NameDialog*>(dialog.Get())->Run();

	return TString(sharedData.Get());
}
