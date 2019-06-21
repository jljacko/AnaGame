#pragma once
#pragma 
#include "TObject.h"


/*
* Function: Get
* Purpose: Returns the pointer to the Trec Pointer requesting it
* Parameters: UINT index - index of the reference entry
*			unsigned char time - key to determine if the trec pointer making the request is legitimate
* Returns: void* - the pointer being held or NULL if time is off
*/
_TREC_LIB_DLL void* Get(UINT index, UCHAR time);

/*
* Function: Delete
* Purpose: Returns an entry for the Trec Pointer to delete (or Release)
* Parameters: UINT index - Index of the Pointer to delete
* Returns: void* - the pointer being held
*/
_TREC_LIB_DLL void* Delete(UINT index, UCHAR time /*, const char* = nullptr*/);

/*
* Function: DeReference
* Purpose: Lowers the Counter for a given pointer, returning the pointer if count reaches 0
* Parameters: UINT index - index of the reference entry
*			unsigned char time - key to determine if the trec pointer making the request is legitimate
* Returns: void* - the pointer being held or NULL if counter is still on
*/
_TREC_LIB_DLL void* DeReference(UINT index, UCHAR time/*, const char* = nullptr*/);

/*
* Function: InsertNewReference
* Purpose: Adds a new Reference to the table, used for brand new TrecPointers
* Parameters: void* ref - the reference to add
*			UINT& index - reference to the index to update
* Returns: int - Holds the time count of the new reference
*/
_TREC_LIB_DLL int InsertNewReference(void* ref,  UINT& index /*, const char* = nullptr*/);

/*
* Function: InsertNewReference
* Purpose: Adds a reference to an existing entry (Not this version expects the entry to already be set
* Parameters: UINT index - the index to the reference in the table
*			UCHAR& tc - the time count of the reference
* Returns: bool - success rate (expected to be true)
*/
_TREC_LIB_DLL bool InsertNewReference(UINT index, UCHAR& tc/*, const char* type = nullptr*/);

/*
* Function: BoostPointer
* Purpose: Boosts the references time count to protect it from premature deletion
* Parameters:  UINT index - index of the reference entry
*			unsigned char time - key to determine if the trec pointer making the request is legitimate
* Returns: void
* Note: DEPRECIATED: Used for debugging purposes and negated by introduction of copu constructors
*	Return statement at beginning ensures it does nothing
*/
void BoostPointer(UINT index, unsigned char time);

/*
* Function: Shutdown
* Purpose: Shuts down the debugging file
* Parameters: void
* Returns: void
* Note: DEPRECIATED - used only for debugging purposes
*/
void Shutdown();

/*
* Function: GetCount
* Purpose: Reports how many pointers are currently pointing to a registered object
* Parameters: UINT index - index of the reference entry
*			unsigned char time - key to determine if the trec pointer making the request is legitimate
* Returns: UINT - how many pointers pointing to the object (0 if pointer is null)
*/
UINT GetCount(UINT index, UCHAR time);
