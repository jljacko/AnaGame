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

	int insertVariable(TString name, TrecPointer<TObject>);
	int insertVariable(TString name, long long value);
	int insertVariable(TString name, double value);
	intVariable* getVariable(TString name, bool lowScope = true, int scope = 0);

	int clearVariable(TString name, bool lowScope = true);

private:
	varNode root;
	int insertVariable(TString name, TrecPointer<TObject>, varNode& node);
	int insertVariable(TString name, long long value, varNode& node);
	int insertVariable(TString name, double value, varNode& node);

	intVariable* getVariable(TString name, varNode& node, bool lowScope = true, int scope = 0);

};

