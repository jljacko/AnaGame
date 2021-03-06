
#include "Logger.h"

#include "TFile.h"
#include "DirectoryInterface.h"

TFile memLogger;
TFile lanLogger;
TFile pntLogger;
TFile bnfLogger;

bool openLog(LogType lt) {
	if(lt != LogType::lt_bnf) return false;
	TString fileName(GetDirectoryWithSlash(CentralDirectories::cd_AppData));
	fileName.Append(L"Logs\\");
	ForgeDirectory(fileName);
	
	TFile* logFile = nullptr;
	switch (lt) {
	case LogType::lt_bnf:
		logFile = &bnfLogger;
		if (logFile->IsOpen()) return true;
		fileName.Append(L"Bnf_");
		break;
	case LogType::lt_code:
		logFile = &lanLogger;
		if (logFile->IsOpen()) return true;
		fileName.Append(L"Lan_");
		break;
	case LogType::lt_memory:
		logFile = &memLogger;
		if (logFile->IsOpen()) return true;
		fileName.Append(L"Mem_");
		break;
	case LogType::lt_pointer:
		logFile = &pntLogger;
		if (logFile->IsOpen()) return true;
		fileName.Append(L"Pnt_");
	}
	if (!logFile)
		return false;

	SYSTEMTIME time;
	ZeroMemory(&time, sizeof(time));
	GetSystemTime(&time);

	TString fileName2;
	fileName2.Format(TString(L"%u-%u-%u_%u_%u_%u.log"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	fileName.Append(fileName2);

	return logFile->Open(fileName, OF_WRITE | OF_CREATE);
}


void _TREC_LIB_DLL Log(LogType lt, TString& logMessage)
{
	if (!openLog(lt)) return;

	SYSTEMTIME time;
	ZeroMemory(&time, sizeof(time));
	GetSystemTime(&time);

	TString fileName2;
	fileName2.Format(TString(L"%u-%u-%u_%u:%u:%u:%u_%u  "), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, GetCurrentProcessId());

	switch (lt)
	{
	case LogType::lt_code:
		lanLogger.WriteString(fileName2 + logMessage + TString(L"\n"));
		break;
	case LogType::lt_memory:
		memLogger.WriteString(fileName2 + logMessage + TString(L"\n"));
		break;
	case LogType::lt_pointer:
		pntLogger.WriteString(fileName2 + logMessage + TString(L"\n"));
		break;
	case LogType::lt_bnf:
		bnfLogger.WriteString(logMessage + TString("\n"));
	}
}

void CloseLog()
{
	if (memLogger.IsOpen()) memLogger.Close();
	if (lanLogger.IsOpen()) lanLogger.Close();
	if (pntLogger.IsOpen()) pntLogger.Close();
	if (bnfLogger.IsOpen()) bnfLogger.Close();
}
