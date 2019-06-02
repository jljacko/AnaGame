#include "stdafx.h"
#include "IntLanguage.h"
#include <DirectoryInterface.h>
#include <TFile.h>
#include <TMap.h>

static TString languageFolder = GetDirectoryWithSlash(cd_Executable) + TString(L"Languages");

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


	TString folderTarget = languageFolder + TString(L"\\Int_") + langName + L"\\";

	TString langDef = folderTarget + TString(L"language.properties");
	TFile langFile;
	
	CFileException ex;
	if (!langFile.Open(langDef, CFile::modeRead, &ex))
	{
		TCHAR buf[200];
		ex.GetErrorMessage(buf, 200);

		return nullptr;
	}



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

	TrecPointer<TString> val = maps.retrieveEntry(TString(L"String Tokens"));

	IntLanguage* lang = new IntLanguage();

	if (val.get())
	{
		val->Trim();
		TrecPointer<TArray<TString>> str = val->split(L";");
		for(UINT c = 0; c < str->Count(); c++)
			lang->string.push_back(str->ElementAt(c).get());
	}

	val = maps.retrieveEntry(TString(L"Multi String Tokens"));



	if (val.get())
	{
		val->Trim();
		TrecPointer<TArray<TString>> str = val->split(L";");
		for (UINT c = 0; c < str->Count(); c++)
			lang->multiLineString.push_back(str->ElementAt(c).get());
	}

	val = maps.retrieveEntry(TString(L"Single Line Comment Start"));
	
	if (val.get())
	{
		val->Trim();
		lang->singleComment = val.get();
	}

	val = maps.retrieveEntry(TString(L"Multi-line Comment Start"));
	if (val.get())
	{
		val->Trim();
		lang->startComment = val.get();
	}

	val = maps.retrieveEntry(TString(L"Multi-line Comment End"));
	if (val.get());
	{
		val->Trim();
		lang->endComment = val.get();
	}
	val = maps.retrieveEntry(TString(L"Statement End"));
	if (val.get())
	{
		val->Trim();
		lang->statementEnd = val.get();
	}

	val = maps.retrieveEntry(TString(L"Block Boundaries"));
	if (val.get())
	{
		val->Trim();
		lang->blockMarks = val.get();
	}

	// To-Do: Add some checking for conflicts

	// Now collect the root tag
	val = maps.retrieveEntry(TString(L"Root BNF"));
	if (val.get())
	{
		val->Trim();
		lang->rootBNF = val.get();
	}

	// To-Do: No conflicts have been detected, now to set up Code syntax parsing
	val = maps.retrieveEntry(TString(L"Syntax File"));
	if (val.get())
	{
		val->Trim();
		TString syntaxFile = folderTarget + val.get();
		TFile bnf;
		
		CFileException ex;
		if (!bnf.Open(syntaxFile, CFile::modeRead, &ex))
		{
			WCHAR buff[200];
			ex.GetErrorMessage(buff, 200);
			int e = 0;
		}

		if (bnf.IsOpen())
		{
			lang->tagList = setUpTagList(bnf);
			if (lang->tagList)
				CompileIntLanguage(*lang->tagList);

			for (UINT Rust = 0; Rust < lang->tagList->Size(); Rust++)
			{
				if (lang->tagList->at(Rust)->GetTageName() == lang->rootBNF)
				{
					lang->startIndex = Rust;
					break;
				}
			}
		}
	}




	// Now time to add the language to the collection
	languages.push_back({ langName, lang });
	return lang;
}

UINT IntLanguage::ProcessCode(TString & statement, TrecPointer<TFile> file, UINT codeStart, VariableTree* gv, TInterpretor* inter, UINT line)
{
	if(startIndex == -1 || !tagList || !tagList->Size() || !gv || !inter)
		return 1;

	if (!rootBNF.GetLength())
		return 2;

	// tagList->at(startIndex)->ProcessTag(statement, *gv, *inter, *tagList);
	tagList->at(startIndex)->ProcessTag(statement, codeStart, file, *gv, *inter, *tagList);
	return 0;
}

IntLanguage::IntLanguage()
{
	tagList = nullptr;
	startIndex = -1;
}


IntLanguage::~IntLanguage()
{
}
