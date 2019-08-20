#pragma once
#include "TString.h"


/*
* Class: Parser
* Base class for parsing data into actuall objects
*/
class _TREC_LIB_DLL Parser_ : public TObject
{
public:
	Parser_();
	virtual ~Parser_();

	// for the initial object type
	virtual bool Obj(TString *v);
	// for the attribute name
	virtual bool Attribute(TString* v, TString e);
	virtual bool Attribute(TrecPointer<TString> v, TString& e);
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v);
	virtual bool submitEdition(TString v);

	virtual bool goChild();
	virtual void goParent();

	virtual UCHAR* GetAnaGameType();
};

