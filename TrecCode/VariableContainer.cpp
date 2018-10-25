#include "stdafx.h"
#include "VariableContainer.h"


VariableContainer::VariableContainer()
{
}


VariableContainer::~VariableContainer()
{
}

intVariable::intVariable()
{
}

intVariable::intVariable(intVariable & iv)
{
	this->hold = iv.hold;
	if (iv.varName)
		this->reference.varName = iv.reference.varName;
	else
		this->reference.index = iv.reference.index;
	switch (this->hold)
	{
	case 1:
		this->value.object = iv.value.object;
		break;
	case 2:
		this->value.primInt = iv.value.primInt;
		break;
	case 3:
		this->value.primFloat = iv.value.primFloat;
	}
	this->varName = iv.varName;

}

intVariable::~intVariable()
{
}

elementRef::elementRef()
{
}

elementRef::elementRef(elementRef & er)
{
	this->index = er.index;
	this->varName = er.varName;
}

elementRef::~elementRef()
{
}

varVal::varVal()
{
}

varVal::~varVal()
{
}
