#include "stdafx.h"
#include "DoubleOpTag.h"


DoubleOpTag::DoubleOpTag(TString& tagName) : BNFTag(tagName)
{
}


DoubleOpTag::~DoubleOpTag()
{
}

dualExpression DoubleOpTag::Get2Expressions(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags)
{
	TagCheck left, right;

	bool expressionsParsed = false;
	bool lessThan2 = false;

	for (UINT rust = 0; rust < syntax.Size(); rust++)
	{
		ProcessedCode pc = PreProcessLine(bounds, rust);
		if (!pc.worked)
			continue;

		TDataArray<TagCheck> expressions;
		for (UINT c = 0; c < pc.expressions.Size(); c++)
		{
			expressions.push_back(tags[pc.expressions[c].tagProcess]->ProcessTag(pc.expressions[c].code, globalVariables, inter, tags));
		}

		if (expressions.Size() < 2)
		{
			lessThan2 = true;
			continue;
		}

		lessThan2 = false;

		left = expressions[0];
		right = expressions[1];

		if (left.success && right.success)
		{
			expressionsParsed = true;
			break;
		}
	}

	USHORT e = 0;
	if (lessThan2)
		e = 2;
	else
	{
		if (!left.success)
			e = e | 0x0001;
		if (!right.success)
			e = e | 0x0002;
	}

	return dualExpression{ left, right, e };
}
