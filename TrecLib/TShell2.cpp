#include "TShell2.h"
static UINT shellNumStat = 0;

/*
* Method: (TShell) (Constructor)
* Purpose: Creates the Shell, with the Home directory as the initial working directory
* Parameters: void
* Returns: void
*/
TShell2::TShell2()
{
	workingDirectory.Set(GetDirectoryWithSlash(cd_User));

	TString dir(workingDirectory + "AnagameShell\\");
	ForgeDirectory(dir);

	
	TString fileName(dir);
	fileName.AppendFormat(TString(L"stdout-%d.txt"), shellNumStat++);

	file.Open(fileName, TFile::t_file_share_read | TFile::t_file_read | TFile::t_file_share_write |
		TFile::t_file_write | TFile::t_file_create_always);

	outputLoc = 0LL;
	ZeroMemory(&info, sizeof(info));

	WCHAR shellBuffer[150];
	ZeroMemory(shellBuffer, sizeof(WCHAR) * 150);

	auto ret = GetEnvironmentVariableW(L"COMSPEC", shellBuffer, 150);
	auto er = GetLastError();

	programShell.Set(shellBuffer);
	
}

/*
* Method: (TShell) (Destructor)
* Purpose: Standard Destructor
* Parameters: void
* Returns: void
*/
TShell2::~TShell2()
{
}

/*
* Method: TShell - SubmitCommand
* Purpose: Allows applications to submit commands for processing
* Parameters: TString& command - the command to run
* Returns:
*/
void TShell2::SubmitCommand(TString& command)
{
	// First check to see if a Process is already running from a previous command.
// If it is, simply send the input to that process as it might be scanning for input
	if (CheckProcess())
	{
		std::string aString = command.GetRegString();
		DWORD written = 0;
		// WriteFile(stdInWt, aString.c_str(), aString.size(), &written, nullptr);
		return;
	}


	command.Trim();
	if (!command.GetSize())
		return;
	TString com(command.SubString(0, command.Find(L'\s')));

	if (!com.CompareNoCase(L"pwd"))
	{

	}
	else if (!com.CompareNoCase(L"cd"))
	{

	}
	else
	{
		if (command[command.GetSize() - 1] == L'&')
		{
			// Because the user has decided to run this process in the background, call the function that will start it
			// without causing the TShell to keep track of it.
			ProcessBackgroundProcess(command);
		}
		else
		{
			// The User has failed to specify the background signal, meaning that the TShell should start the command in such a way as to 
			// monitor it's standard output and error and be prepared to send it input
			ProcessFrontCommand(command);
		}
	}
}

/*
* Method: TShell - Get Output
* Purpose: Retrieves the output for Interface UIs that manage a shell session
* Parameters: void
* Returns: TString - the output produced
*
* Note: If output is returned, don't squander it as the object will delete it's own record.
*/
TString TShell2::GetOutput()
{
	if (info.hProcess && file.IsOpen())
	{

		file.ReadString(output, 100U);
	}
	TString ret(output);
	output.Empty();
	return ret;
}

/*
* Method: TShell - TerminateProcess
* Purpose: Terminates any running process under the objects control
* Parameters: void
* Returns: void
*/
void TShell2::TerminateProcess()
{
	if (info.hProcess)
	{
		::TerminateProcess(info.hProcess, 0);
		info.hProcess = 0;
	}
}

/*
* Method: TShell - CheckProcess
* Purpose: Reports whether there is an active process running under the shell
* Parameters: void
* Returns: bool - whether there is a process running under this process
*/
bool TShell2::CheckProcess()
{
	if (info.hProcess)
	{
		DWORD statusCode = 0;
		BOOL runStatus = GetExitCodeProcess(info.hProcess, &statusCode);

		if (runStatus == STILL_ACTIVE)
			return true;
		else if (runStatus)
			output.Format(TString(L"Process terminated with Exit Code: %i\n"), statusCode);
		ZeroMemory(&info, sizeof(info));
	}
	return false;
}

/*
* Method: TShell - GetWorkingDirectory
* Purpose: Reports the working directory of the Shell
* Parameters: void
* Returns: TString - the working directory of the Shell
*/
TString TShell2::GetWorkingDirectory()
{
	return workingDirectory;
}


/*
* Method: TShell - Process_pwd
* Purpose: processes the pwd command
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell2::Process_pwd(TString& command)
{
	output.Set(workingDirectory);
}

/*
* Method: TShell - Process_cd
* Purpose: processes the cd command
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell2::Process_cd(TString& command)
{
	// If cd is alone, set to Home Directory
	if (!command.CompareNoCase(TString(L"cd")))
		workingDirectory.Set(GetDirectory(cd_User));
	else
	{
		// To:Do: Implement change Directory code
	}
}

/*
* Method: TShell - ProcessBackgroundProcess
* Purpose: Starts a process in the background
* Parameters: TString& command - parameters for the command
* Returns: void
*
* Note: this should be called if the user has an ampersend in the command as it is a sign that the process should be detatched from the prompt
*/
void TShell2::ProcessBackgroundProcess(TString& command)
{
	info.cbSize = sizeof(info);
	info.fMask = SEE_MASK_FLAG_NO_UI;
	info.lpVerb = L"open";
	info.lpFile = programShell.GetConstantBuffer();
	info.lpParameters = command.GetConstantBuffer();
	info.lpDirectory = workingDirectory.GetConstantBuffer();
	info.nShow = SW_HIDE;

	ShellExecuteExW(&info);

}

/*
* Method: TShell - ProcessFrontCommand
* Purpose: Starts a process in the forefront
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell2::ProcessFrontCommand(TString& command)
{

	TString upDatedCommand(TString(L"(") + command + TString(L") "));

	if (file.IsOpen())
	{
		auto wcFilePath = file.GetFilePath();
		auto rawWcFilePath = wcFilePath.GetConstantBuffer();
		upDatedCommand.AppendFormat(TString(L"> %ls 2> %ls"), rawWcFilePath, rawWcFilePath);
	}
	info.cbSize = sizeof(info);
	info.fMask = SEE_MASK_NOCLOSEPROCESS |
		SEE_MASK_FLAG_NO_UI;
	info.lpVerb = L"open";
	info.lpFile = programShell.GetConstantBuffer();
	info.lpParameters = upDatedCommand.GetConstantBuffer();
	info.lpDirectory = workingDirectory.GetConstantBuffer();
	info.nShow = SW_HIDE;

	ShellExecuteExW(&info);

	auto er = GetLastError();
}
