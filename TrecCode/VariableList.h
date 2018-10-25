#pragma once
#include <TString.h>
#include <TDataArray.h>
#include "VariableContainer.h"



class VariableList : public VariableContainer
{
public:
	VariableList();
	~VariableList();

	int insertVariable(TString name, TrecPointer<TObject>);
	int insertVariable(TString name, long long value);
	int insertVariable(TString name, double value);

	int insertVariable(long long index, TrecPointer<TObject>);
	int insertVariable(long long index, long long value);
	int insertVariable(long long index, double value);

	intVariable* getVariable(TString name, bool lowScope = true, int scope = 0);

	intVariable * getVariable(long long index, bool lowScope, int scope);

	int clearVariable(TString name, bool lowScope = true);

private:
	TDataArray<intVariable> variables;
};

