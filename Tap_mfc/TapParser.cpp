#include "stdafx.h"
#include "TapParser.h"


TapParser::TapParser(TArray<use>* list)
{
	fileList = list;
}


TapParser::~TapParser()
{
}

bool TapParser::Obj(TString v)
{
	if (!v.Compare(L"Anaface"))
	{
		currentType = Anaface;
		return true;
	}
	if (!v.Compare(L"Code"))
	{
		currentType = code;
		return true;
	}
	if (!v.Compare(L"Image"))
	{
		currentType = image;
		return true;
	}
	if (!v.Compare(L"Video"))
	{
		currentType = video;
		return true;
	}
	if (!v.Compare(L"Graphics"))
	{
		currentType = graphics;
		return true;
	}
	if (!v.Compare(L"Shaders"))
	{
		currentType = shadeCode;
		return true;
	}
	return false;

}

bool TapParser::Attribute(TrecPointer<TString> v,TString& e)
{
	if (!fileList)
		return false;
	if(v->Compare(L"file"))
		return false;
	TrecPointer<use> usage = new use();
	usage->type = currentType;
	usage->fileDirectory.SetString(e);
	fileList->Add(usage);
	return true;
}
