#pragma once

#include <TString.h>
#include "TrecCode.h"

#define AG_I_UNDEFINED	0
#define AG_I_OBJECT		1
#define AG_I_S_INT		2
#define AG_I_DOUBLE		3
#define AG_I_BOOLEAN	4
#define AG_I_FUNCTION	5
#define AG_I_ARRAY		6
#define AG_I_U_INT		7
#define AG_I_STRING		8

typedef struct varVal
{
	TrecPointer<TObject> object;
	long long primInt;
	double primFloat;
	varVal();
	~varVal();
}varVal;

typedef struct elementRef
{
	TString varName;
	long long index;
	elementRef();
	elementRef(elementRef& er);
	~elementRef();
}elementRef;

class _TREC_CODE_DLL intVariable: public TObject
{
public:
	UCHAR hold;
	bool varName;			// Does the Variable have a name? 
	elementRef reference;
	varVal value;
	intVariable();
	intVariable(intVariable& iv);
	~intVariable();
	intVariable operator=(intVariable& iv);
};

class VariableList;
class TInterpretor;

class _TREC_CODE_DLL VariableContainer : public TObject
{
public:
	VariableContainer();
	~VariableContainer();

	virtual int insertVariable(TString name, TrecPointer<TObject>, UCHAR type = 1) = 0;				// 1. Basic object
	virtual int insertVariable(TString name, TString& value) = 0;					// 8. Basic String Object
	virtual int insertVariable(TString name, TrecPointer<TInterpretor> value) = 0;	// 5. Used for functions
	virtual int insertVariable(TString name, long long value) = 0;					// 2. signed integer
	virtual int insertVariable(TString name, double value) = 0;						// 3. floating point
	virtual int insertVariable(TString name, bool value) = 0;						// 4. Boolean values
	virtual int insertVariable(TString name, unsigned long long value) = 0;			// 7. unsigned integer
	virtual int insertVariable(TString name, TrecPointer<VariableList> value) = 0;	// 6. Object used for arrays

	
	virtual intVariable* getVariable(TString name, bool lowScope = true, int scope = 0) = 0;

	virtual int clearVariable(TString name, bool lowScope = true) = 0;
};

