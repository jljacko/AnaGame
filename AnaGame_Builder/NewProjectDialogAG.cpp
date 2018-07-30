#include "stdafx.h"
#include "NewProjectDialogAG.h"
#include <DirectoryInterface.h>

TString on_Cancel = L"onCancel";
TString on_okay = L"onOkay";
TString on_solStat = L"onSolutionChange";
TString on_projType = L"onProjectType";

NewProjectDialogAG::NewProjectDialogAG(TrecPointer<Solution> sol) : TDialog(600,700)
{
	fileHolder = GetDirectoryWithSlash(CentralDirectories::cd_AppData);

	for (UINT c = 0; c < NEW_PROJECT_METHOD_COUNT; c++)
		ProjectMethods[c];

	fileHolder += L"Anagame\\NewProject.tml";
	constructionWorked = setAnaface(fileHolder);
	setName(TString(L"New Project"));

	ProjectMethods[0] = &NewProjectDialogAG::OnCancel;
	ProjectMethods[1] = &NewProjectDialogAG::OnOkay;
	ProjectMethods[2] = &NewProjectDialogAG::OnSolution;
	ProjectMethods[3] = &NewProjectDialogAG::OnProjectType;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_Cancel;
	idMatch.push_back(enid);

	enid.eventID = 1;
	enid.name = on_okay;
	idMatch.push_back(enid);

	enid.eventID = 2;
	enid.name = on_solStat;
	idMatch.push_back(enid);

	enid.eventID = 3;
	enid.name = on_projType;
	idMatch.push_back(enid);

	this->sol = sol;
}


NewProjectDialogAG::~NewProjectDialogAG()
{
}

//void NewProjectDialogAG::onDraw()
//{
//}

bool NewProjectDialogAG::GetValidConstruction()
{
	return constructionWorked;
}

void NewProjectDialogAG::initializeControls()
{
}

void NewProjectDialogAG::MessageHandler()
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
		if (e_id > -1 && e_id < NEW_PROJECT_METHOD_COUNT)
		{
			// call method
			if (ProjectMethods[e_id])
				(this->*ProjectMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

bool NewProjectDialogAG::GetOkay()
{
	return okay;
}

TrecPointer<Solution> NewProjectDialogAG::GetSolution()
{
	return sol;
}

void NewProjectDialogAG::InitializeControls()
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

	TrecPointer<TControl> comboChild = rootLayout->GetLayoutChild(0, 3);
	try
	{
		projType = dynamic_cast<TComboBox*>(comboChild.get());
		comboChild = rootLayout->GetLayoutChild(0, 4);
		projSol = dynamic_cast<TComboBox*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}

	TrecPointer<TLayout> lay;
	comboChild = rootLayout->GetLayoutChild(0, 1);
	try
	{
		lay = dynamic_cast<TLayout*>(comboChild.get());
		comboChild = lay->GetLayoutChild(1, 0);
		nameBox = dynamic_cast<TTextField*>(comboChild.get());

		comboChild = rootLayout->GetLayoutChild(0, 2);
		lay = dynamic_cast<TLayout*>(comboChild.get());

		nameBox2Label = lay->GetLayoutChild(0, 0);
		comboChild = lay->GetLayoutChild(1, 0);
		nameBox2 = dynamic_cast<TTextField*>(comboChild.get());
	}
	catch (std::bad_cast& e)
	{

		return;
	}


}

void NewProjectDialogAG::OnCancel(TControl * tc, EventArgs ea)
{
	okay = false;
	DestroyWindow(windowHandle);
}

void NewProjectDialogAG::OnOkay(TControl * tc, EventArgs ea)
{
	UINT type = 0;

	if (typeString.Compare(L"Anagame Application"))
		type = PROJECT_ANAGAME_TAP;
	else if (typeString.Compare(L"Desktop Application"))
		type = PROJECT_APPLICATION;
	else if (typeString.Compare(L"Front-End App (Web-Site)"))
		type = PROJECT_FRONT_END;
	else if (typeString.Compare(L"Back-End App (Web-Service)"))
		type = PROJECT_BACK_END;
	else if (typeString.Compare(L"Programming Language"))
		type = PROJECT_PROG_LANG;
	else if (typeString.Compare(L"Anagame Extension"))
		type = PROJECT_ANAGAME_EXT;
	else if (typeString.Compare(L"Mobile Application"))
		type = PROJECT_MOBILE;
	else if (typeString.Compare(L"Video"))
		type = PROJECT_ANAGAME_VIDEO;
	else if (typeString.Compare(L"Library"))
		type = PROJECT_LIBRARY;




	
	okay = true;
	TString name;
	if (nameBox2.get())
	{
		name = nameBox2->GetText();
	}
	if (!name.GetLength())
		name = L"Solution";

	if (!sol.get())
	{
		sol = new Solution();
		sol->SetName(name);
	}

	TrecPointer<Project> newProj;
	newProj = new Project(type);
	newProj->SetName(name);
	if (nameBox.get() && nameBox->GetText().GetLength())
	{
		newProj->SetName(nameBox->GetText());
	}
	else
	{
		newProj->SetName(TString(L"Project"));
	}

	sol->AddProject(newProj);

	DestroyWindow(windowHandle);
}

void NewProjectDialogAG::OnSolution(TControl * tc, EventArgs ea)
{
	if (!tc)
		return;
	if (!nameBox2.get() || !nameBox2Label.get())
		return;
	if (!ea.text.Compare(L"New Solution"))
	{
		nameBox2->setActive(true);
		nameBox2Label->setActive(true);
	}
	else
	{
		nameBox2->setActive(false);
		nameBox2Label->setActive(false);
	}
}

void NewProjectDialogAG::OnProjectType(TControl * tc, EventArgs ea)
{
	typeString = ea.text;

}
