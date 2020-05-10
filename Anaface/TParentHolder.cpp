#include "TParentHolder.h"

TParentHolder::TParentHolder()
{
}

TrecPointer<TControl> TParentHolder::GetParent()
{
	return TrecPointer <TControl>();
}
