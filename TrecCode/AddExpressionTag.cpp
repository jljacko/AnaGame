#include "stdafx.h"
#include "AddExpressionTag.h"


AddExpressionTag::AddExpressionTag() : DoubleOpTag(TString(L"add_exp"))
{
	cases = 0;
}


AddExpressionTag::~AddExpressionTag()
{
}

TagCheck AddExpressionTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags)
{
	dualExpression de = Get2Expressions(bounds, globalVariables, inter, tags);

	switch (de.errorCode)
	{
	case 1:
		return TagCheck{ false, TString(L"Left Expression not parsed Properly"), 0, nullptr };
	case 2:
		return TagCheck{ false, TString(L"Right Expression not parsed Properly"), 0, nullptr };
	case 3:
		return TagCheck{ false, TString(L"Both Expressions not parsed Properly"), 0, nullptr };
	case 4:
	case 5:
		return TagCheck{ false, TString(L"Could not find two distinct expressions"), 0, nullptr };
	}

	long long leftStringI = 0, rightStringI = 0;
	double leftStringF = 0.0, rightStringF = 0.0;

	// To-Do
	bool rightIsObject = false;
	switch (de.right.returnValue->hold)
	{
	case AG_I_BOOLEAN:
	case AG_I_FUNCTION:
	case AG_I_OBJECT:
	case AG_I_UNDEFINED:
		rightIsObject = true;
	}

	switch (de.left.returnValue->hold)
	{
	case AG_I_BOOLEAN:
	case AG_I_FUNCTION:
	case AG_I_OBJECT:
	case AG_I_UNDEFINED:
		return TagCheck{ false, TString(L"Type can't be added"), 0, nullptr };
	case AG_I_DOUBLE:
		if(rightIsObject)
			return TagCheck{ false, TString(L"Object can't be added to floating point value"), 0, nullptr };
		switch (de.right.returnValue->hold)
		{
		case AG_I_ARRAY:
			if (cases & 0x0010)
			{

			}
			else
				return TagCheck{ false, TString(L"Language does not support addinng Arrays to numbers")};
			break;
		case AG_I_DOUBLE: // In this case, both are doubles
			de.left.returnValue->value.primFloat += de.right.returnValue->value.primFloat;
			return de.left;
		case AG_I_STRING:
			
			// To-Do: Figure out efficient string handling mechanism
			//if(cases & 0x0002 || dynamic_cast<TString*>(de.right.returnValue->value.object.get())->ConvertToDouble(&rightStringF))
				
			break;
		case AG_I_S_INT:
			de.left.returnValue->value.primFloat += de.right.returnValue->value.primInt;
			return de.left;
		case AG_I_U_INT:
			de.left.returnValue->value.primFloat += (unsigned long long)de.right.returnValue->value.primInt;
			return de.left;
		}
		break;

	case AG_I_S_INT:
	case AG_I_U_INT:
		if (rightIsObject)
			return TagCheck{ false, TString(L"Object can't be added to floating point value"), 0, nullptr };
		switch (de.right.returnValue->hold)
		{
		case AG_I_ARRAY:
			if (cases & 0x0010)
			{

			}
			else
				return TagCheck{ false, TString(L"Language does not support addinng Arrays to numbers") };
			break;
		case AG_I_DOUBLE: // In this case, both are doubles
			de.left.returnValue->value.primInt += de.right.returnValue->value.primFloat;
			return de.left;
		case AG_I_STRING:

			// To-Do: Figure out efficient string handling mechanism
			//if(cases & 0x0002 || dynamic_cast<TString*>(de.right.returnValue->value.object.get())->ConvertToDouble(&rightStringF))

			break;
		case AG_I_S_INT:
			de.left.returnValue->value.primInt += de.right.returnValue->value.primInt;
			return de.left;
		case AG_I_U_INT:
			de.left.returnValue->value.primInt += (unsigned long long)de.right.returnValue->value.primInt;
			return de.left;
		}
		break;

	case AG_I_STRING:

		break;
	case AG_I_ARRAY:
		break;
	}
	

	return TagCheck();
}

void AddExpressionTag::addAttribute(TString & att, TString & val)
{
	if (!att.Compare(L"FirstExpNumString") && !val.Compare(L"Concatinate"))
	{
		cases = cases | 0x0001;
		return;
	}

	if (!att.Compare(L"SecondExpNumString") && !val.Compare(L"Concatinate"))
	{
		cases = cases | 0x0002;
		return;
	}

	if (!att.Compare(L"BothExpNumString") && !val.Compare(L"Concatinate"))
	{
		cases = cases | 0x0004;
		return;
	}

	if (!att.Compare(L"FirstExpArray") && !val.Compare(L"Push"))
	{
		cases = cases | 0x0008;
		return;
	}

	if (!att.Compare(L"SecondExpArray") && !val.Compare(L"Push"))
	{
		cases = cases | 0x0010;
		return;
	}

	if (!att.Compare(L"BothExpArray") && !val.Compare(L"Push"))
	{
		cases = cases | 0x0020;
		return;
	}


}
