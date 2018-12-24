#pragma once
#include <TString.h>
#include <TDataArray.h>
#include "VariableContainer.h"



class VariableList : public VariableContainer
{
public:
	VariableList();
	~VariableList();

	virtual int insertVariable(TString name, TrecPointer<TObject>);				// 1. Basic object
	virtual int insertVariable(TString name, TString& value);					// 8. Basic String Object
	virtual int insertVariable(TString name, TrecPointer<TInterpretor> value);	// 5. Used for functions
	virtual int insertVariable(TString name, long long value);					// 2. signed integer
	virtual int insertVariable(TString name, double value);						// 3. floating point
	virtual int insertVariable(TString name, bool value);						// 4. Boolean values
	virtual int insertVariable(TString name, unsigned long long value);			// 7. unsigned integer
	virtual int insertVariable(TString name, TrecPointer<VariableList> value);	// 6. Object used for arrays

	int insertVariable(long long index, TrecPointer<TObject>);
	int insertVariable(long long index, long long value);
	int insertVariable(long long index, double value);

	intVariable* getVariable(TString name, bool lowScope = true, int scope = 0);

	intVariable * getVariable(long long index, bool lowScope, int scope);

	int clearVariable(TString name, bool lowScope = true);

private:
	TDataArray<intVariable> variables;
};

