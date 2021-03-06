#include "pch.h"
#include "TEnvironment.h"
#include <TMap.h>
#include <DirectoryInterface.h>

static bool languagesMapped = false;

// Holds list of Languages and their file extensions
static TDataArray<LangNames> languageList;

TrecPointer<TFileShell> TEnvironment::GetRootDirectory()
{
	return rootDirectory;
}

void TEnvironment::SetUpLanguageExtensionMapping()
{
	if (languagesMapped)
		return;

	TString languageFolder = GetDirectoryWithSlash(CentralDirectories::cd_Executable) + TString(L"Languages");

	if (languageList.Size())
		return;

	TFile languageLister;

	languageLister.Open(languageFolder + TString(L"\\languages.properties"), TFile::t_file_read);

	if (!languageLister.IsOpen())
	{
		//char errorBuf[100];
		//ex.GetErrorMessage((LPTSTR)errorBuf, 99);
		return;
	}
	TString line;
	while (languageLister.ReadString(line))
	{
		line.Trim();
		TrecPointer<TDataArray<TString>> sep = line.split(TString(L":"));

		if (sep->Size() != 2)
			continue;
		LangNames ln;
		ln.language = sep->at(0);
		sep = sep->at(1).split(TString(L";"));

		for (UINT rust = 0; rust < sep->Size(); rust++)
		{
			if (!sep->at(rust).GetSize())
				continue;
			sep->at(rust).Trim();
			ln.fileExtensions.push_back(sep->at(rust));
		}
		languageList.push_back(ln);
	}

	languageLister.Close();
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


TEnvironment::TEnvironment(TrecPointer<TFileShell> shell)
{
}

TEnvironment::~TEnvironment()
{
}

TEnvironment::TEnvironment(TrecSubPointer<TControl, TPromptControl> prompt)
{
	shellRunner = prompt;
}

void TEnvironment::Compile()
{
}

void TEnvironment::Compile(TrecPointer<TFile> logFile)
{
}

void TEnvironment::Log()
{
}

void TEnvironment::Run()
{
}

TDataArray<TString> TEnvironment::GetTaskList()
{
	return taskList;
}

void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType)
{
	if (!directory.Get() || !directory->IsDirectory())
		return;

	auto files = dynamic_cast<TDirectory*>(directory.Get())->GetFileListing();

	for (UINT Rust = 0; Rust < files.Size(); Rust++)
	{
		if (!files[Rust].Get())
			continue;

		TString fileName = files[Rust]->GetName();

		if (!fileName.CompareNoCase(TString(L"build.gradle")))
		{
			environmentType.push_back(TString(L"Gradle"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"pom.xml")))
		{
			environmentType.push_back(TString(L"Maven"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"treccode.tml")))
		{
			environmentType.push_back(TString(L"TrecCode"));
			continue;
		}

		if (!fileName.CompareNoCase(TString(L"ag_vid.tml")))
		{
			environmentType.push_back(TString(L"AnagameVideo"));
			continue;
		}

		// file.sln
		// 012345678

		if (fileName.Find(TString(L".snl")) == fileName.GetSize() - 4 || fileName.Find(TString(L".vcxproj")) == fileName.GetSize() - 8)
		{
			environmentType.push_back(TString(L"VisualStudio"));
			continue;
		}
	}
}
