#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>
#include "TLanguage.h"
#include <TType.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TPromptControl.h>

typedef enum class CompileErrorHandling
{
	ceh_stop,
	ceh_contain_to_block,
	ceh_document_errors
}CompileErrorHandling;

typedef enum class TargetAnagameMachine
{
	tam_object_register,
	tam_object_stack,
	tam_binary_register,
	tam_binary_stack
}TargetAnagameMachine;

typedef struct Variable
{
	TString name;
	TrecPointer<TType> type;
	UINT location;
}Variable;


void GetAnagameProvidedEnvironmentList(TrecPointer<TFileShell> directory, TDataArray<TString>& environmentType);

/* struct LangNames
 * Purpose: holds the name of a Programming language and the file extensions associated with them
 */
class _TREC_CODE_DLL LangNames : public TObject
{
public:
	TString language;
	TDataArray<TString> fileExtensions;
};


class _TREC_CODE_DLL TEnvironment :
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

protected:

	void SetUpLanguageExtensionMapping();

	TrecPointer<TFileShell> rootDirectory;

	TDataArray<TString> taskList;

	TrecSubPointer<TControl, TPromptControl> shellRunner;
};

