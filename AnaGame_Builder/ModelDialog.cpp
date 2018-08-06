#include "stdafx.h"
#include "ModelDialog.h"
#include <DirectoryInterface.h>

TString on_m_Cancel = L"OnCancel";
TString on_m_okay = L"OnOkay";

ModelDialog::ModelDialog(): TDialog(600,900)
{
	ok = false;

	fileHolder = GetDirectoryWithSlash(CentralDirectories::cd_AppData);

	//for (UINT c = 0; c < NEW_FILE_METHOD_COUNT; c++)
	//	ProjectMethods[c];

	fileHolder += L"Anagame\\NewModel.tml";
	setName(TString(L"Set Name"));
	constructionWorked = setAnaface(fileHolder);

	for (UINT c = 0; c < MODEL_METHOD_COUNT; c++)
		modelDialMethods[c] = 0;

	modelDialMethods[0] = &ModelDialog::OnCancel;
	modelDialMethods[1] = &ModelDialog::OnOkay;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_m_Cancel;
	idMatch.push_back(enid);

	//eventNameID enid;

	enid.eventID = 1;
	enid.name = on_m_okay;
	idMatch.push_back(enid);
}


ModelDialog::~ModelDialog()
{
	nameField = nullptr;
	indexField = nullptr;
	dataField = nullptr;
}

bool ModelDialog::GetValidConstruction()
{
	return constructionWorked;
}

void ModelDialog::InitializeControls()
{
	try
	{
		TrecPointer<TLayout> core = dynamic_cast<TLayout*>(pointer.get());
		if (!core.get())
			return;
		TrecPointer<TLayout> gal = dynamic_cast<TLayout*>(core->GetLayoutChild(0, 0).get());
		if (!gal.get())
			return;
		nameField = dynamic_cast<TTextField*>(gal->GetLayoutChild(1, 0).get());

		gal = dynamic_cast<TLayout*>(core->GetLayoutChild(0, 8).get());
		if (!gal.get())
			return;

		indexField = dynamic_cast<TTextField*>(gal->GetLayoutChild(1, 0).get());

		TrecPointer<AnafaceUI> aui = dynamic_cast<AnafaceUI*>(core->GetLayoutChild(0, 7).get());

		gal = dynamic_cast<TLayout*>(aui->GetCurrentChild().get());
		if (!gal.get())
			return;
		dataField = dynamic_cast<TSpreadSheet*>(gal->GetLayoutChild(1, 0).get());

	}
	catch (std::bad_cast& e)
	{

	}
}

void ModelDialog::MessageHandler()
{
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < EventCred.Size(); c++)
	{
		tc = EventCred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < MODEL_METHOD_COUNT)
		{
			// call method
			if (modelDialMethods[e_id])
				(this->*modelDialMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

bool ModelDialog::GetOkay()
{
	return ok;
}

TString ModelDialog::GetIndexString()
{
	if (indexField.get())
		return indexField->GetText();
	return TString();
}

TString ModelDialog::GetVertexString()
{
	if (dataField.get())
		return dataField->GetData();
	return TString();
}

void ModelDialog::OnCancel(TControl * tc, EventArgs ea)
{
	DestroyWindow(windowHandle);
}

void ModelDialog::OnOkay(TControl * tc, EventArgs ea)
{
	ok = true;
	DestroyWindow(windowHandle);
}