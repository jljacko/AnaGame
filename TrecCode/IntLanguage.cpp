#include "stdafx.h"
#include "IntLanguage.h"
#include <DirectoryInterface.h>
#include <TFile.h>
#include <TMap.h>

static TString languageFolder = GetDirectoryWithSlash(cd_Executable) + TString(L"\\Languages");

typedef struct LanguageEntry
{
	TString langName;
	IntLanguage* lang;
}LanguageEntry;

TDataArray<LanguageEntry> languages;

IntLanguage * IntLanguage::getLanguage(TString & langName)
{
	for (int Rust = 0; Rust < languages.Size(); Rust++)
	{
		if (languages[Rust].langName == langName)
			return languages[Rust].lang;
	}


	TString folderTarget = languageFolder + TString(L"\\Int_") + langName;

	TString langDef = folderTarget + TString(L"language.properties");
	TFile langFile(langDef, CFile::modeRead);

	if (!langFile.IsOpen())
		return nullptr;

	TDataArray<TString> lines;
	TString lineString;
	while (langFile.ReadString(lineString))
	{
		lines.push_back(lineString);
	}

	TMap<TString> maps;
	for (UINT Rust = 0; Rust < lines.Size(); Rust++)
	{
		TrecPointer<TArray<TString>> sections = lines[Rust].split(L":");
		if (!sections.get() || sections->Count() < 2)
			continue;

		TString value = sections->ElementAt(1).get();
		for (int c = 2; c < sections->Count(); c++)
		{
			value += L":";
			value += *(sections->ElementAt(c).get());
		}

		if (!value.GetLength())
			continue;

		if (value.GetAt(0) == L'\"')
			value = value.SubString(1);

		if (!value.GetLength())
			continue;

		if (value.GetAt(value.GetLength() - 1) == L'\"')
			value = value.SubString(0, value.GetLength() - 1);


		maps.addEntry(*sections->ElementAt(0).get(), TrecPointer<TString>(new TString(value)));
	}

	TrecPointer<TString> val = maps.retrieveEntry(TString(L"Single String Tokens"));

	IntLanguage* lang = new IntLanguage();

	if (val.get())
	{
		TrecPointer<TArray<TString>> str = val->split(L";");
		for(UINT c = 0; c < str->Count(); c++)
			lang->string.push_back(val.get());
	}

	val = maps.retrieveEntry(TString(L"Multi String Tokens"));

	IntLanguage* lang = new IntLanguage();

	if (val.get())
	{
		TrecPointer<TArray<TString>> str = val->split(L";");
		for (UINT c = 0; c < str->Count(); c++)
			lang->multiLineString.push_back(val.get());
	}

	val = maps.retrieveEntry(TString(L"Single Line Comment Start"));
	
	if (val.get())
		lang->singleComment = val.get();

	val = maps.retrieveEntry(TString(L"Multi-line Comment Start"));
	if (val.get())
		lang->startComment = val.get();


	val = maps.retrieveEntry(TString(L"Multi-line Comment End"));
	if (val.get());
		lang->endComment = val.get();

	val = maps.retrieveEntry(TString(L"Statement End"));
	if (val.get())
		lang->statementEnd = val.get();


	// To-Do: Add some checking for conflicts


	// To-Do: No conflicts have been detected, now to set up Code syntax parsing
	val = maps.retrieveEntry(TString(L"SyntaxFile"));
	if (val.get())
	{
		TString syntaxFile = folderTarget + val.get();
		TFile bnf(syntaxFile, CFile::modeRead);

		if (bnf.IsOpen())
		{
			lang->tagList = setUpTagList(bnf);
			if (lang->tagList)
				CompileIntLanguage(*lang->tagList);
		}
	}


	// Now time to add the language to the collection
	languages.push_back({ langName, lang });
	return lang;
}

IntLanguage::IntLanguage()
{
	tagList = nullptr;
}


IntLanguage::~IntLanguage()
{
}
