#pragma once
#include "TrecLib.h"
#include "TString.h"

// Debugging functions meant to log the operation of AnaGame

typedef enum class LogType {
	lt_memory,
	lt_code,
	lt_pointer,
	lt_bnf
}LogType;

void _TREC_LIB_DLL Log(LogType lt, TString& logMessage);

void CloseLog();