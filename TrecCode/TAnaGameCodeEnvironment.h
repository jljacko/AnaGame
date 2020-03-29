#pragma once
#include "TEnvironment.h"
#include <TDirectory.h>




class TAnaGameCodeEnvironment :
	public TEnvironment
{
public:
	TAnaGameCodeEnvironment(TrecPointer<TFileShell> shell);
	void PreProcessSingleFile(TrecPointer<TFile> file);

	virtual UINT RunTask(TString& task);
	virtual UINT SetUpEnv();

	virtual void Compile();
	virtual void Compile(TrecPointer<TFile> logFile);

	virtual void Log();

	virtual void Run();


protected:
	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;
	TString endEnvironment;

	TMap<TLanguage> languages;

	TDataArray<Variable> globalVariables;
};

