#pragma once
#include <TDialog.h>
#include <TSpreadSheet.h>

#define MODEL_METHOD_COUNT 2

class ModelDialog;

typedef void (ModelDialog::*methodArrayM)(TControl* tc, EventArgs ea);

class ModelDialog:public TDialog
{
public:
	ModelDialog();
	~ModelDialog();

	bool GetValidConstruction();

	void InitializeControls();
	void MessageHandler()override;

	bool GetOkay();
	TString GetIndexString();
	TString GetVertexString();
	TString GetModelName();

private:
	TString fileHolder;
	bool constructionWorked, ok;

	TrecPointer<TTextField> nameField, indexField;
	TrecPointer<TSpreadSheet> dataField;

	methodArrayM modelDialMethods[MODEL_METHOD_COUNT];
	void OnCancel(TControl* tc, EventArgs ea);
	void OnOkay(TControl* tc, EventArgs ea);
};

