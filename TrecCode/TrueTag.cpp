#include "stdafx.h"
#include "TrueTag.h"


TrueTag::TrueTag() : BNFTag(TString(L"true"))
{
}


TrueTag::~TrueTag()
{
}

TagCheck TrueTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags)
{
	bounds.Trim();

	TagCheck ret;



	ret.fileByteEnd = 0;
	ret.returnValue = nullptr;

	ret.success = false;

	if (!syntax.Size())
	{
		ret.error = TString(L"Null Tag not Compiled properly.");
		return ret;
	}

	for (UINT c = 0; c < syntax.Size(); c++)
	{
		if (syntax[c].Size() == 0)
			continue;

		if (syntax[c][0].mark == bounds)
		{
			ret.returnValue = new intVariable();
			ret.returnValue->value.primInt = 1;  // Since this is a true Tag

			// TO-DO: Determine the means of expressing the object (as oppposed to a number)
			ret.success = true;
			return ret;
		}
	}
	ret.error = TString(L"Not 'Null' Expression");
	return ret;
}
