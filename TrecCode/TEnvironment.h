#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>
#include "TLanguage.h"

typedef enum CompileErrorHandling
{
	ceh_stop,
	ceh_contain_to_block,
	ceh_document_errors
}CompileErrorHandling;

typedef enum TargetAnagameMachine
{
	tam_object_register,
	tam_object_stack,
	tam_binary_register,
	tam_binary_stack
}TargetAnagameMachine;


class TEnvironment :
	public TObject
{
public:
	TEnvironment();
	~TEnvironment();

	UINT SetUpEnv(TFile& props);

	void Compile();
	void Compile(TrecPointer<TFile> logFile);

	void Log();

	void Run();

protected:
	TString rootDirectory;		// Root directory of the project
	TString rootSource;			// Root directory of the source files of the project
	TString rootResources;		// Root directory of the resource files of the project

	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;

	TMap<TrecPointer<TLanguage>> languages;
};

