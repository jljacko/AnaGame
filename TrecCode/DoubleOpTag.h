#pragma once
#include "BNFTag.h"

typedef struct dualExpression
{
	TagCheck left;
	TagCheck right;
	USHORT errorCode;
}dualExpression;


class DoubleOpTag :	public BNFTag
{
public:
	DoubleOpTag(TString& tagName);
	~DoubleOpTag();

	dualExpression Get2Expressions(TString& bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags);
};

