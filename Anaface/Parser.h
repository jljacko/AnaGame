#pragma once
#include <TString.h>
#include "stdafx.h"
#include "Anaface.h"
class _ANAFACE_DLL Parser
{
public:
	Parser();
	virtual ~Parser();

	// for the initial object type
	virtual bool Obj(TString v);
	// for the attribute name
	virtual bool Attribute(TString v, TString e);
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v);
	virtual bool submitEdition(TString v);

	virtual bool goChild();
	virtual void goParent();
};

