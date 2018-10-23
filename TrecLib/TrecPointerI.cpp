#include "TrecPointerI.h"
#include "stdafx.h"
#include <Unknwn.h>
#include <vector>
#include <queue>
#include <typeinfo>
#include "TString.h"
#include <random>

// ReferenceCount
// Keeps track of Trec References used by AnaGame and it's TAPs
typedef struct ReferenceCount
{
	void* pointer;            // Actual pointer
	unsigned short count;     // NUmber of references to it
	unsigned char timeCount;  // Used to flag dangling pointers
}ReferenceCount;

static CFile refFile;
static CArchive* arch = nullptr;

/*
* Function: 
* Purpose:
* Parameters:
* Returns:
*/

/*
* Function: InitializeFile
* Purpose: Set up file for debugging
* Parameters: void
* Returns: bool - success
* Note: DEPRECIATED - used for debugging purposes
*/
bool InitializeFile()
{
	if (arch)
		return true;

	WCHAR filepath[300];
	
	ASSERT(GetModuleFileNameW(nullptr, filepath, 299));
	UINT c = wcslen(filepath) - 1;
	for (; filepath[c] != L'\\' && filepath[c] != L'/'; c--)
	{

	}
	filepath[c + 1] = L'\0';
	TString fileNamePath(filepath);
	fileNamePath += L"TrecPointerDebug.txt";

	if (!refFile.Open(fileNamePath, CFile::modeCreate | CFile::modeWrite))
		return false;
	arch = new CArchive(&refFile, CArchive::store);
	return arch;
}

/*
* Function: Shutdown
* Purpose: Shuts down the debugging file
* Parameters: void
* Returns: void
* Note: DEPRECIATED - used only for debugging purposes
*/
void Shutdown()
{
	if (arch)
	{
		arch->Close();
		delete arch;
		arch = nullptr;
	}
	refFile.Close();
}

/*WCHAR* getWCHARSfromchars(const char* str)
{
	if (!str)
		return nullptr;
	const UINT size = strlen(str)+1;
	WCHAR* wc = new WCHAR[size];
	UINT con = 0;
	mbstowcs_s(&con, wc, size, str, size -1);
	return wc;
}*/


std::vector<ReferenceCount> ReferenceTable;// The Reference Table
std::queue<UINT> freeSpaces;               // Used to find a free space in the Reference Table

/*
* Function: FindReferencePlace
* Purpose: Locates the Reference in the table
* Parameters: void* ref - the Reference to look for
* Returns: int - the reference index
*/
 int FindReferencePlace(void* ref)
{
	for (size_t c = 0; c < ReferenceTable.size(); c++)
	{
		if (ReferenceTable[c].pointer == ref && ReferenceTable[c].timeCount != 0b11111111)
			return c; // This reference is already in the table, look at that!
	}
	return -1; // Reference is not in the table, need to set a new space
}

 /*
 * Function: InsertNewReference
 * Purpose: Adds a new Reference to the table, used for brand new TrecPointers
 * Parameters: void* ref - the reference to add
 *			UINT& index - reference to the index to update
 * Returns: int - Holds the time count of the new reference
 */
 int InsertNewReference(void* ref, UINT& index/*, const char* type*/)
{

	//ReferenceCount rc = { ref, 1 };
	int refLoc = FindReferencePlace(ref);
	int returnable = 0;
	ReferenceCount rc;
	if (refLoc == -1)   // if True, we have a new pointer and we need to find a place for it in the table
	{
		if (freeSpaces.empty()) // We do not have a free space, need to add a new one
		{
			// Time count can remain 0

			rc.count = 1;                  // Initializing a new pointer, there will be one reference to it now

			rc.pointer = ref;              // Set the pointer
			rc.timeCount = rand() % 255;   // Add some randomness to thrawt malware
			ReferenceTable.push_back(rc);  // Add to the reference table
			refLoc = ReferenceTable.size() - 1;
			index = refLoc;                // Allow the Requesting TrecPointer to know the index to its reference is
			returnable = rc.timeCount << 16; // Allow the TrecPointer to know the time_count (key) to this reference
		}
		else
		{
			refLoc = freeSpaces.front();

			ASSERT(refLoc < ReferenceTable.size());
			freeSpaces.pop();
			ReferenceTable[refLoc].pointer = ref;
			ReferenceTable[refLoc].timeCount++;  // So Any dangling Trec Pointers will know that this reference is no longer valid for them 
			ReferenceTable[refLoc].count = 1;
			index = refLoc;
			returnable = ReferenceTable[refLoc].timeCount << 16;
			
		}
	}
	else
	{
		//if (details != ReferenceTable[refLoc].details)
		//{
		//	throw new std::bad_cast();
		//}
		returnable = static_cast<int>(ReferenceTable[refLoc].timeCount) << 16;
		index = refLoc;
		ReferenceTable[refLoc].count++;
	}

	/*if (type && InitializeFile())
	{
		try
		{
			TString formater;
			WCHAR* wc_type = getWCHARSfromchars(type);
			TString tType = wc_type;
			delete[] wc_type;
			formater.Format(L"Reference Increase at index %d at address %p with counter %hu and timeCount %hhu for type ", refLoc, ReferenceTable[refLoc].pointer,
				ReferenceTable[refLoc].count, ReferenceTable[refLoc].timeCount);
			formater += tType;
			formater += L"\n";
			arch->WriteString(formater);
			
		}
		catch (CArchiveException& e)
		{

				WCHAR er[400];
				e.GetErrorMessage(er, 399);
				TRACE(er);
			
		}
		catch (CFileException& e)
		{

				WCHAR er[400];
				e.GetErrorMessage(er, 399);
				TRACE(er);
			
		}
		catch (CMemoryException& e)
		{

				WCHAR er[400];
				e.GetErrorMessage(er, 399);
				TRACE(er);
			
		}
	}*/

	return returnable;
}

 /*
 * Function: InsertNewReference
 * Purpose: Adds a reference to an existing entry (Not this version expects the entry to already be set
 * Parameters: UINT index - the index to the reference in the table
 *			UCHAR& tc - the time count of the reference
 * Returns: bool - success rate (expected to be true)
 */
 bool InsertNewReference(UINT index, UCHAR& tc/*, const char * type*/)
 {
	 if(ReferenceTable.size() <= index)
		return false;

	 if (!ReferenceTable[index].pointer)
		 return false;
	 ReferenceTable[index].count++;
	 ASSERT(tc == ReferenceTable[index].timeCount);
	 /*if (type && InitializeFile())
	 {
		 TString formater;
		 //MultiByteToWideChar(CP_ACP, 0, type, -1, )
		 WCHAR* wc_type = getWCHARSfromchars(type);
		 TString tType = wc_type;
		 delete[] wc_type;
		 formater.Format(L"Reference Increase at index %hu at address %p with counter %hu and timeCount %hhu for type ", index, ReferenceTable[index].pointer,
			 ReferenceTable[index].count, ReferenceTable[index].timeCount);
		 formater += tType;
		 formater += L"\n";
		 arch->WriteString(formater);
	 }*/
	 return true;
 }

 /*
 * Function: BoostPointer
 * Purpose: Boosts the references time count to protect it from premature deletion
 * Parameters:  UINT index - index of the reference entry
 *			unsigned char time - key to determine if the trec pointer making the request is legitimate
 * Returns: void
 * Note: DEPRECIATED: Used for debugging purposes and negated by introduction of copu constructors
 *	Return statement at beginning ensures it does nothing
 */
 void BoostPointer(UINT index, unsigned char time)
 {
	 return;
	 if (ReferenceTable.size() > index)
	 {
		 if (ReferenceTable[index].timeCount == time)
			 if (ReferenceTable[index].count == 1)
				 ReferenceTable[index].count++;
	 }
 }

 /*
 * Function: GetStackBase
 * Purpose: Returns the base of the stack
 * Parameters: void
 * Returns: UINT32 - the base of the stack
 */
 UINT32 GetStackBase()
 {
	 __asm
	 {
		 mov eax, ebp
	 }
 }

 /*
 * Function: GetStackTop
 * Purpose: Gets the top of the current stack
 * Parameters: void
 * Returns: UINT32 - the top of the stack
 */
 UINT32 GetStackTop()
 {
	 __asm mov eax, esp
 }

 /*
 * Function: InStack
 * Purpose: Determines if a given reference is within the stack
 * Parameters: UINT32 ptr - the pointer to check
 * Returns: bool - is pointer within the stack
 */
 bool InStack(UINT32 ptr)
 {
	 UINT32 stackBase = GetStackBase();
	 UINT32 stackTop = GetStackTop();
	 

	 return stackTop < ptr;// && ptr < stackBase;
 }

 /*
 * Function: Delete
 * Purpose: Returns an entry for the Trec Pointer to delete (or Release)
 * Parameters: UINT index - Index of the Pointer to delete
 * Returns: void* - the pointer being held
 */
 void* Delete(UINT index, UCHAR time /*, const char* type*/)
{
	// Set up null pointer to return
	void* ptr = nullptr;
	if (ReferenceTable.size() > index)
	{
		if (ReferenceTable[index].pointer && ReferenceTable[index].timeCount == time)
		{
			// Found the reference to delete, set it to return
			ptr = ReferenceTable[index].pointer;


			ReferenceTable[index].count = 0;
			ReferenceTable[index].pointer = nullptr;

			freeSpaces.push(static_cast<UINT>(index));

			// If pointer is in stack, return null since object WILL be deleted once it goes out of scope
			// We should not delete it ahead of time.
			//if (InStack(reinterpret_cast<UINT32>(ptr)))
			//	ptr = nullptr;

			// We leave the Time Count alone because that is our means of handling dangling Trec Pointers,
			// as well as over used indexes 
		}
	}
	/*if (ptr && type && InitializeFile())
	{
		TString formater;
		WCHAR* wc_type = getWCHARSfromchars(type);
		TString tType = wc_type;
		delete[] wc_type;
		formater.Format(L"Deletion set at index %hu at address %p with counter %hu and timeCount %hhu for type ", index, ptr,
			ReferenceTable[index].count, ReferenceTable[index].timeCount);
		formater += tType;
		formater += L"\n";
		arch->WriteString(formater);
	}*/
	return ptr;
}

 /*
 * Function: DeReference
 * Purpose: Lowers the Counter for a given pointer, returning the pointer if count reaches 0
 * Parameters: UINT index - index of the reference entry
 *			unsigned char time - key to determine if the trec pointer making the request is legitimate
 * Returns: void* - the pointer being held or NULL if counter is still on
 */
 void* DeReference(UINT index, UCHAR time/*, const char* type*/)
{
	if (ReferenceTable.size() > index && ReferenceTable[index].count)
	{

		if (time != ReferenceTable[index].timeCount)
			return nullptr;

		ReferenceTable[index].count--;

		if (!ReferenceTable[index].count)
			return Delete(index, time/*,type*/);
		/*else if (type && InitializeFile())
		{
			TString formater;
			WCHAR* wc_type = getWCHARSfromchars(type);
			TString tType = wc_type;
			delete[] wc_type;
			formater.Format(L"Reference Decrease at index %hu at address %p with counter %hu and timeCount %hhu for type ", index, ReferenceTable[index].pointer,
				ReferenceTable[index].count, ReferenceTable[index].timeCount);
			formater += tType;
			formater += L"\n";
			arch->WriteString(formater);
		}*/


	}
	if (ReferenceTable.size() > index && !ReferenceTable[index].count)
		return Delete(index, time/*,type*/);
	return nullptr;
}

 /*
 * Function: Get
 * Purpose: Returns the pointer to the Trec Pointer requesting it
 * Parameters: UINT index - index of the reference entry
 *			unsigned char time - key to determine if the trec pointer making the request is legitimate
 * Returns: void* - the pointer being held or NULL if time is off
 */
void* Get(UINT index, UCHAR time)
{

	if (index >= ReferenceTable.size())
		return nullptr;
	if (time == ReferenceTable[index].timeCount)
		return ReferenceTable[index].pointer;
	return nullptr;
}
