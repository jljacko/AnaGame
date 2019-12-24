#include "pch.h"
#include "TEnvironment.h"
#include <TMap.h>
#include <DirectoryInterface.h>

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
		TrecPointer<TLanguage> language = *languages.GetEntryAt(C).Get()->object.Get();
		if (!language.Get())
			continue;

		std::map<TString, TString> pTypes = language->GetPrimitiveTypes();

		for (auto it = pTypes.begin(); it != pTypes.end(); it++)
		{
			it->first;
		}
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
