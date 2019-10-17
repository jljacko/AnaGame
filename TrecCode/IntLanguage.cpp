#include "stdafx.h"
#include "IntLanguage.h"
#include <DirectoryInterface.h>
#include <TFile.h>
#include <TMap.h>
#include <Logger.h>

static TString languageFolder = GetDirectoryWithSlash(cd_Executable) + TString(L"Languages");

typedef struct LanguageEntry
{
	TString langName;
	IntLanguage* lang;
}LanguageEntry;

TDataArray<LanguageEntry> languages;

IntLanguage * IntLanguage::getLanguage(TString & langName)
{
	for (UINT Rust = 0; Rust < languages.Size(); Rust++)
	{
		if (languages[Rust].langName == langName)
			return languages[Rust].lang;
	}


	TString folderTarget = languageFolder + TString(L"\\Int_") + langName + L"\\";

	TString langDef = folderTarget + TString(L"language.properties");
	TFile langFile;
	

	if (!langFile.Open(langDef, TFile::t_file_read))
	{
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
		auto sections = lines[Rust].split(L":");
		if (!sections.Get() || sections->Size() < 2)
			continue;

		TString value = sections->at(1);
		for (UINT c = 2; c < sections->Size(); c++)
		{
			value.Append(L":");
			value.Append(sections->at(c));
		}

		if (!value.GetSize())
			continue;

		if (value.GetAt(0) == L'\"')
			value.Set(value.SubString(1));

		if (!value.GetSize())
			continue;

		if (value.GetAt(value.GetSize() - 1) == L'\"')
			value.Set(value.SubString(0, value.GetSize() - 1));


		maps.addEntry(sections->at(0), TrecPointerKey::GetNewTrecPointer<TString>(value));
	}

	TrecPointer<TString> val = maps.retrieveEntry(TString(L"String Tokens"));

	IntLanguage* lang = new IntLanguage();

	if (val.Get())
	{
		val->Trim();
		auto str = val->split(L";");
		for (UINT c = 0; c < str->Size(); c++)
		{
			parseSpecialCharacters(str->at(c));
			lang->string.push_back(str->at(c));
		}
	}

	val = maps.retrieveEntry(TString(L"Multi String Tokens"));



	if (val.Get())
	{
		val->Trim();
		auto str = val->split(L";");
		for (UINT c = 0; c < str->Size(); c++)
		{
			parseSpecialCharacters(str->at(c));
			lang->multiLineString.push_back(str->at(c));
		}
	}

	val = maps.retrieveEntry(TString(L"Single Line Comment Start"));
	
	if (val.Get())
	{
		val->Trim();
		parseSpecialCharacters(*val.Get());
		lang->singleComment.Set(val.Get());
	}

	val = maps.retrieveEntry(TString(L"Multi-line Comment Start"));
	if (val.Get())
	{
		val->Trim();
		parseSpecialCharacters(*val.Get());
		lang->startComment.Set(val.Get());
	}

	val = maps.retrieveEntry(TString(L"Multi-line Comment End"));
	if (val.Get())
	{
		val->Trim();
		parseSpecialCharacters(*val.Get());
		lang->endComment.Set(val.Get());
	}
	val = maps.retrieveEntry(TString(L"Statement End"));
	if (val.Get())
	{
		val->Trim();
		parseSpecialCharacters(*val.Get());
		lang->statementEnd.Set(val.Get());
	}

	val = maps.retrieveEntry(TString(L"Block Boundaries"));
	if (val.Get())
	{
		val->Trim();
		parseSpecialCharacters(*val.Get());
		lang->blockMarks.Set(val.Get());
	}

	// To-Do: Add some checking for conflicts

	// Now collect the root tag
	val = maps.retrieveEntry(TString(L"Root BNF"));
	if (val.Get())
	{
		val->Trim();
		lang->rootBNF.Set(val.Get());
	}

	// To-Do: No conflicts have been detected, now to set up Code syntax parsing
	val = maps.retrieveEntry(TString(L"Syntax File"));
	if (val.Get())
	{
		val->Trim();
		TString syntaxFile = folderTarget + val.Get();
		TFile bnf;
		
	
		if (!bnf.Open(syntaxFile, TFile::t_file_read))
		{
			int e = 0;
		}

		if (bnf.IsOpen())
		{
			BlockType bt = (lang->blockMarks == TString(L"Tabs")) ? block_indent: block_tokens;


			Log(lt_bnf, TString(L"LANGUAGE: ") + langName);
			Log(lt_bnf, TString());
			lang->tagList = setUpTagList(bnf, bt);
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

UINT IntLanguage::ProcessCode(TString & statement, TrecPointer<TFile> file, UINT codeStart, VariableContainer* gv, TInterpretor* inter, UINT line)
{
	if(startIndex == -1 || !tagList || !tagList->Size() || !gv || !inter)
		return 1;

	if (!rootBNF.GetSize())
		return 2;

	// tagList->at(startIndex)->ProcessTag(statement, *gv, *inter, *tagList);
	TagCheck check = tagList->at(startIndex)->ProcessTag(statement, codeStart, file, *gv, *inter, *this, *tagList);

	if (!check.success)
		return 3;

	return 0;
}

BNFTag* IntLanguage::getTagAt(UINT index)
{
	if(!tagList)
		return nullptr;
	if (index < tagList->Size())
		return tagList->at(index);
	return nullptr;
}

TString IntLanguage::getLanguageName()
{
	return language;
}

IntLanguage::IntLanguage()
{
	tagList = nullptr;
	startIndex = -1;
}


IntLanguage::~IntLanguage()
{
}

void parseSpecialCharacters(TString& string)
{
	string.Replace(L"\\n", L"\n");
	string.Replace(L"\\s", L"\s");
	string.Replace(L"\\t", L"\t");
	string.Replace(L"\\r", L"\r");
	string.Replace(L"\\v", L"\v");
	string.Replace(L"\\f", L"\f");
}
