#pragma once

#include <TDialog.h>
#include "Solution.h"

#define NEW_PROJECT_METHOD_COUNT 4

class NewProjectDialogAG;

typedef void (NewProjectDialogAG::*methodArray)(TControl* tc, EventArgs ea);

class NewProjectDialogAG : public TDialog
{
public:
	NewProjectDialogAG(TrecPointer<Solution> sol);
	~NewProjectDialogAG();

	//void onDraw() override;
	bool GetValidConstruction();

	void initializeControls();
	void MessageHandler()override;

	bool GetOkay();

	TrecPointer<Solution> GetSolution();

	void InitializeControls();

private:
	TString fileHolder;
	bool constructionWorked, okay;
	methodArray ProjectMethods[NEW_PROJECT_METHOD_COUNT];

	TrecPointer<Solution> sol;

	TrecPointer<TComboBox> projType, projSol;
	TrecPointer<TTextField> nameBox;
	TrecPointer<TTextField> nameBox2;
	TrecPointer<TControl> nameBox2Label;

	TString typeString;

	afx_msg void OnCancel(TControl* tc, EventArgs ea);
	afx_msg void OnOkay(TControl* tc, EventArgs ea);
	afx_msg void OnSolution(TControl*tc, EventArgs ea);
	afx_msg void OnProjectType(TControl* tc, EventArgs ea);
};

