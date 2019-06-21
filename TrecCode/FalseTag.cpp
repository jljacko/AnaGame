#include "stdafx.h"
#include "FalseTag.h"


FalseTag::FalseTag() : BNFTag(TString(L"false"))
{
}


FalseTag::~FalseTag()
{
}


TagCheck FalseTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	bounds.Trim();

	TagCheck ret;



	ret.fileByteEnd = 0;
	ret.returnValue = nullptr;

	ret.success = false;

	if (!syntax.Size())
	{
		ret.error = TString(L"Error! Null Tag not Compiled properly.");
		return ret;
	}

	for (UINT c = 0; c < syntax.Size(); c++)
	{
		if (syntax[c].Size() == 0)
			continue;

		if (syntax[c][0].mark == bounds)
		{
			ret.returnValue = new intVariable();
			ret.returnValue->value.primInt = 0;  // Since this is a false Tag

			// TO-DO: Determine the means of expressing the object (as oppposed to a number)
			ret.success = true;
			return ret;
		}
	}
	ret.error = TString(L"Not 'Null' Expression");
	return ret;
}