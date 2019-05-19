#include "stdafx.h"
#include "VariableTree.h"
#include "TInterpretor.h"

VariableTree::VariableTree()
{
	root.variable.hold = 0;
	root.variable.varName = true;
	root.variable.reference.varName.Set(TString());
}


VariableTree::~VariableTree()
{
}

int VariableTree::insertVariable(TString name, TrecPointer<TInterpretor> value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, TrecPointer<VariableList> value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, unsigned long long value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, TrecPointer<TObject> obj)
{
	return insertVariable(name, obj, root);
}

int VariableTree::insertVariable(TString name, TString & value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, long long value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, double value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, bool value)
{
	return insertVariable(name, value, root);
}

intVariable * VariableTree::getVariable(TString name, bool lowScope, int scope)
{
	return getVariable(name, root, lowScope, scope);
}

int VariableTree::clearVariable(TString name, bool lowScope)
{
	return 0;
}

int VariableTree::insertVariable(TString name, TrecPointer<TObject> obj, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_OBJECT;
		node.variable.reference.varName.Set(name);
		node.variable.value.object = obj;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_OBJECT;
		node.variable.value.object = obj;
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, obj, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, obj, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, obj, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, obj, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, TString & value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_STRING;
		node.variable.reference.varName.Set(name);
		node.variable.value.object = new TString(value);
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_STRING;
		node.variable.value.object = new TString(value);
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, unsigned long long value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_U_INT;
		node.variable.reference.varName.Set(name);
		node.variable.value.primInt = (long long)value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_U_INT;
		node.variable.value.primInt = (long long)value;
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, long long value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_S_INT;
		node.variable.reference.varName.Set(name);
		node.variable.value.primInt = value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_S_INT;
		node.variable.value.primInt = value;
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, double value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_DOUBLE;
		node.variable.reference.varName.Set(name);
		node.variable.value.primFloat = value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_DOUBLE;
		node.variable.value.primFloat = value;
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, bool value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_BOOLEAN;
		node.variable.reference.varName.Set(name);
		node.variable.value.primInt = value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_BOOLEAN;
		node.variable.value.primInt = value;
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

intVariable * VariableTree::getVariable(TString name, varNode & node, bool lowScope, int scope)
{
	if(!node.variable.reference.varName.Compare(name))
		return &node.variable;

	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (!node.higher.get())
			return nullptr;
		return getVariable(name, *node.higher.get(), lowScope, scope);
	}
	if (!node.lower.get())
		return nullptr;
	return getVariable(name, *node.lower.get(), lowScope, scope);
}

int VariableTree::insertVariable(TString name, TrecPointer<TInterpretor> value, varNode& node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_FUNCTION;
		node.variable.reference.varName.Set(name);
		node.variable.value.object = value.get();
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_FUNCTION;
		node.variable.value.object = value.get();
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, TrecPointer<VariableList> value, varNode& node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = AG_I_ARRAY;
		node.variable.reference.varName.Set(name);
		node.variable.value.object = value.get();
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = AG_I_ARRAY;
		node.variable.value.object = value.get();
		return 2;
	}
	if (node.variable.reference.varName.Compare(name) > 0)
	{
		if (node.higher.get())
			return insertVariable(name, value, *node.higher.get());
		node.higher = new intVariable();
		node.higher->variable.hold = 0;
		node.higher->variable.varName = true;
		node.higher->variable.reference.varName.Set(L"");
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName.Set(L"");
	insertVariable(name, value, *node.lower.get());
	return 4;
}


varNode::varNode()
{
}

varNode::~varNode()
{
}
