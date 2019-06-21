#include "stdafx.h"
#include "VarTag.h"


VarTag::VarTag() : BNFTag(TString(L"var"))
{
	create = false;
}


VarTag::~VarTag()
{
}

TagCheck VarTag::ProcessTag(TString & bounds, VariableContainer & globalVariables, TInterpretor & inter, IntLanguage& lang, TDataArray<BNFTag*>& tags)
{
	bounds.Trim();
	
	intVariable* var = inter.GetVariable(bounds);
	TagCheck ret;
	ret.returnValue = var;
	ret.success = true;
	ret.fileByteEnd = 0;

	if (!var)
	{
		if (create)
		{
			var = new intVariable();
			ret.returnValue = var;
			ret.error.Set(bounds);
			
		}
		else
		{
			ret.success = false;
			ret.error.Set(bounds + TString(L" Does not appear to exist in Scope"));
		}
	}
	
	return ret;
}
