#include "stdafx.h"
#include "OrExpression.h"


OrExpression::OrExpression(): DoubleOpTag(TString(L"or_exp"))
{
}


OrExpression::~OrExpression()
{
}

TagCheck OrExpression::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	dualExpression de = Get2Expressions(bounds, globalVariables, inter, lang, tags);

	switch (de.errorCode)
	{
	case 1:
		return TagCheck( false, TString(L"Left Expression not parsed Properly"), 0, nullptr);
	case 2:
		return TagCheck( false, TString(L"Right Expression not parsed Properly"), 0, nullptr);
	case 3:
		return TagCheck( false, TString(L"Both Expressions not parsed Properly"), 0, nullptr);
	case 4:
	case 5:
		return TagCheck( false, TString(L"Could not find two distinct expressions"), 0, nullptr);
	}
	long long rightInt;
	double rightFloat;
	bool rightBool = false;
	TString* expString = nullptr;

	switch (de.right.returnValue->hold)
	{
	case AG_I_ARRAY:
		// To-Do: Handle arrays


		break;
	case AG_I_FUNCTION:
	case AG_I_OBJECT:

		rightBool = de.right.returnValue->value.object.get();
		break;
	case AG_I_UNDEFINED:
		rightBool = false;
		break;
	case AG_I_STRING:
		try
		{
			expString = dynamic_cast<TString*>(de.right.returnValue->value.object.get());
			if (expString->ConvertToLong(&rightInt))
			{

				if (expString->ConvertToDouble(&rightFloat))
				{
					if (expString->CompareNoCase(L"true"))
						rightBool = true;
					else if (expString->CompareNoCase(L"false"))
						rightBool = false;
					else
					{
						return TagCheck( false, TString(L"Non Bool String compared by Boolean operation"), 0, nullptr);
					}
				}

			}
		}
		catch (...)
		{
			return TagCheck( false, TString(L"Right Expression is null or unsuited for subtraction"), 0 , nullptr);
		}
		break;
	case AG_I_DOUBLE:
		rightBool = de.right.returnValue->value.primFloat;
		break;
	case AG_I_S_INT:
	case AG_I_BOOLEAN:
	case AG_I_U_INT:
		rightInt = de.right.returnValue->value.primInt;
	}

	long long leftInt = 0;
	double leftFloat = 0.0;
	bool leftBool = false;

	switch (de.left.returnValue->hold)
	{
	case AG_I_ARRAY:
		// To-Do: Handle arrays


		break;
	case AG_I_FUNCTION:
	case AG_I_OBJECT:

		leftBool = de.right.returnValue->value.object.get();
		break;
	case AG_I_UNDEFINED:
		leftBool = false;
		break;
	case AG_I_STRING:
		try
		{
			expString = dynamic_cast<TString*>(de.left.returnValue->value.object.get());
			if (expString->ConvertToLong(&leftInt))
			{

				if (expString->ConvertToDouble(&leftFloat))
				{
					if (expString->CompareNoCase(L"true"))
						leftBool = true;
					else if (expString->CompareNoCase(L"false"))
						leftBool = false;
					else
					{
						return TagCheck( false, TString(L"Non Bool String compared by Boolean operation"), 0, nullptr);
					}
				}

			}
		}
		catch (...)
		{
			return TagCheck( false, TString(L"Right Expression is null or unsuited for subtraction"), 0 , nullptr);
		}
		break;
	case AG_I_DOUBLE:
		leftBool = de.left.returnValue->value.primFloat;
		break;
	case AG_I_S_INT:
	case AG_I_U_INT:
	case AG_I_BOOLEAN:
		leftBool = de.left.returnValue->value.primInt;
	}

	// At this point, subtraction is being done
	de.left.returnValue->hold = AG_I_BOOLEAN;
	de.left.returnValue->value.primInt = leftBool || rightBool;

	return de.left;
}
