#include "stdafx.h"
#include "StylesSupport.h"

TArray<StylesList> styles;

bool SetUpStylesList()
{
	if (styles.Count())
		return true;

	return false;
}

TDataArray<TString>* GetStyleList(TString & style)
{
	return nullptr;
}
