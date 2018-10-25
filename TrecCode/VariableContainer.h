#pragma once

#include <TString.h>

typedef union varVal
{
	TrecPointer<TObject> object;
	long long primInt;
	double primFloat;
	varVal();
	~varVal();
}varVal;

typedef union elementRef
{
	TString varName;
	long long index;
	elementRef();
	elementRef(elementRef& er);
	~elementRef();
}elementRef;

typedef struct intVariable
{
	UCHAR hold;
	bool varName;
	elementRef reference;
	varVal value;
	intVariable();
	intVariable(intVariable& iv);
	~intVariable();
}intVariable;

class VariableContainer : public TObject
{
public:
	VariableContainer();
	~VariableContainer();

	virtual int insertVariable(TString name, TrecPointer<TObject>) = 0;
	virtual int insertVariable(TString name, long long value) = 0;
	virtual int insertVariable(TString name, double value) = 0;
	virtual intVariable* getVariable(TString name, bool lowScope = true, int scope = 0) = 0;

	virtual int clearVariable(TString name, bool lowScope = true) = 0;
};

