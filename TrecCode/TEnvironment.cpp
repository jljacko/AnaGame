#include "pch.h"
#include "TEnvironment.h"
#include <TMap.h>
#include <DirectoryInterface.h>

static bool languagesMapped = false;

// Holds list of Languages and their file extensions
static TDataArray<LangNames> languageList;

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

TEnvironment::TEnvironment(TString& rootDirectory, TString& sourceDirectory, TString& resourceDirectory, TString& binDirectory)
{
	this->rootDirectory.Set(rootDirectory);
	this->rootSource.Set(sourceDirectory);
	this->rootResources.Set(resourceDirectory);
	this->rootBin.Set(binDirectory);

	ForgeDirectory(rootDirectory);
	if (rootDirectory.GetSize() && rootDirectory[rootDirectory.GetSize() - 1] != L'\\')
		rootDirectory.AppendChar(L'\\');


	if (rootSource.GetSize() > 5 && (rootSource[0] != L'\\' || rootSource[0] != L'/' || rootSource[1] != L':'))
	{
		rootSource.Set(rootDirectory + rootSource);
	}
	ForgeDirectory(rootSource);

	if (rootResources.GetSize() > 5 && (rootResources[0] != L'\\' || rootResources[0] != L'/' || rootResources[1] != L':'))
	{
		rootResources.Set(rootDirectory + rootResources);
	}
	ForgeDirectory(rootResources);

	if (rootBin.GetSize() > 5 && (rootBin[0] != L'\\' || rootBin[0] != L'/' || rootBin[1] != L':'))
	{
		rootBin.Set(rootDirectory + rootBin);
	}
	ForgeDirectory(rootBin);
}

TEnvironment::~TEnvironment()
{
}

UINT TEnvironment::SetUpEnv(TFile& props)
{
	if (!props.IsOpen())
		return 1;

	TString prop;
	TMap<TString> properties;
	while (props.ReadString(prop))
	{

		int commentStart = prop.FindOutOfQuotes(TString(L"#"));
		if (commentStart != -1)
			prop.Set(prop.SubString(0, commentStart));

		int split = prop.FindOutOfQuotes(TString(L":"));
		if (split = -1)
			continue;

		properties.addEntry(prop.SubString(0, split), TrecPointerKey::GetNewTrecPointer<TString>(prop.SubString(split + 1)));
	}

	TrecPointer<TString> value = properties.retrieveEntry(TString(L"TargetEnvironment"));
	if (value.Get())
		targetExtensions.Set(value.Get());


	// To-Do: Scan Source Directory for source files and set up languages




	// Now set up Primitive types that each detected language offers
	for (UINT C = 0; C < languages.count(); C++)
	{
		TrecPointer<TLanguage> language = languages.GetEntryAt(C).Get()->object;
		if (!language.Get())
			continue;

		TMap<TString> pTypes = language->GetPrimitiveTypes();

	}

	return 0;
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

void TEnvironment::PreProcessSingleFile(TrecPointer<TFile> file)
{
	if (!file.Get() || !file->IsOpen())
		return;

	TString fileExt(file->GetFileExtension());

	TrecPointer<TLanguage> lang = languages.retrieveEntry(fileExt);

	if (!lang.Get())
	{
		SetUpLanguageExtensionMapping();

		TString langName(retrieveLanguageByExtension(fileExt));
		if (!langName.GetSize())
			return;

		auto language = TLanguage::getLanguage(langName);
		if (!language.Get())
			return;

		for (UINT C = 0; C < languageList.Size(); C++)
		{
			if (languageList[C].language.Compare(langName))
				continue;
			for (UINT Rust = 0; Rust < languageList[C].fileExtensions.Size(); Rust++)
			{
				languages.addEntry(languageList[C].fileExtensions[Rust], language);
			}
			break;
		}

		lang = language;
	}

	lang->PreProcessFile(file);
}
