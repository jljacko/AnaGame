#pragma once

#include <TString.h>
#include <TFile.h>
#include <TDataArray.h>
#include <TArray.h>

typedef struct StylesList
{
	TString style;
	TDataArray<TString> atlernatves;
};

TDataArray<TString>* GetStyleList(TString& style);