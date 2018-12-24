#pragma once
#include "VariableContainer.h"



typedef struct varNode
{
	intVariable variable;
	TrecPointer<varNode> lower, higher;
	varNode();
	~varNode();
}varNode;

class VariableTree : public VariableContainer
{
public:
	VariableTree();
	~VariableTree();

	virtual int insertVariable(TString name, TrecPointer<TObject>);				// 1. Basic object
	virtual int insertVariable(TString name, TString& value);					// 8. Basic String Object
	virtual int insertVariable(TString name, TrecPointer<TInterpretor> value);	// 5. Used for functions
	virtual int insertVariable(TString name, long long value);					// 2. signed integer
	virtual int insertVariable(TString name, double value);						// 3. floating point
	virtual int insertVariable(TString name, bool value);						// 4. Boolean values
	virtual int insertVariable(TString name, unsigned long long value);			// 7. unsigned integer
	virtual int insertVariable(TString name, TrecPointer<VariableList> value);	// 6. Object used for arrays

	intVariable* getVariable(TString name, bool lowScope = true, int scope = 0);

	int clearVariable(TString name, bool lowScope = true);

private:
	varNode root;
	int insertVariable(TString name, TrecPointer<TObject>, varNode& node);
	int insertVariable(TString name, TrecPointer<TInterpretor>, varNode& node);
	int insertVariable(TString name, TrecPointer<VariableList>, varNode& node);
	int insertVariable(TString name, TString& value, varNode& node);
	int insertVariable(TString name, unsigned long long value, varNode& node);
	int insertVariable(TString name, long long value, varNode& node);
	int insertVariable(TString name, double value, varNode& node);
	int insertVariable(TString name, bool value, varNode& node);

	intVariable* getVariable(TString name, varNode& node, bool lowScope = true, int scope = 0);

};

