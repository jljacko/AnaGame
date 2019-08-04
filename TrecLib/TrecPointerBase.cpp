
#include "TrecPointerBase.h"
#include "TrecPointerI.h"

/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/


/*
* Method: (TrecPointerBase) (Constructor) 
* Purpose: Basic Set up
* Parameters: void
* Returns: void
*/
TrecPointerBase::TrecPointerBase()
{
	dontDeRef = false;
}

/*
* Method: (TrecPointerBase) (Constructor) 
* Purpose: Provides a copy constructor
* Parameters: TrecPointerBase& copy - the TrecPointer to base itself off of
* Returns:
*/
TrecPointerBase::TrecPointerBase(TrecPointerBase & copy/*, const char * type*/)
{	timeCount = copy.timeCount;
	index = copy.index;
	dontDeRef = copy.dontDeRef;
	IncrementCount(/*type*/);
}

/*
* Method: (TrecPointerBase) (Destructor) 
* Purpose: Cleans up (nothing)
* Parameters: void
* Returns: void
*/
TrecPointerBase::~TrecPointerBase()
{
}

/*
* Method: TrecPointerBase - clean
* Purpose: Enables a Trec Pointer to be deleted
* Parameters: void
* Returns: void
* Note: DEPRECIATED - fixes a problem that no longer exists
*/
void TrecPointerBase::clean()
{
	resetHold();
	DecrementCount();
}

/*
* Method: TrecPointerBase - Boost
* Purpose: Boosts a pointer for protection from deletion
* Parameters: void
* Returns: void
* Note: DEPRECIATED - used before Copy Constructors were introduced
*/
void TrecPointerBase::Boost()
{
	::BoostPointer(index, timeCount);
}

/*
* Method: TrecPointerBase - IncrementCount
* Purpose: Increases the count of the reference
* Parameters: void* ref - the reference to increase the count for
* Returns: void
*/
void TrecPointerBase::IncrementCount(void * ref, const char* type)
{
	if (!ref)
	{
		index = -1;
		return;
	}
	int i = InsertNewReference(ref, index/*,type*/);
	
	timeCount = static_cast<unsigned char>(i >> 16);

}

/*
* Method: TrecPointerBase - IncrementCount
* Purpose: Increases the count for an existing reference
* Parameters:
* Returns:
*/
void TrecPointerBase::IncrementCount(const char* type)
{
	InsertNewReference(index, timeCount/*, type*/);
}

/*
* Method: TrecPointerBase - Decrement Count
* Purpose: Lowers the count of the Reference once Trec Pointer is no longer referencing it
* Parameters: void (const char* type is depreciated and should be removed)
* Returns: void* - null if count is still above 0, the reference if time for deletion
*/
void* TrecPointerBase::DecrementCount(const char* type)
{
	return DeReference(index, timeCount/*,type*/);
}

/*
* Method: TrecPointerBase - NukeCount
* Purpose: Enables the deletion of the object regardless of how many references there still is to it
* Parameters: void (const char* type is depreciated and should be removed)
* Returns: void* - null if reference is no longer valid or the reference if deletion is possible
*/
void* TrecPointerBase::NukeCount(const char* type)
{
	return ::Delete(index, timeCount/*,type*/);
}

/*
* Method: TrecPointerBase - SetTrecPointer 
* Purpose: Sets the attributes based off of the other Trec Pointer
* Parameters: TrecPointerBase& base - the pointer to copy from
* Returns: void
*/
void TrecPointerBase::SetTrecPointer(TrecPointerBase & base)
{
	timeCount = base.timeCount;
	index = base.index;
	dontDeRef = base.dontDeRef;
}

/*
* Method: TrecPointerBase - Get
* Purpose: Returns the Reference to the Trec Pointer
* Parameters: void
* Returns: void* the reference pointer for this Trec Pointer
*/
void * TrecPointerBase::Get()
{
	if (index < 0)
		return nullptr;
	void* returnable = ::Get(index, timeCount);

	//details = details & 0b10000001;

	if (!returnable)
	{
		timeCount = 0;
		index = -1;
	}
	
	return returnable;
}

/*
* Method: TrecPointerBase - nullify
* Purpose: Nullifies the TrecPointer
* Parameters: void
* Returns: void
*/
void TrecPointerBase::nullify()
{
	index = -1;
}

/*
* Method: TrecPointerBase - init
* Purpose: Initializes the Trec Pointer
* Parameters: void* ref - the reference to use
* Returns: void
*/
void TrecPointerBase::init(void * ref, const char* type)
{
	if (ref)
	{
		IncrementCount(ref,type);
	}
	else
	{
		index = -1;
	}
	dontDeRef = false;
}

/*
* Method: TrecPointerBase - setHold
* Purpose: Nothing
* Parameters: void
* Returns: void
*/
void TrecPointerBase::setHold()
{

}

/*
* Method: TrecPointerBase - getHold
* Purpose: Nothing
* Parameters: void 
* Returns: bool - whether the Pointer is on hold
*/
bool TrecPointerBase::getHold()
{
	return timeCount & 0b10000000;
}

/*
* Method: TrecPointerBase - resetHold
* Purpose: Intended for the undoing of the Boot Method, but does nothing
* Parameters: void
* Returns: void
*/
void TrecPointerBase::resetHold()
{

}

/*
* Method: TrecPointerBase - stopDeRef
* Purpose: protects TrecPointer from being dereferenced
* Parameters: void
* Returns: void
*/
void TrecPointerBase::stopDeRef()
{
	dontDeRef = true;
}

/*
* Method: TrecPointerBase - enableDeRef
* Purpose: allows TrecPointer to be dereferenced
* Parameters: void
* Returns: void
*/
void TrecPointerBase::enableDeRef()
{
	dontDeRef = false;
}

/*
* Method: TrecPointerBase - getRefStatus
* Purpose: reports whether Trec Pointer is protected from dereferencing
* Parameters: void
* Returns: bool - whether reference is protected from dereferencing
*/
bool TrecPointerBase::getRefStatus()
{
	return dontDeRef;
}

UINT TrecPointerBase::getCount()
{
	return ::GetCount(index, timeCount);
}


