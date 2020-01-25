#include "TShell.h"
#include "DirectoryInterface.h"

/*
* Method: (TShell) (Constructor) 
* Purpose: Creates the Shell, with the Home directory as the initial working directory
* Parameters: void
* Returns: void
*/
TShell::TShell()
{
	workingDirectory.Set(GetDirectoryWithSlash(cd_Documents));
	stdOutRd = stdInRd = stdErrRd = nullptr;
	stdOutWt = stdInWt = stdErrWt = nullptr;

	RtlZeroMemory(&processInfo, sizeof(processInfo));
}

/*
* Method: (TShell) (Destructor)
* Purpose: Standard Destructor
* Parameters: void
* Returns: void
*/
TShell::~TShell()
{
}

/*
* Method: TShell - SubmitCommand
* Purpose: Allows applications to submit commands for processing
* Parameters: TString& command - the command to run
* Returns:
*/
void TShell::SubmitCommand(TString& command)
{
	// First check to see if a Process is already running from a previous command.
	// If it is, simply send the input to that process as it might be scanning for input
	if (CheckProcess())
	{
		std::string aString = command.GetRegString();
		DWORD written = 0;
		WriteFile(stdInWt, aString.c_str(), aString.size(), &written, nullptr);
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
TString TShell::GetOutput()
{
	if (processInfo.hProcess)
	{
		char* str = new char[101];
		ZeroMemory(str, sizeof(char) * 101);
		DWORD read;
		if (ReadFile(stdOutRd, str, 100, &read, nullptr))
			output.Set(str);
		else
			output.Set(L"Error Obtaining Standard Output");
		delete[] str;
		str = nullptr;
	}
	TString ret(output);
	output.Empty();
	return ret;
}

/*
* Method: TShell - GetError
* Purpose: Retrieves Error information from a running process
* Parameters: void
* Returns: TString - Error Output
*/
TString TShell::GetError()
{
	if (processInfo.hProcess)
	{
		char* str = new char[101];
		ZeroMemory(str, sizeof(char) * 101);
		DWORD read;
		if (ReadFile(stdErrRd, str, 100, &read, nullptr))
			standardError.Set(str);
		else
			standardError.Set(L"Error Obtaining Standard Output");
		delete[] str;
		str = nullptr;
	}
	TString ret(standardError);
	standardError.Empty();
	return ret;
}

/*
* Method: TShell - TerminateProcess
* Purpose: Terminates any running process under the objects control
* Parameters: void
* Returns: void
*/
void TShell::TerminateProcess()
{
	if (processInfo.hProcess)
	{
		::TerminateProcess(processInfo.hProcess, 0);
		ZeroMemory(&processInfo, sizeof(processInfo));
	}
}

/*
* Method: TShell - CheckProcess
* Purpose: Reports whether there is an active process running under the shell
* Parameters: void
* Returns: bool - whether there is a process running under this process
*/
bool TShell::CheckProcess()
{
	if (processInfo.hProcess)
	{
		DWORD statusCode = 0;
		BOOL runStatus = GetExitCodeProcess(processInfo.hProcess, &statusCode);

		if (runStatus == STILL_ACTIVE)
			return true;
		else if (runStatus)
			output.Format(TString(L"Process %i terminated with Exit Code: %i\n"), processInfo.dwProcessId, statusCode);
		ZeroMemory(&processInfo, sizeof(processInfo));
	}
	return false;
}

/*
* Method: TShell - GetWorkingDirectory
* Purpose: Reports the working directory of the Shell
* Parameters: void
* Returns: TString - the working directory of the Shell
*/
TString TShell::GetWorkingDirectory()
{
	return workingDirectory;
}

/*
* Method: TShell - Process_pwd
* Purpose: processes the pwd command
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell::Process_pwd(TString& command)
{
	output.Set(workingDirectory);
}

/*
* Method: TShell - Process_cd
* Purpose: processes the cd command
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell::Process_cd(TString& command)
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
void TShell::ProcessBackgroundProcess(TString& command)
{
	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	security.bInheritHandle = TRUE;


	STARTUPINFOW start;
	ZeroMemory(&start, sizeof(start));

	start.cb = sizeof(start);
	
	WCHAR* commandBuff = command.GetBufferCopy();
	BOOL creat = CreateProcessW(nullptr,
		commandBuff,
		&security,
		nullptr,
		FALSE,
		0,
		nullptr,
		workingDirectory.GetConstantBuffer(),
		&start,
		&processInfo
	);
	delete[] commandBuff;
	commandBuff = nullptr;

	if (creat)
	{
		output.Format(TString(L"Created Process with ID=[%d] and Thread ID=[%d]"), processInfo.dwProcessId, processInfo.dwThreadId);

		// We don't care about the process at this point so erase information about it
		ZeroMemory(&processInfo, sizeof(processInfo));
	}
	else
		output.Set(L"Failed to create background Process");
	
}

/*
* Method: TShell - ProcessFrontCommand
* Purpose: Starts a process in the forefront
* Parameters: TString& command - parameters for the command
* Returns: void
*/
void TShell::ProcessFrontCommand(TString& command)
{
	STARTUPINFOW start;
	ZeroMemory(&start, sizeof(start));

	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	security.bInheritHandle = TRUE;

	if (!CreatePipe(&stdOutRd, &stdOutWt, &security, 0)) 
	{
		output.Set(L"Error Creating Outout Pipe for process");
		return;
	}

	if (!CreatePipe(&stdInRd, &stdInWt, &security, 0))
	{
		CloseHandle(stdOutRd);
		CloseHandle(stdOutWt);
		stdOutRd = stdOutWt = nullptr;
		output.Set(L"Error Creating Input Pipe for process");
		return;
	}

	if (!CreatePipe(&stdErrRd, &stdErrWt, &security, 0))
	{
		CloseHandle(stdOutRd);
		CloseHandle(stdOutWt);
		CloseHandle(stdInRd);
		CloseHandle(stdInWt);
		stdOutRd = stdOutWt = stdInRd = stdInWt = nullptr;
		output.Set(L"Error Creating Error Pipe for process");
		return;
	}

	start.cb = sizeof(start);
	start.dwFlags = STARTF_USESTDHANDLES;//  STARTF_PREVENTPINNING | STARTF_TITLEISAPPID
	start.hStdError = stdErrWt;
	start.hStdInput = stdInRd;
	start.hStdOutput = stdOutWt;

	WCHAR* commandBuff = command.GetBufferCopy();

	BOOL creat = CreateProcessW(nullptr,
		commandBuff,
		&security,
		nullptr,
		FALSE,
		0,
		nullptr,
		workingDirectory.GetConstantBuffer(),
		&start,
		&processInfo
	);
	delete[] commandBuff;
	commandBuff = nullptr;
	if (!creat)
	{
		CloseHandle(stdOutRd);
		CloseHandle(stdOutWt);
		CloseHandle(stdInRd);
		CloseHandle(stdInWt);
		CloseHandle(stdErrRd);
		CloseHandle(stdErrWt);
		stdOutRd = stdOutWt = stdInRd = stdInWt = stdErrRd = stdErrWt = nullptr;
		output.Set(L"Failed to run Command Process");
		return;
	}
}
