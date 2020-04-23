#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>
#include "Tap_dll.h"
#include <TType.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TPromptControl.h>




void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType);

/* struct LangNames
 * Purpose: holds the name of a Programming language and the file extensions associated with them
 */
class _TAP_DLL LangNames : public TObject
{
public:
	TString language;
	TDataArray<TString> fileExtensions;
};


class _TAP_DLL TEnvironment :
	public TObject
{
public:
	TEnvironment(TrecPointer<TFileShell> shell);
	~TEnvironment();

	TEnvironment(TrecSubPointer<TControl, TPromptControl> prompt);
	virtual TDataArray<TString> GetTaskList();
	//virtual static bool IsViable(TrecPointer<TFileShell> shell) = 0;
	virtual UINT SetUpEnv() = 0;

	virtual void Compile() = 0;
	virtual void Compile(TrecPointer<TFile> logFile) = 0;

	virtual void Log() = 0;

	virtual void Run() = 0;

	

	virtual UINT RunTask(TString& task) = 0;

	TrecPointer<TFileShell> GetRootDirectory();

protected:

	void SetUpLanguageExtensionMapping();

	TrecPointer<TFileShell> rootDirectory;

	TDataArray<TString> taskList;

	TrecSubPointer<TControl, TPromptControl> shellRunner;
};

