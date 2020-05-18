#include "TParentHolder.h"

/**
 * Method: TParentHolder::TParentHolder
 * Purpose: Constructor
 * Parameters: void
 * Returns: New TParentHolder
 */
TParentHolder::TParentHolder()
{
}


/**
 * Method: TParentHolder::GetParent
 * Purpose: Allows the Retrieval of the Parent Control (if the holder is holding a control)
 * Parameters: void
 * Returns: TrecPointer<TControl> - the Parent (the default returns null but the TControlParentHolder will return the parent)
 */
TrecPointer<TControl> TParentHolder::GetParent()
{
	return TrecPointer <TControl>();
}
