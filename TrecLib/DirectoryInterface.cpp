#include "stdafx.h"
#include "DirectoryInterface.h"
//#include "TString.h"


static bool initialized = false;

static TString directories[9];
static TString shadowDirectories[9];

void ForgeDirectory(TString& dir)
{
	TrecPointer<TArray<TString>> pieces = dir.split(TString(L"/\\"));
	TString bDir;
	for (UINT rust = 0; rust < pieces->Count(); rust++)
	{
		bDir += *pieces->ElementAt(rust).get() + L'\\';
		CreateDirectoryW(bDir, 0);
	}
}

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

	tempString.Set(filepath);
	directories[0].Set(tempString);

	PWSTR folderString = nullptr;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[1].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[2].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[3].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Music, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[4].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[5].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Videos, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[6].Set(tempString);

	SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &folderString);
	tempString.Set(folderString);
	directories[7].Set(tempString);

	directories[8].Set(directories[7]);
	directories[8].Replace(TString(L"\\Downloads"), L"");

	initialized = true;
	
	TString baseShadow = GetDirectoryWithSlash(cd_AppData) + TString(L"AnaGame\\ShadowFiles");
	ForgeDirectory(baseShadow);

	initialized = false;

	shadowDirectories[0].Set(baseShadow + TString(L"\\AnaGameExe"));
	shadowDirectories[1].Set(baseShadow + TString(L"\\AppData"));
	shadowDirectories[2].Set(baseShadow + TString(L"\\Desktop"));
	shadowDirectories[3].Set(baseShadow + TString(L"\\Documents"));
	shadowDirectories[4].Set(baseShadow + TString(L"\\Music"));
	shadowDirectories[5].Set(baseShadow + TString(L"\\Pictures"));
	shadowDirectories[6].Set(baseShadow + TString(L"\\Videos"));
	shadowDirectories[7].Set(baseShadow + TString(L"\\Downloads"));
	shadowDirectories[8].Set(baseShadow);

	for (UINT c = 0; c < 9; c++)
	{
		CreateDirectoryW(shadowDirectories[c], 0);
	}

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

/*
* Function: GetShadowDirectory
* Purpose: Gets AnaGame's "shadow" version of the specified directory
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString GetShadowDirectory(CentralDirectories cd)
{
	if (!initialized)
		InitializeDirectories();
	return shadowDirectories[static_cast<UINT>(cd)];
}

/*
* Function: GetShadowDirectoryWithSlash
* Purpose: Gets AnaGame's "shadow" version of the specified directory
* Parameters: CentralDirectories cd - the Directory type being sought
* Returns: TString - the Directory in the computer being requested
*/
TString GetShadowDirectoryWithSlash(CentralDirectories cd)
{
	TString returnable = GetShadowDirectory(cd);
	returnable += L"\\";
	return returnable;
}

/*
* Function: GetShadowFilePath
* Purpose: Sets up the directory and returns the path for the Shadow version of the provided file
* Parameters: TFile& f - the file to get s shadow File's path for
* Returns: TString - the Path of the intended Shadow file
* Note: The provided File has to be open AND it has to be found in an AnaGame approved directory. Otherwise, an empty string is returned
*/
TString GetShadowFilePath(TFile& f)
{
	if (!f.IsOpen())
		return TString();

	UINT dirNum = 0;
	TString returnable = f.GetFilePath();
	while (dirNum < 9)
	{
		if (returnable.Find(directories[dirNum]) != -1)
		{
			returnable.Replace(directories[dirNum], TString(L""));
			break;
		}
		dirNum++;
	}

	if (dirNum > 8)
		return TString();

	return shadowDirectories[dirNum] + TString(L"\\") + returnable;
}