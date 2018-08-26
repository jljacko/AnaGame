#include "stdafx.h"
#include "NameDialog.h"
#include <DirectoryInterface.h>

TString on_n_Cancel = L"OnCancel";
TString on_n_okay = L"OnOkay";

NameDialog::NameDialog():TDialog(200,350)
{
	fileHolder = GetDirectoryWithSlash(CentralDirectories::cd_AppData);

	//for (UINT c = 0; c < NEW_FILE_METHOD_COUNT; c++)
	//	ProjectMethods[c];

	fileHolder += L"Anagame\\New Arena Dialog.tml";
	setName(TString(L"Set Name"));
	constructionWorked = setAnaface(fileHolder);

	for (UINT c = 0; c < NAME_METHOD_COUNT; c++)
		NameMethods[c] = 0;

	NameMethods[0] = &NameDialog::OnCancel;
	NameMethods[1] = &NameDialog::OnOkay;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_n_Cancel;
	idMatch.push_back(enid);

	//eventNameID enid;

	enid.eventID = 1;
	enid.name = on_n_okay;
	idMatch.push_back(enid);
}


NameDialog::~NameDialog()
{
	text = nullptr;
}

bool NameDialog::GetValidConstruction()
{
	return constructionWorked;
}

void NameDialog::InitializeControls()
{
	try
	{
		TrecPointer<TLayout> lay = dynamic_cast<TLayout*>(pointer.get());
		lay = dynamic_cast<TLayout*>(lay->GetLayoutChild(0, 1).get());
		text = dynamic_cast<TTextField*>(lay->GetLayoutChild(1, 0).get());
	}
	catch (std::bad_cast& e)
	{

	}
}

void NameDialog::MessageHandler()
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
		if (e_id > -1 && e_id < NAME_METHOD_COUNT)
		{
			// call method
			if (NameMethods[e_id])
				(this->*NameMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

TString NameDialog::GetName()
{
	return nameUsed;
}

void NameDialog::OnCancel(TControl * tc, EventArgs ea)
{
	nameUsed = L"";
	ShutdownWindow();
}

void NameDialog::OnOkay(TControl * tc, EventArgs ea)
{
	if (text.get())
		nameUsed = text->GetText();
	if (nameUsed.GetLength())
		ShutdownWindow();
}
