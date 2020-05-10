#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include "Anaface.h"
class TControl;

class _ANAFACE_DLL TParentHolder : public TObject
{
public:
	TParentHolder();

	virtual void SwitchChildControl(TrecPointerSoft<TControl>, TrecPointer<TControl>) = 0;
	virtual TrecPointer<TControl> GetParent();
};

