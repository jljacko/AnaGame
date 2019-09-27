#pragma once
#include "Tap_dll.h"
#include "TObject.h"
#include <TString.h>
#include <AnafaceUI.h>
#include "AnafaceParser.h"
#include "TWindow.h"
#include <TrecReference.h>

typedef enum TDialogMode
{
	dialog_mode_free,		// Other widnows could still be interacted with
	dialog_mode_soft_modal,	// Other Windows will not respond to user interaction, but they could still be moved around
	dialog_mode_hard_model	// Other Windows will not respond to users
}TDialogMode;

class _TAP_DLL TDialog : public TWindow
{
public:
	TDialog(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins, TDialogMode mode);
	virtual ~TDialog();
	int PrepareWindow() override;
	bool OnDestroy() override;

	virtual int CompileView(TrecComPointer<ID2D1Factory1> fact) = 0;

	void Run();

protected:
	TDialogMode dialogMode;
	bool breakLoop;
};

_TAP_DLL void ActivateAlertDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption);

_TAP_DLL TString ActivateNameDialog(TrecPointer<TInstance> ins, HWND parent, TString& caption);