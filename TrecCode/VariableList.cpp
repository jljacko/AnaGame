#include "stdafx.h"
#include "VariableList.h"
#include "TInterpretor.h"

VariableList::VariableList()
{
}


VariableList::~VariableList()
{
}

int VariableList::insertVariable(TString name, TrecPointer<TInterpretor> value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = AG_I_FUNCTION;
			variables[c].value.object = dynamic_cast<TObject*>(value.get());
			return 1;
		}
	}
	intVariable var;
	var.hold = AG_I_FUNCTION;
	var.reference.varName = name;
	var.varName = true;
	var.value.object = value.get();
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, TrecPointer<VariableList> value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = AG_I_ARRAY;
			variables[c].value.object = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = AG_I_ARRAY;
	var.reference.varName = name;
	var.varName = true;
	var.value.object = value;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, TrecPointer<TObject> obj, UCHAR type)
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
	var.hold = type;
	var.reference.varName = name;
	var.varName = true;
	var.value.object = obj;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, TString & value)
{
	TString* newString = new TString(value);
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = AG_I_STRING;
			
			variables[c].value.object = newString;
			return 1;
		}
	}
	intVariable var;
	var.hold = AG_I_STRING;
	var.reference.varName = name;
	var.varName = true;
	var.value.object = newString;
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

int VariableList::insertVariable(TString name, bool value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = AG_I_BOOLEAN;
			variables[c].value.primInt = value;
			return 1;
		}
	}
	intVariable var;
	var.hold = AG_I_BOOLEAN;
	var.reference.varName = name;
	var.varName = true;
	var.value.primInt = value;
	variables.push_back(var);
	return 0;
}

int VariableList::insertVariable(TString name, unsigned long long value)
{
	for (UINT c = 0; c < variables.Size(); c++)
	{
		if (variables[c].varName && !variables[c].reference.varName.Compare(name))
		{
			variables[c].hold = AG_I_U_INT;
			variables[c].value.primInt = (long long)value;
			return 1;
		}
	}
	intVariable var;
	var.hold = AG_I_U_INT;
	var.reference.varName = name;
	var.varName = true;
	var.value.primInt = (long long)value;
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

TDataArray<intVariable> VariableList::GetVariableListCopy()
{
	TDataArray<intVariable> returnable;

	for (UINT Rust = 0; Rust < variables.Size(); Rust++)
	{
		returnable.push_back(variables[Rust]);
	}

	return returnable;
}
