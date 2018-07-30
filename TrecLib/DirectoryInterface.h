#pragma once
#include "stdafx.h"
#include "TString.h"

/*
* enum CentralDirectories
* Represents possible directory types AnaGame and its taps might be interested in
*/
typedef enum CentralDirectories
{
	cd_Executable,
	cd_AppData,
	cd_Desktop,
	cd_Documents,
	cd_Music,
	cd_Pictures,
	cd_Videos,
	cd_Downloads
}CentralDirectories;

/*
* Function: GetDirectory
* Purpose: Gets the directory specified
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetDirectory(CentralDirectories cd);

/*
* Function: GetDirectoryWithSlash
* Purpose: Gets the directory specified with a slash at the end
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString _TREC_LIB_DLL GetDirectoryWithSlash(CentralDirectories cd);