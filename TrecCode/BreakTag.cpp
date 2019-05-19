#include "stdafx.h"
#include "BreakTag.h"


BreakTag::BreakTag() : BNFTag(TString(L"break"))
{
}


BreakTag::~BreakTag()
{
}

TagCheck BreakTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, TDataArray<BNFTag*>& tags)
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
			inter.SendFlowMessage(im_break, nullptr);
			ret.success = true;
			return ret;
		}
	}
	return ret;
}