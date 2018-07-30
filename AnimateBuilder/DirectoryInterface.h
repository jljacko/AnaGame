#pragma once
#include <TString.h>
/* Class writeen to help the builder work with special directories,
 such as the local appData directory, the Documents directory, the video
 directory, etc. There is a special function in the Windows API called 

 HRESULT SHGetKnownFolderPath(
  _In_     REFKNOWNFOLDERID rfid,
  _In_     DWORD            dwFlags,
  _In_opt_ HANDLE           hToken,
  _Out_    PWSTR            *ppszPath
);

found at https://msdn.microsoft.com/en-us/library/bb762188(VS.85).aspx

also, rfid can be found here: https://msdn.microsoft.com/en-us/library/bb762494(v=vs.85).aspx
*/
class DirectoryInterface
{
public:
	DirectoryInterface(REFKNOWNFOLDERID);
	~DirectoryInterface();

	TString getFolderString();
	bool AppendFolderString(TString& folder);

private:
	TString folderString;
};

