#include "stdafx.h"
#include "TApp.h"
#include "TDllApp.h"

TApp::TApp()
{
}

TApp::~TApp()
{
}

TApp* TApp::Initialize(TFile& file)
{
	if(!file.IsOpen())
		return nullptr;

	TString line;

	TMap<TString> properties;

	while (file.ReadString(line))
	{
		auto pair = line.split(TString(L"="));
		if (pair->Count() == 2)
		{
			TString prop = pair->ElementAt(0).get();
			TString* value = pair->ElementAt(1).get();
			prop.Trim();
			value->Trim();
			properties.addEntry(prop, TrecPointer<TString>(value));
		}
	}

	TString* appType = properties.retrieveEntry(TString(L"AppType")).get();

	TDllApp* app = nullptr;
	if (!appType)
		return nullptr;
	if (!appType->Compare(L"DLL_App"))
	{
		app = new TDllApp();
	}

	

	if (app && !app->Initialize(properties,file.GetFileDirectory()))
	{
		delete app;
		app = nullptr;
	}
	return app;
}
