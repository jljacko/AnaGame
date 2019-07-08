#include "stdafx.h"
#include "TDllApp.h"

TDllApp::TDllApp()
{
	library = NULL;
	onDraw = nullptr;
	onMessage = nullptr;
}

TDllApp::~TDllApp()
{
	ReleaseLibrary();
}

bool TDllApp::Initialize(TMap<TString>& properties, TString& directory)
{
    TString* prop = properties.retrieveEntry(TString(L"DirectoryPath")).get();

	if (!prop)
		return false;
	if (!SetDllDirectoryW(directory)) return false;

	library = LoadLibraryW(prop->GetBuffer());
	prop->ReleaseBuffer();

	if (!library)
		return false;

	onDraw = (DRAW)GetProcAddress(library, "OnDraw");
	onMessage = (MESSAGE)GetProcAddress(library, "OnMessage");

	if (onDraw && onMessage)
		return true;
	return false;
}

void TDllApp::ReleaseLibrary()
{
	if (library)
		FreeLibrary(library);

	library = NULL;
}
