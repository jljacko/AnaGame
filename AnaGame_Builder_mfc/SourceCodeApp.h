#pragma once
#include "BuilderApp.h"
#include <TInterpretor.h>

#define SOURCE_CODE_APP_COUNT 0

class SourceCodeApp;

typedef void (SourceCodeApp::*SCBuilderEvents)(TControl* tc, EventArgs ea);

class SourceCodeApp :
	public BuilderApp
{
public:
	SourceCodeApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, CAnaGameBuilderDoc* r, HWND w);
	~SourceCodeApp();

	bool InitializeControls() override;

	void SetContents(TFile& content);
	virtual void OnSave() override;

	virtual void OnShow() override;
	virtual void onHide() override;

	UINT ProcessIntLanguage();
protected:
	// Refactor code
	void PrepLanguage(TFile& sourceCode);


	// Interpretors, incase code needs to be run or compiled
	TInterpretor* interpret;
	TrecPointer<TTextField> lines;
	TrecPointer<TTextField> code;

	D2D1::ColorF col_keyword, col_type_id, col_var_id, col_fun_id;
	SCBuilderEvents sc_events[SOURCE_CODE_APP_COUNT];

	
};

