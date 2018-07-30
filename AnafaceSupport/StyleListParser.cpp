#include "stdafx.h"
#include "StyleListParser.h"


StyleListParser::StyleListParser(TArray<StylesList>* list)
{
}


StyleListParser::~StyleListParser()
{
}

bool StyleListParser::Obj(TString * v)
{
	return false;
}

bool StyleListParser::Attribute(TString * v, TString e)
{
	return false;
}

bool StyleListParser::Attribute(TrecPointer<TString> v, TString & e)
{
	return false;
}

bool StyleListParser::submitType(TString v)
{
	return false;
}

bool StyleListParser::submitEdition(TString v)
{
	return false;
}

bool StyleListParser::goChild()
{
	return false;
}

void StyleListParser::goParent()
{
}
