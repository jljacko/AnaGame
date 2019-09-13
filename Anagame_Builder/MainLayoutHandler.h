#pragma once
#include <EventHandler.h>
#include "BuilderApp.h"

class MainLayoutHandler;

typedef void (MainLayoutHandler::* LayoutCall)(TControl* tc, EventArgs ea);

class MainLayoutHandler : public EventHandler
{
public:
	MainLayoutHandler(TrecPointer<TInstance> ins);
	~MainLayoutHandler();

	void Initialize(Page* page);
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);
protected:
	Page* page;
	TDataArray<LayoutCall> calls;

	// Pointers to frequently access controls in the main layout
	TrecPointer<TControl> rootControl;
	bool init;
	TString fileAnaface;
	TDataArray<eventNameID> idMatch;

	D2D1_COLOR_F backColor;

	TrecPointer<TControl> ribbon;					// AnafaceUI
	TrecPointer<TControl> body;						// AnafaceUI
	TrecPointer<TControl> classUI;					// AnafaceUI
	TrecPointer<TControl> outputPanel;				// AnafaceUI

	// The first Ribbon Panel
	TrecPointer<TControl> ribbon1;					// TLayout
	TrecPointer<TControl> solutionName;			// TTextField

	// The Fourth Ribbon Panel (Arena)
	TrecPointer<TControl> ribbon4;					// TLayout
	TrecPointer<TControl> arenaStack1, arenaStack3;	// TLayout

	// The Fifth Ribbon Panel (Document/Source)
	TrecPointer<TControl> ribbon5;					// TLayout
	TrecPointer<TControl> docStack2;					// TLayout

	TDataArray<TrecPointer<BuilderApp>> ActiveDocuments;
	//TrecPointer<BuilderApp> currentDocument;

	// Found on the Home Tab
	void OnLoadNewSolution(TControl* tc, EventArgs ea);
	void OnSaveFile(TControl* tc, EventArgs ea);
	void OnSaveAllFiles(TControl* tc, EventArgs ea);
	void OnNewFile(TControl* tc, EventArgs ea);
	void OnImportFile(TControl* tc, EventArgs ea);
	void OnPrint(TControl* tc, EventArgs ea);

	// Found on the Arena Tab
	void OnNewArena(TControl* tc, EventArgs ea);
	void OnUpdateClearColor(TControl* tc, EventArgs ea);
	void OnNewModel(TControl* tc, EventArgs ea);

	// Found on the Document (Source Code) Tab
	void OnNewCodeFile(TControl* tc, EventArgs ea);
	void OnImportCode(TControl* tc, EventArgs ea);
	void OnProcessCode(TControl* tc, EventArgs ea);
};

