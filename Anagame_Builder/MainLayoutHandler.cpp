#include "MainLayoutHandler.h"
#include <Page.h>


// Found on the Home Tab
TString on_LoadNewSolution(L"LoadNewSolution");
TString on_SaveFile(L"SaveFile");
TString on_SaveAll(L"SaveAllFiles");
TString on_NewFile(L"OnNewFile");
TString on_ImportFile(L"OnImportFile");
TString on_Print(L"OnPrint");

// Found on the Arena Tab
TString on_NewArena(L"OnNewArena");
TString on_Change3DColor(L"OnUpdateClearColor");
TString on_NewModel(L"OnNewModel");

// Found on the Document (Source Code) Tab
TString on_NewCodeFile(L"OnNewCodeFile");
TString on_ImportCode(L"OnImportCode");
TString on_ProcessCode(L"OnProcessCode");

MainLayoutHandler::MainLayoutHandler(TrecPointer<TInstance> ins) : EventHandler(ins)
{
	page = nullptr;

	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_LoadNewSolution);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnLoadNewSolution);

	enid.eventID = 1;
	enid.name.Set(on_SaveFile);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnSaveFile);

	enid.eventID = 2;
	enid.name.Set(on_SaveAll);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnSaveAllFiles);

	enid.eventID = 3;
	enid.name.Set(on_NewFile);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnNewFile);

	enid.eventID = 4;
	enid.name.Set(on_ImportFile);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnImportFile);

	enid.eventID = 5;
	enid.name.Set(on_Print);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnPrint);

	enid.eventID = 6;
	enid.name.Set(on_NewArena);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnNewArena);

	enid.eventID = 7;
	enid.name.Set(on_Change3DColor);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnUpdateClearColor);

	enid.eventID = 8;
	enid.name.Set(on_NewModel);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnNewModel);

	enid.eventID = 9;
	enid.name.Set(on_NewCodeFile);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnNewCodeFile);

	enid.eventID = 10;
	enid.name.Set(on_ImportCode);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnImportCode);

	enid.eventID = 11;
	enid.name.Set(on_ProcessCode);
	events.push_back(enid);
	calls.push_back(&MainLayoutHandler::OnProcessCode);


}

MainLayoutHandler::~MainLayoutHandler()
{
}

void MainLayoutHandler::Initialize(Page* page)
{
	if (!page)
		throw L"Error! Expected an actual Page Pointer to be provided!";

	this->page = page;


	rootControl = page->GetRootControl();

	TLayout* rootLayout = dynamic_cast<TLayout*>(rootControl.Get());

	assert(rootLayout);

	// Setting up the main Ribbon
	ribbon = rootLayout->GetLayoutChild(0, 0);
	AnafaceUI* rib = dynamic_cast<AnafaceUI*>(ribbon.Get());
	assert(rib);

	// First Tab in ribbon
	ribbon1 = rib->GetChildAt(0);

	TLayout* subLayout = dynamic_cast<TLayout*>(ribbon1.Get());
	assert(subLayout);
	subLayout = dynamic_cast<TLayout*>(subLayout->GetLayoutChild(0, 0).Get());
	assert(subLayout);
	solutionName = subLayout->GetLayoutChild(0, 0);
	assert(dynamic_cast<TTextField*>(solutionName.Get()));

	// Rest of Layout
	subLayout = dynamic_cast<TLayout*>(rootLayout->GetLayoutChild(0, 1).Get());
	assert(subLayout);							// SubLayout should now be a Grid
	classUI = subLayout->GetLayoutChild(0, 0);
	body = subLayout->GetLayoutChild(1, 0);
	outputPanel = subLayout->GetLayoutChild(0, 1);
	assert(dynamic_cast<AnafaceUI*>(classUI.Get()));
	assert(dynamic_cast<AnafaceUI*>(body.Get()));
	assert(dynamic_cast<AnafaceUI*>(outputPanel.Get()));

	// Set up the Arena Tab
	ribbon4 = rib->GetChildAt(3);
	subLayout = dynamic_cast<TLayout*>(ribbon4.Get());
	assert(subLayout);
	arenaStack1 = subLayout->GetLayoutChild(0, 0);
	arenaStack3 = subLayout->GetLayoutChild(2, 0);
	assert(dynamic_cast<TLayout*>(arenaStack1.Get()));
	assert(dynamic_cast<TLayout*>(arenaStack3.Get()));
	arenaStack1->setActive(false);
	arenaStack3->setActive(false);

	// Set up the Document Tab
	ribbon5 = rib->GetChildAt(5);
	subLayout = dynamic_cast<TLayout*>(ribbon5.Get());
	docStack2 = subLayout->GetLayoutChild(1, 0);
	assert(dynamic_cast<TLayout*>(docStack2.Get()));
	docStack2->setActive(false);
}

void MainLayoutHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{

	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		TControl* cont = eventAr[Rust].control;
		if (!cont) continue;

		EventArgs ea = cont->getEventArgs();


		int ea_id = ea.methodID;

		if (ea_id > -1 && ea_id < calls.Size())
		{
			(this->*calls[ea_id])(cont, ea);
		}
		cont->resetArgs();
	}
}

void MainLayoutHandler::OnLoadNewSolution(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnSaveFile(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnSaveAllFiles(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnNewFile(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnImportFile(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnPrint(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnNewArena(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnUpdateClearColor(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnNewModel(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnNewCodeFile(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnImportCode(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnProcessCode(TControl* tc, EventArgs ea)
{
}
