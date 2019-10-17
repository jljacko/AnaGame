#pragma once
#include <TMap.h>
#include <Tstring.h>

typedef struct pByteCode {
	TString functionID;
	unsigned char* code[];
}pByteCode;

typedef struct oByteCode {
	TObject* base;
	TArray<pByteCode> methods;
}oByteCode;