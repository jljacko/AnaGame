#pragma once
#include <TDialog.h>

#define MODEL_METHOD_COUNT 0

class ModelDialog:public TDialog
{
public:
	ModelDialog();
	~ModelDialog();

	bool GetValidConstruction();

	void InitializeControls();
	void MessageHandler()override;

private:
	TString fileHolder;
	bool constructionWorked;
};

