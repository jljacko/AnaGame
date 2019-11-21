#pragma once
#include "TObject.h"
#include "TrecReference.h"
#include "TString.h"

class TTypeMetadata;	// Used to hold metadata on various resources. Use for languages like Typescript decorators and Java annotations
class TAttribute;		// Used to hold detils about a class or structs attributes
class TProcedure;		// Used to hold procedures, such as functions

class TType :
	public TObject
{
public:
	TType(TString& name);

	virtual int GetByteSize() = 0;
	virtual TrecPointer<TTypeMetadata> GetMetadata() = 0;
	virtual TrecPointer<TAttribute> GetAttributeByName(TString&) = 0;
	virtual TrecPointer<TProcedure> GetProcedureByName(TString&) = 0;
	virtual TString GetName();

protected:
	TString name;
};

