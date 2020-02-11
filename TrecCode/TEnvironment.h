#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>
#include "TLanguage.h"
#include <TType.h>

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

typedef struct Variable
{
	TString name;
	TrecPointer<TType> type;
	UINT location;
}Variable;

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
	TEnvironment(TString& rootDirectory, TString& sourceDirectory, TString& resourceDirectory, TString& binDirectory);
	~TEnvironment();

	UINT SetUpEnv(TFile& props);

	void Compile();
	void Compile(TrecPointer<TFile> logFile);

	void Log();

	void Run();

	void PreProcessSingleFile(TrecPointer<TFile> file);

protected:

	void SetUpLanguageExtensionMapping();

	TString rootDirectory;		// Root directory of the project
	TString rootSource;			// Root directory of the source files of the project
	TString rootResources;		// Root directory of the resource files of the project
	TString rootBin;			// Root directory of the bin files (output files)

	TString targetExtensions;	// Extensions to target for compilation

	CompileErrorHandling compileErrorHandling;
	TargetAnagameMachine targetMachine;
	TString endEnvironment;

	TMap<TLanguage> languages;

	TDataArray<Variable> globalVariables;
};

