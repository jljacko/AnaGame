#include "stdafx.h"
#include "F_DivExpressionTag.h"


F_DivExpressionTag::F_DivExpressionTag() : DoubleOpTag(TString(L"float_div_exp"))
{
}


F_DivExpressionTag::~F_DivExpressionTag()
{
}

TagCheck F_DivExpressionTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	dualExpression de = Get2Expressions(bounds, globalVariables, inter, lang, tags);

	switch (de.errorCode)
	{
	case 1:
		return TagCheck( false, TString(L"Left Expression not parsed Properly"), 0, nullptr );
	case 2:
		return TagCheck( false, TString(L"Right Expression not parsed Properly"), 0, nullptr );
	case 3:
		return TagCheck( false, TString(L"Both Expressions not parsed Properly"), 0, nullptr );
	case 4:
	case 5:
		return TagCheck( false, TString(L"Could not find two distinct expressions"), 0, nullptr );
	}
	long long rightInt = 0;
	double rightFloat = 0.0;
	bool tryRightDouble = false;
	TString* expString = nullptr;

	switch (de.right.returnValue->hold)
	{
	case AG_I_ARRAY:
	case AG_I_BOOLEAN:
	case AG_I_FUNCTION:
	case AG_I_OBJECT:
	case AG_I_UNDEFINED:
		return TagCheck( false, TString(L"Right expression is not a value that could be subtracted from"), 0, nullptr );
	case AG_I_STRING:
		try
		{
			expString = dynamic_cast<TString*>(de.right.returnValue->value.object.get());
			if (expString->ConvertToLong(&rightInt))
			{
				tryRightDouble = true;
				if (expString->ConvertToDouble(&rightFloat))
					return TagCheck( false, TString(L"RIght Expression was a non-numeric String"), 0, nullptr);
			}
		}
		catch (...)
		{
			return TagCheck( false, TString(L"Right Expression is null or unsuited for subtraction"), 0 , nullptr);
		}
	case AG_I_DOUBLE:
		rightFloat = de.right.returnValue->value.primFloat;
		tryRightDouble = true;
	case AG_I_S_INT:
	case AG_I_U_INT:
		rightInt = de.right.returnValue->value.primInt;
	}
	bool divBy0 = false;
	if (tryRightDouble && !rightFloat)
		divBy0 = true;
	else if (!rightInt)
		divBy0 = true;

	if (divBy0)
		return TagCheck( false, TString(L"Divide By 0!"), 0, nullptr );

	long long leftInt = 0;
	double leftFloat = 0.0;
	bool tryLeftDouble = false;

	switch (de.left.returnValue->hold)
	{
	case AG_I_ARRAY:
	case AG_I_BOOLEAN:
	case AG_I_FUNCTION:
	case AG_I_OBJECT:
	case AG_I_UNDEFINED:
		return TagCheck( false, TString(L"Right expression is not a value that could be subtracted from"), 0, nullptr );
	case AG_I_STRING:
		try
		{
			expString = dynamic_cast<TString*>(de.left.returnValue->value.object.get());
			if (expString->ConvertToLong(&leftInt))
			{
				tryLeftDouble = true;
				if (expString->ConvertToDouble(&leftFloat))
					return TagCheck( false, TString(L"Leftt Expression was a non-numeric String"), 0, nullptr );
			}
		}
		catch (...)
		{
			return TagCheck( false, TString(L"Left Expression is null or unsuited for subtraction"), 0 , nullptr );
		}
	case AG_I_DOUBLE:
		leftFloat = de.left.returnValue->value.primFloat;
		tryLeftDouble = true;
	case AG_I_S_INT:
	case AG_I_U_INT:
		leftInt = de.left.returnValue->value.primInt;
	}

	// At this point, subtraction is being done
	if (tryRightDouble)
	{
		if (tryLeftDouble)
		{
			de.left.returnValue->hold = AG_I_DOUBLE;
			de.left.returnValue->value.primFloat = (leftFloat / rightFloat);
		}
		else
		{
			de.left.returnValue->hold = AG_I_DOUBLE;
			de.left.returnValue->value.primFloat = ((double)leftInt) / rightFloat;
		}
	}
	else
	{
		if (tryLeftDouble)
		{
			de.left.returnValue->hold = AG_I_DOUBLE;
			de.left.returnValue->value.primFloat = leftFloat / (double)rightInt;
		}
		else
		{
			de.left.returnValue->hold = AG_I_DOUBLE;
			de.left.returnValue->value.primFloat = ((double)leftInt) / ((double)rightInt);
		}
	}
	Log(lt_code, TString(L"Div F Tag"));
	return de.left;
}
