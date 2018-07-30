#pragma once
#include "StylesSupport.h"
#include <Parser_.h>

class StyleListParser : public Parser_
{
public:
	StyleListParser(TArray<StylesList>* list);
	~StyleListParser();

	// for the initial object type
	virtual bool Obj(TString *v) override;
	// for the attribute name
	virtual bool Attribute(TString* v, TString e)override;
	virtual bool Attribute(TrecPointer<TString> v, TString& e)override;
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v)override;
	virtual bool submitEdition(TString v)override;

	virtual bool goChild()override;
	virtual void goParent()override;
private:
	TArray<StylesList>* listPointer;
};

