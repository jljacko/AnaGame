#include "stdafx.h"
#include "VariableTree.h"


VariableTree::VariableTree()
{
	root.variable.hold = 0;
	root.variable.varName = true;
	root.variable.reference.varName = L"";
}


VariableTree::~VariableTree()
{
}

int VariableTree::insertVariable(TString name, TrecPointer<TObject> obj)
{
	return insertVariable(name, obj, root);
}

int VariableTree::insertVariable(TString name, long long value)
{
	return insertVariable(name, value, root);
}

int VariableTree::insertVariable(TString name, double value)
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
		node.variable.hold = 1;
		node.variable.reference.varName = name;
		node.variable.value.object = obj;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = 1;
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
		node.higher->variable.reference.varName = L"";
		insertVariable(name, obj, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, obj, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName = L"";
	insertVariable(name, obj, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, long long value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = 2;
		node.variable.reference.varName = name;
		node.variable.value.primInt = value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = 2;
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
		node.higher->variable.reference.varName = L"";
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName = L"";
	insertVariable(name, value, *node.lower.get());
	return 4;
}

int VariableTree::insertVariable(TString name, double value, varNode & node)
{
	if (!node.variable.reference.varName.GetLength())
	{
		node.variable.hold = 3;
		node.variable.reference.varName = name;
		node.variable.value.primFloat = value;
		return 1;
	}
	if (node.variable.reference.varName.Compare(name) == 0)
	{
		node.variable.hold = 3;
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
		node.higher->variable.reference.varName = L"";
		insertVariable(name, value, *node.higher.get());
		return 3;
	}
	if (node.lower.get())
		return insertVariable(name, value, *node.lower.get());
	node.lower = new intVariable();
	node.lower->variable.hold = 0;
	node.lower->variable.varName = true;
	node.lower->variable.reference.varName = L"";
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

varNode::varNode()
{
}

varNode::~varNode()
{
}
