#pragma once
#include <Windows.h>
#include "TObject.h"

#include "TFile.h"
#include "DirectoryInterface.h"
class  TShell2 :
	public TObject
{
public:
	TShell2();
	~TShell2();

	void SubmitCommand(TString& command);
	TString GetOutput();

	void TerminateProcess();

	bool CheckProcess();

	TString GetWorkingDirectory();

private:
	void Process_pwd(TString& command);
	void Process_cd(TString& command);



	void ProcessBackgroundProcess(TString& command);
	void ProcessFrontCommand(TString& command);

	TString workingDirectory;
	TString programShell;

	TFile file;
	UINT outputLoc;
	TString output;
	SHELLEXECUTEINFOW info;
};

