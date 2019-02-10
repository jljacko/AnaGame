#pragma once

#include <TDataArray.h>
#include "TrecCode.h"
/**
 * Purpose: Provide various functions needed to set up an enviromnent through which a script can run, including
 * interpretors and libraries that a script needs
*/

/* struct LangNames
 * Purpose: holds the name of a Programming language and the file extensions associated with them
 */
class _TREC_CODE_DLL LangNames : public TObject
{
public:
	TString language;
	TDataArray<TString> fileExtensions;
};


/*
 * Function: checkLanguages
 * Purpose: checks a list of languages installed and matches available 
 * Parameters: void
 * Returns: void
 */
void _TREC_CODE_DLL checkLanguages();


/*
 * Function: retrieveLanguages
 * Purpose: Returns a constant list of languages detected
 * Parameters: void
 * Returns: const TDataArray<LangNames>* const - the list of languages detected
 */
// const TDataArray<LangNames>* const _TREC_CODE_DLL retrieveLanguages();

/*
 * Function: retrieveLanguageByExtension
 * Purpose: Returns a Language name based off of the provided file extension
 * Parameters: TString ext - the file extension to check
 * Returns: TString - the name of the language (if not found, String is empty)
 */
TString _TREC_CODE_DLL retrieveLanguageByExtension(TString ext);

/*
 * Function: retrieveLanguageByIndex
 * Purpose: Returns the name of the language specified in the Index
 * Parameters: UINT index - the index of the language database
 * Returns: TString - the name of the language or an empty string if out of bounds
 */
TString _TREC_CODE_DLL retrieveLanguageByIndex(UINT index);

/*
 * Function: retrieveExtensionsByIndex
 * Purpose: Returns the extensions of the language specified in the Index
 * Parameters: UINT index - the index of the language database
 * Returns: TDataArray<TString> - the extensions of the language or an empty array if out of bounds
 */
TDataArray<TString> _TREC_CODE_DLL retrieveExtensionsByIndex(UINT index);