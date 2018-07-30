#pragma once
#include <TDialog.h>

#define NAME_METHOD_COUNT 2

class NameDialog;

typedef void (NameDialog::*methodArrayN)(TControl* tc, EventArgs ea);

class NameDialog: public TDialog
{
public:
	NameDialog();
	~NameDialog();

	bool GetValidConstruction();

	void InitializeControls();
	void MessageHandler()override;

	TString GetName();

	//void InitializeControls();

private:
	TString fileHolder;
	TString nameUsed;
	bool constructionWorked;

	TrecPointer<TTextField> text;

	methodArrayN NameMethods[NAME_METHOD_COUNT];
	void OnCancel(TControl* tc, EventArgs ea);
	void OnOkay(TControl* tc, EventArgs ea);
};

