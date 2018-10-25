#include "stdafx.h"
#include "VariableList.h"


VariableList::VariableList()
{
}


VariableList::~VariableList()
{
}

int VariableList::insertVariable(TString name, TrecPointer<TObject> obj)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = 1;
			variables[c].value.object = obj;
			return 1;
		}
	}
	intVariable var;
	var.hold = 1;
	var.reference.varName = name;
	var.varName = true;
	var.value.object = obj;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, long long value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = 2;
			variables[c].value.primInt = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = 2;
	var.reference.varName = name;
	var.varName = true;
	var.value.primInt = value;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, double value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = 3;
			variables[c].value.primFloat = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = 3;
	var.reference.varName = name;
	var.varName = true;
	var.value.primFloat = value;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(long long index, TrecPointer<TObject> obj)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (!variables[c].varName && variables[c].reference.index == index)
		{
			variables[c].hold = 1;
			variables[c].value.object = obj;
			return 1;
		}
	}
	intVariable var;
	var.hold = 1;
	var.reference.index = index;
	var.varName = false;
	var.value.object = obj;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(long long index, long long value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (!variables[c].varName && variables[c].reference.index == index)
		{
			variables[c].hold = 2;
			variables[c].value.primInt = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = 2;
	var.reference.index = index;
	var.varName = false;
	var.value.primInt = value;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(long long index, double value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (!variables[c].varName && variables[c].reference.index == index)
		{
			variables[c].hold = 3;
			variables[c].value.primFloat = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = 3;
	var.reference.index = index;
	var.varName = false;
	var.value.primFloat = value;
	variables.push_back(var);
	return 0;
}

intVariable * VariableList::getVariable(TString name, bool lowScope, int scope)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
			return &variables[c];
	}
	return nullptr;
}

intVariable * VariableList::getVariable(long long index, bool lowScope, int scope)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (!variables[c].varName && variables[c].reference.index == index)
			return &variables[c];
	}
	return nullptr;
}

int VariableList::clearVariable(TString name, bool lowScope)
{
	return 0;
}
