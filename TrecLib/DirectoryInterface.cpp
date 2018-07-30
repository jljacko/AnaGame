#include "stdafx.h"
#include "DirectoryInterface.h"
//#include "TString.h"


static bool initialized = false;

static TString directories[8];

/*
* InitializeDirectories
* Purpose: Sets up the Directory list for the Get Directory functions to use
* Parameters: void
* Returns: void
*/
void InitializeDirectories()
{
	WCHAR filepath[300];

	ASSERT(GetModuleFileNameW(nullptr, filepath, 299));
	UINT c = wcslen(filepath) - 1;
	for (; filepath[c] != L'\\' && filepath[c] != L'/'; c--)
	{

	}
	filepath[c] = L'\0';

	TString tempString;

	tempString = filepath;
	directories[0] = tempString;

	PWSTR folderString = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &folderString);
	tempString = folderString;
	directories[1] = tempString;

	SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &folderString);
	tempString = folderString;
	directories[2] = tempString;

	SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &folderString);
	tempString = folderString;
	directories[3] = tempString;

	SHGetKnownFolderPath(FOLDERID_Music, 0, NULL, &folderString);
	tempString = folderString;
	directories[4] = tempString;

	SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &folderString);
	tempString = folderString;
	directories[5] = tempString;

	SHGetKnownFolderPath(FOLDERID_Videos, 0, NULL, &folderString);
	tempString = folderString;
	directories[6] = tempString;

	SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &folderString);
	tempString = folderString;
	directories[7] = tempString;

	initialized = true;
}

/*
* Function: GetDirectory
* Purpose: Gets the directory specified
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString GetDirectory(CentralDirectories cd)
{
	if (!initialized)
		InitializeDirectories();
	return directories[static_cast<UINT>(cd)];
}

/*
* Function: GetDirectoryWithSlash
* Purpose: Gets the directory specified with a slash at the end
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString GetDirectoryWithSlash(CentralDirectories cd)
{
	TString returnable = GetDirectory(cd);
	returnable += L"\\";
	return returnable;
}
