#include "stdafx.h"
#include "EnvManager.h"
#include <DirectoryInterface.h>
#include <TFile.h>

// Holds list of Languages and their file extensions
static TDataArray<LangNames> languageList;


void checkLanguages()
{
	// Location of the language folder
	TString languageFolder = GetDirectoryWithSlash(cd_Executable) + TString(L"Languages");

	if (languageList.Size())
		return;

	TFile languageLister;
	CFileException ex;
	languageLister.Open(languageFolder + TString(L"\\languages.properties"), CFile::modeRead, &ex);

	if (!languageLister.IsOpen())
	{
		char errorBuf[100];
		ex.GetErrorMessage((LPTSTR)errorBuf, 99);
		return;
	}
	TString line;
	while (languageLister.ReadString(line))
	{
		line.Trim();
		TrecPointer<TArray<TString>> sep = line.split(TString(L":"));

		if (sep->Count() != 2)
			continue;
		LangNames ln;
		ln.language = sep->ElementAt(0).get();
		sep = sep->ElementAt(1)->split(TString(L";"));

		for (UINT rust = 0; rust < sep->Count(); rust++)
		{
			if (!sep->ElementAt(rust).get())
				continue;
			sep->ElementAt(rust)->Trim();
			ln.fileExtensions.push_back(sep->ElementAt(rust).get());
		}
		languageList.push_back(ln);
	}

	languageLister.Close();
}

const TDataArray<LangNames>* const retrieveLanguages()
{
	return &languageList;
}

TString retrieveLanguageByExtension(TString ext)
{
	for (UINT c = 0; c < languageList.Size(); c++)
	{
		for (UINT Rust = 0; Rust < languageList[c].fileExtensions.Size(); Rust++)
		{
			if (languageList[c].fileExtensions[Rust] == ext)
				return languageList[c].language;
		}
	}
	return TString();
}

TString retrieveLanguageByIndex(UINT index)
{
	if (index < languageList.Size())
	{
		return languageList[index].language;
	}
	return TString();
}

TDataArray<TString> retrieveExtensionsByIndex(UINT index)
{
	if (index < languageList.Size())
	{
		return languageList[index].fileExtensions;
	}
	return TDataArray<TString>();
}


