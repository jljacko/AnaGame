#include "stdafx.h"
#include "NullTag.h"


NullTag::NullTag() : BNFTag(TString(L"null"))
{
}


NullTag::~NullTag()
{
}

TagCheck NullTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	bounds.Trim();

	TagCheck ret;

	

	ret.fileByteEnd = 0;
	ret.returnValue = nullptr;

	ret.success = false;

	if (!syntax.Size())
	{
		ret.error.Set(TString(L"Null Tag not Compiled properly."));
		return ret;
	}

	for (UINT c = 0; c < syntax.Size(); c++)
	{
		if (syntax[c].Size() == 0)
			continue;

		if (syntax[c][0].mark == bounds)
		{
			ret.returnValue = new intVariable();
			ret.returnValue->value.object = nullptr;  // Since this is a Null Tag

			// TO-DO: Determine the means of expressing the object (as oppposed to a number)
			ret.success = true;
			Log(lt_code, TString(L"Null Tag"));
			return ret;
		}
	}
	ret.error.Set(TString(L"Not 'Null' Expression"));
	return ret;
}
