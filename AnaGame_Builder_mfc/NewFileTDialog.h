#pragma once

#include <TDialog.h>
#include "Solution.h"

#define NEW_FILE_METHOD_COUNT 2

class NewFileTDialog;

typedef void (NewFileTDialog::*methodArrayF)(TControl* tc, EventArgs ea);

class NewFileTDialog : public TDialog
{
public:
	NewFileTDialog();
	~NewFileTDialog();

	bool GetValidConstruction();

	void initializeControls();
	void MessageHandler()override;

	bool GetOkay();

	//TrecPointer<Solution> GetSolution();

	void InitializeControls();

private:
	TString fileHolder;
	bool constructionWorked, okay;
	methodArrayF ProjectMethods[NEW_FILE_METHOD_COUNT];

	TrecPointer<TComboBox> projType, projSol;
	TrecPointer<TTextField> nameBox;

	afx_msg void OnCancel(TControl* tc, EventArgs ea);
	afx_msg void OnOkay(TControl* tc, EventArgs ea);
};

