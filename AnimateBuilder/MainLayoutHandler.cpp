#include "MainLayoutHandler.h"
#include <Page.h>
#include "ArenaApp.h"
#include <TDialog.h>
#include "SourceCodeApp.h"


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
	for (UINT Rust = 0; Rust < ActiveDocuments.Size(); Rust++)
	{
		ActiveDocuments[Rust].Delete();
	}
}

void MainLayoutHandler::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnRButtonUp(nFlags, point, mOut);
}

void MainLayoutHandler::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnLButtonDown(nFlags, point, mOut);
}

void MainLayoutHandler::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnRButtonDown(nFlags, point, mOut);
}

void MainLayoutHandler::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnMouseMove(nFlags, point, mOut);
}

void MainLayoutHandler::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnLButtonDblClk(nFlags, point, mOut);
}

void MainLayoutHandler::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	if (currentDocument.Get())
		currentDocument->OnLButtonUp(nFlags, point, mOut);
}

bool MainLayoutHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	if (currentDocument.Get())
		return currentDocument->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
	return false;
}

void MainLayoutHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get())
		throw L"Error! Expected an actual Page Pointer to be provided!";

	this->page = page;

	instance = page->GetInstance();

	rootControl = page->GetRootControl();


	// Setting up the main Ribbon
	AnafaceUI* rib = dynamic_cast<AnafaceUI*>(rootControl.Get());
	assert(rib);

	// First Tab in ribbon
	ribbon1 = rib->GetChildAt(0);

	TLayout* subLayout = dynamic_cast<TLayout*>(ribbon1.Get());
	assert(subLayout);
	subLayout = dynamic_cast<TLayout*>(subLayout->GetLayoutChild(0, 0).Get());
	assert(subLayout);
	solutionName = subLayout->GetLayoutChild(0, 0);
	assert(dynamic_cast<TTextField*>(solutionName.Get()));


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

		/*if (ea_id > -1 && ea_id < calls.Size())
		{
			(this->*calls[ea_id])(cont, ea);
		}
		else if (ea_id == -1 && ea.control == body.Get() && ea.eventType == On_sel_change)
		{
			OnSwitchTab(cont, ea);
		}*/
		cont->resetArgs();
	}
}

void MainLayoutHandler::Draw()
{
	if (currentDocument.Get())
		currentDocument->Draw();
}

void MainLayoutHandler::OnSwitchTab(TControl* tc, EventArgs ea)
{
	if (ea.arrayLabel >= 0 && ea.arrayLabel < ActiveDocuments.Size())
	{
		if (currentDocument.Get())
			currentDocument->onHide();
		currentDocument = ActiveDocuments[ea.arrayLabel];
		if (currentDocument.Get())
			currentDocument->OnShow();
	}
}

void MainLayoutHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

void MainLayoutHandler::OnLoadNewSolution(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnSaveFile(TControl* tc, EventArgs ea)
{
	if (currentDocument.Get())
		currentDocument->OnSave();
}

void MainLayoutHandler::OnSaveAllFiles(TControl* tc, EventArgs ea)
{
	for (UINT Rust = 0; Rust < ActiveDocuments.Size(); Rust++)
	{
		if (ActiveDocuments[Rust].Get())
			ActiveDocuments[Rust]->OnSave();
	}
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
	/*TString dialog(L"Enter a name for your Arena!");
	TString arenaName(ActivateNameDialog(instance, page->GetWindowHandle()->GetWindowHandle(), dialog));

	if (!arenaName.GetSize())
		return;

	currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniHandler, ArenaApp>(body, outputPanel, classUI, app, arenaName);
	ActiveDocuments.push_back(currentDocument);
	currentDocument->InitializeControls();
	currentDocument->OnShow();
	if (arenaStack1.Get())
		arenaStack1->setActive(true);
	if (arenaStack3.Get())
		arenaStack3->setActive(true);*/
}

void MainLayoutHandler::OnUpdateClearColor(TControl* tc, EventArgs ea)
{
	ArenaApp* arApp = nullptr;
	if (currentDocument.Get())
		arApp = dynamic_cast<ArenaApp*>(currentDocument.Get());

	if (!arApp) return;

	CHOOSECOLORW colorPicker;
	ZeroMemory(&colorPicker, sizeof(colorPicker));

	if (!ChooseColorW(&colorPicker)) return;

	COLORREF co = colorPicker.rgbResult;
	D2D1_COLOR_F color;

	color.b = static_cast<float>(GetBValue(co)) / 255.0f;
	color.r = static_cast<float>(GetRValue(co)) / 255.0f;
	color.g = static_cast<float>(GetGValue(co)) / 255.0f;
	color.a = 1.0f;
	arApp->SetColor(color);
}

void MainLayoutHandler::OnNewModel(TControl* tc, EventArgs ea)
{

}

void MainLayoutHandler::OnNewCodeFile(TControl* tc, EventArgs ea)
{
	/*currentDocument = TrecPointerKey::GetNewSelfTrecPointerAlt<MiniHandler, SourceCodeApp>(body, outputPanel, classUI, app);
	ActiveDocuments.push_back(currentDocument);
	currentDocument->InitializeControls();
	currentDocument->OnShow();*/
}

void MainLayoutHandler::OnImportCode(TControl* tc, EventArgs ea)
{
}

void MainLayoutHandler::OnProcessCode(TControl* tc, EventArgs ea)
{
}

bool MainLayoutHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;
	return message->GetHandlerType() == handler_type_other;
}
