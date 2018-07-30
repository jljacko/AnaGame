#include "stdafx.h"
#include "ModelDialog.h"
#include <DirectoryInterface.h>

ModelDialog::ModelDialog(): TDialog(600,900)
{
	fileHolder = GetDirectoryWithSlash(CentralDirectories::cd_AppData);

	//for (UINT c = 0; c < NEW_FILE_METHOD_COUNT; c++)
	//	ProjectMethods[c];

	fileHolder += L"Anagame\\NewModel.tml";
	setName(TString(L"Set Name"));
	constructionWorked = setAnaface(fileHolder);
}


ModelDialog::~ModelDialog()
{
}

bool ModelDialog::GetValidConstruction()
{
	return constructionWorked;
}

void ModelDialog::InitializeControls()
{

}

void ModelDialog::MessageHandler()
{
}
