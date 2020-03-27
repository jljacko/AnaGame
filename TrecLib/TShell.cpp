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

	// Set up a buffer to get the location of the Command prompt
	WCHAR shellBuffer[150];
	ZeroMemory(shellBuffer, sizeof(WCHAR) * 150);

	// Location of the Command Prompt is held by "COMSPEC", so use it to get our prompt program
	auto ret = GetEnvironmentVariableW(L"COMSPEC", shellBuffer, 150);
	auto er = GetLastError();

	programShell.Set(shellBuffer);
	outputLoc = 0;
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
	// If a process is running capture the output
	if (processInfo.hProcess)
	{
		char* str = new char[101];
		ZeroMemory(str, sizeof(char) * 101);
		DWORD read;
		// Use our read-output handle to read the output from our running process
		if (ReadFile(stdOutRd, str, 100, &read, nullptr))
			output.Set(str);
		else
		{
			auto err = GetLastError();
			output.Set(L"Error Obtaining Standard Output");
		}
		delete[] str;
		str = nullptr;
	}
	// capture the output into a return string and empty the output string
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
	// If Process is running capture the error reported by the process
	if (processInfo.hProcess)
	{
		char* str = new char[101];
		ZeroMemory(str, sizeof(char) * 101);
		DWORD read;

		OVERLAPPED overlap;
		ZeroMemory(&overlap, sizeof(overlap));



		// Use our read-error handle to read the output from our running process
		if (ReadFile(stdErrRd, str, 100, &read, nullptr))
			standardError.Set(str);
		else
			standardError.Set(L"Error Obtaining Standard Output");
		delete[] str;
		str = nullptr;
	}
	// Capture the error string into a return string and empty the error string for further data
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
	// To-Do: Find a less crude way of terminating the process
	if (processInfo.hProcess)
	{
		::TerminateProcess(processInfo.hProcess, 0);
		ZeroMemory(&processInfo, sizeof(processInfo));
	}
}

/*
* Method: TShell - CheckProcess
* Purpose: Reports whether there is an active process running under the shell, if a process has finished,
*	this method also cleans up the resources associated with the process and prepares to host a new one
* Parameters: void
* Returns: bool - whether there is a process running under this process
*/
bool TShell::CheckProcess()
{
	// If a process is active, check on it
	if (processInfo.hProcess)
	{
		DWORD statusCode = 0;
		// Find out if the process is still running
		BOOL runStatus = GetExitCodeProcess(processInfo.hProcess, &statusCode);

		if (runStatus == STILL_ACTIVE) // If the process is still active, return true
			return true;
		else if (runStatus) // Report the termination of the process
			output.Format(TString(L"Process %i terminated with Exit Code: %i"), processInfo.dwProcessId, statusCode);
		
		// Close the handles we no longer need
		CloseHandle(processInfo.hProcess);
		CloseHandle(stdOutRd);
		CloseHandle(stdInWt);
		CloseHandle(stdErrRd);
		ZeroMemory(&processInfo, sizeof(processInfo));
	}
	// If we make it here, we can assume that the TShell no longer has an active process running, so report false
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
	// Set up default security settings
	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	security.bInheritHandle = TRUE;

	// Set up a start struct with default settings
	STARTUPINFOW start;
	ZeroMemory(&start, sizeof(start));

	start.cb = sizeof(start);

	// St up the real command so that any spaces in "programShell" don't get mistaken for a split in arguments
	TString realCommand(TString(L"\"") + programShell + TString(L"\" ") + command);
	
	// We need to use the Buffer Copy as Windows documents that the string could be editied by the function
	WCHAR* commandBuff = realCommand.GetBufferCopy();
	BOOL creat = CreateProcessW(nullptr, 
		commandBuff,	// Command to call
		&security,		// use our security object for the process
		nullptr,		// nothing for the thread
		FALSE,			// no need to inherite handles since this process is background and we don't ned to track it
		0,				// No special flags
		nullptr,		// No special environment settings
		workingDirectory.GetConstantBuffer(), // the Working Directory to use
		&start,			// the start info object we created
		&processInfo	// Get information about our process
	);
	delete[] commandBuff;
	commandBuff = nullptr;

	// Since this is a background process, simply report success or fail back to the shell control
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
	// Set default startup info
	STARTUPINFOW start;
	ZeroMemory(&start, sizeof(start));

	// Set basic Security
	SECURITY_ATTRIBUTES security;
	ZeroMemory(&security, sizeof(security));
	security.nLength = sizeof(security);
	// Have the process inherit our handles
	security.bInheritHandle = TRUE;

	// Attempt to create a pipe so we can capture standard output
	if (!CreatePipe(&stdOutRd, &stdOutWt, &security, 0)) 
	{
		output.Set(L"Error Creating Outout Pipe for process");
		return;
	}

	// Attempt to create standard input pipe
	if (!CreatePipe(&stdInRd, &stdInWt, &security, 0))
	{
		// If we failed, clean up the outout pipe that did succeed and report issue to user
		CloseHandle(stdOutRd);
		CloseHandle(stdOutWt);
		stdOutRd = stdOutWt = nullptr;
		output.Set(L"Error Creating Input Pipe for process");
		return;
	}

	// Attempt to create a standard error pipe
	if (!CreatePipe(&stdErrRd, &stdErrWt, &security, 0))
	{
		// Failed? Clean up the input and output pipe that worked
		CloseHandle(stdOutRd);
		CloseHandle(stdOutWt);
		CloseHandle(stdInRd);
		CloseHandle(stdInWt);
		stdOutRd = stdOutWt = stdInRd = stdInWt = nullptr;
		output.Set(L"Error Creating Error Pipe for process");
		return;
	}

	// Save handle information so we can set up the handles for the process.
	// Failure to do so caused ReadFile calls to fail
	HANDLE saveOut = GetStdHandle(STD_OUTPUT_HANDLE),
		saveIn = GetStdHandle(STD_INPUT_HANDLE),
		saveEr = GetStdHandle(STD_ERROR_HANDLE);


	SetStdHandle(STD_OUTPUT_HANDLE, stdOutWt);
	SetStdHandle(STD_INPUT_HANDLE, stdInRd);
	SetStdHandle(STD_ERROR_HANDLE, stdErrWt);

	SetHandleInformation(stdOutRd, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(stdInWt, HANDLE_FLAG_INHERIT, 0);

	start.cb = sizeof(start);
	start.dwFlags = STARTF_USESTDHANDLES;// Make sure our pipe handles are used
	start.hStdError = stdErrWt;
	start.hStdInput = stdInRd;
	start.hStdOutput = stdOutWt;

	// Set up the real command to use, using quotes to guard against spaces in "programShell"
	TString realCommand(TString(L"\"") + programShell + TString(L"\" /c ") + command);

	// Get buffer copies as the Documentation suggests that CreateProcessW could write to these buffers
	WCHAR* commandBuff = realCommand.GetBufferCopy();
	WCHAR* appBuff = programShell.GetBufferCopy();

	BOOL creat = CreateProcessW(appBuff,		// Program name (command Prompt by Default)
		commandBuff,							// Command to run
		&security,								// Security for the Process
		&security,								// Security for the thread
		TRUE,									// Make sure process get our handles so we can communicate with it
		CREATE_NO_WINDOW,						// We're presenting the output in our window so don't create a new one
		nullptr,								// no special environment
		workingDirectory.GetConstantBuffer(),	// Provide the working directory to use
		&start,									// Default  start info
		&processInfo							// Information about the process being created (out param)
	);
	// Delete the string buffers we no longer need them
	delete[] commandBuff;
	delete[] appBuff;
	commandBuff = appBuff = nullptr;

	// Restore the old handles
	SetStdHandle(STD_OUTPUT_HANDLE, saveOut);
	SetStdHandle(STD_INPUT_HANDLE, saveIn);
	SetStdHandle(STD_ERROR_HANDLE, saveEr);

	if (!creat)
	{
		// Here, we hand failed to create a new process, so clean up the pipes we have created

		// Added for debugging
		auto er = GetLastError();
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

	// close the handle pipes we don't need, Failure to do so will cause ReadFile to hang Anagame
	CloseHandle(stdOutWt);
	CloseHandle(stdErrWt);
	CloseHandle(stdInRd);
	CloseHandle(processInfo.hThread);
	outputLoc = 0;
}
