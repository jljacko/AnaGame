#pragma once
#include "TObject.h"
#include "TString.h"
#include "TFile.h"

/*
 * class TShell
 * Purpose: provides support for command-line functionality in Anagame
 */
class _TREC_LIB_DLL TShell :
	public TObject
{
public:
	TShell();
	~TShell();

	void SubmitCommand(TString& command);
	TString GetOutput();
	TString GetError();

	void TerminateProcess();

	bool CheckProcess();

	TString GetWorkingDirectory();

private:
	void Process_pwd(TString& command);
	void Process_cd(TString& command);



	void ProcessBackgroundProcess(TString& command);
	void ProcessFrontCommand(TString& command);

	TString workingDirectory;

	TString output, standardError;

	HANDLE stdOutRd, stdInRd, stdErrRd;
	HANDLE stdOutWt, stdInWt, stdErrWt;
	PROCESS_INFORMATION processInfo;
};

