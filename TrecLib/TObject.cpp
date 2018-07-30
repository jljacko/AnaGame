#include "stdafx.h"
#include "TObject.h"

UCHAR TObjectType[] = { 1, 0b10000000 };




/*
* Method: (TObject) (Constructor)
* Purpose: Default constructor for the TObject
* Parameters: void
* Returns: void
*/
TObject::TObject()
{
	//sys_Type = new LPCTSTR((LPCTSTR)"SYS_TOBJECT");
}

/*
* Method: (TObject) (Destructor)
* Purpose: Cleans up TObject
* Parameters: void
* Returns: void
*/
TObject::~TObject()
{
	//if (sys_Type)
		//delete sys_Type;
	sys_Type = NULL;
}

/*
* Method: TObject - getType
* Purpose: Returns a Pointer to a new copy of the Data type.For security,
*	a new copy is generated to preserve the integrity of the original
* Parameters: void
* Returns: LPCTSTR* String Representation of the type
* Note: DEPRECIATED - type based mechanism depreciated in favor of getAnaGameType
*/
LPCTSTR* TObject::getType()
{
	//return new LPCTSTR(*sys_Type);
	return NULL;
}

/*
* Method: TObject - alloStack
* Purpose: Allocates a stack for a TObject. This is used to extend TObject
*	to a subclass defined in Animate bytecode to be interpreted by Animate,
*	not by the native compiler
* Parameters: int s - the size of the extra data
* Returns: unsigned char* - the base of the new stack
*/
unsigned char* TObject::alloStack(int s)
{
	if (startStack)
		return NULL;
	// need to add catch mechanism for failure
	try
	{
		startStack = new unsigned char[s];
	}
	catch (std::bad_alloc& ac)
	{
		return NULL;
	}
	stackSize = s;
	endStack = startStack + sizeof(unsigned char) * s;
	return startStack;
}

/*
* Method: TObject - deAlloStack 
* Purpose: Removed the extra data fromt the TObject
* Parameters: void
* Returns: void
* Note: RESTRICTED - method not meant to be used outside of the AnaGame Virtual machine and could be REMOVED
*/
void TObject::deAlloStack()
{
	if (startStack)
		delete[] startStack;
	stackSize = 0;
	endStack = nullptr;
	startStack = nullptr;
}

/*
* Method: TObject - getStack
* Purpose: Gets the  data not declared in C++ but rather in AnaGame bytecode
* Parameters: void
* Returns: unsigned char* - the base of the extra data
* Note: RESTRICTED - method not meant to be used outside of the AnaGame Virtual machine and could be REMOVED
*/
unsigned char* TObject::getStack()
{
	return startStack;
}

/*
* Method: TObject - getStackSize
* Purpose: Gets the size of the data not declared in C++ but rather in AnaGame bytecode
* Parameters: void
* Returns: int - the size of the extra data 
* Note: RESTRICTED - method not meant to be used outside of the AnaGame Virtual machine and could be REMOVED
*/
int TObject::getStackSize()
{
	return stackSize;
}

/*
* Method: TObject - GetAnaGameType
* Purpose: Retrieves the AnaGame type
* Parameters: void
* Returns: UCHAR* - the AnaGame type represenation 
*/
UCHAR * TObject::GetAnaGameType()
{
	return TObjectType;
}

/*
* Function: boolToString 
* Purpose: returns string representations of the bool
* Parameters: bool val -  the bool to convert
* Returns: WCHAR* string representations
*/
WCHAR* boolToString(bool val)
{
	if (val)
		return L"true";
	else
		return L"false";
}