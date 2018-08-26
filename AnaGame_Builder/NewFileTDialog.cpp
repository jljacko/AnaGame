#include "stdafx.h"
#include "NewFileTDialog.h"
#include <DirectoryInterface.h>

TString on_nf_Cancel = L"onCancel";
TString on_nf_okay = L"onOkay";

NewFileTDialog::NewFileTDialog() : TDialog(600, 700)
{
	fileHolder = GetDirectoryWithSlash(CentralDirectories::cd_AppData);

	for (UINT c = 0; c < NEW_FILE_METHOD_COUNT; c++)
		ProjectMethods[c] = 0;

	fileHolder += L"Anagame\\NewFile.tml";
	setName(TString(L"New File"));
	constructionWorked = setAnaface(fileHolder);


	ProjectMethods[0] = &NewFileTDialog::OnCancel;
	ProjectMethods[1] = &NewFileTDialog::OnOkay;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_nf_Cancel;
	idMatch.push_back(enid);

	//eventNameID enid;

	enid.eventID = 1;
	enid.name = on_nf_okay;
	idMatch.push_back(enid);
}


NewFileTDialog::~NewFileTDialog()
{
}

bool NewFileTDialog::GetValidConstruction()
{
	return constructionWorked;
}

void NewFileTDialog::initializeControls()
{
	TrecPointer<TLayout> rootLayout;
	if (!pointer.get())
	{

		return;
	}
	try
	{
		rootLayout = dynamic_cast<TLayout*>(pointer.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

	TrecPointer<TControl> comboChild = rootLayout->GetLayoutChild(0, 2);
	TrecPointer<TLayout> lay;
	try
	{
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		projType = dynamic_cast<TComboBox*>(comboChild.get());
		comboChild = rootLayout->GetLayoutChild(0, 3);
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		projSol = dynamic_cast<TComboBox*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

	comboChild = rootLayout->GetLayoutChild(0, 1);
	try
	{
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		nameBox = dynamic_cast<TTextField*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

}

void NewFileTDialog::MessageHandler()
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
		if (e_id > -1 && e_id < NEW_FILE_METHOD_COUNT)
		{
			// call method
			if (ProjectMethods[e_id])
				(this->*ProjectMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

bool NewFileTDialog::GetOkay()
{
	return okay;
}

void NewFileTDialog::InitializeControls()
{
	TrecPointer<TLayout> rootLayout;
	if (!pointer.get())
	{

		return;
	}
	try
	{
		rootLayout = dynamic_cast<TLayout*>(pointer.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

	TrecPointer<TControl> comboChild = rootLayout->GetLayoutChild(0, 2);
	TrecPointer<TLayout> lay;
	try
	{
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		projType = dynamic_cast<TComboBox*>(comboChild.get());
		comboChild = rootLayout->GetLayoutChild(0, 3);
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		projSol = dynamic_cast<TComboBox*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

	comboChild = rootLayout->GetLayoutChild(0, 1);
	try
	{
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		nameBox = dynamic_cast<TTextField*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}
}

void NewFileTDialog::OnCancel(TControl * tc, EventArgs ea)
{
	okay = false;
	ShutdownWindow();
}

void NewFileTDialog::OnOkay(TControl * tc, EventArgs ea)
{
	okay = true;
	ShutdownWindow();
}
