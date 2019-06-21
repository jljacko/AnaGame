#pragma once
#include "TrecLib.h"
#include "TString.h"

// Debugging functions meant to log the operation of AnaGame

typedef enum LogType {
	lt_memory,
	lt_code,
	lt_pointer
}LogType;

void _TREC_LIB_DLL Log(LogType lt, TString& logMessage);

void CloseLog();