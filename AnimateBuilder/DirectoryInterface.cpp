#include "stdafx.h"
#include "DirectoryInterface.h"


DirectoryInterface::DirectoryInterface(REFKNOWNFOLDERID ri)
{
	PWSTR path = nullptr;
	HRESULT hr = SHGetKnownFolderPath(ri, 0, NULL, &path);
	if (SUCCEEDED(hr))
	{
		folderString = path;
	}
}


DirectoryInterface::~DirectoryInterface()
{
}

TString DirectoryInterface::getFolderString()
{
	if (!folderString)
		return TString();
	WCHAR* returnable_ = new WCHAR[folderString.GetLength() + 1];
	wcsncpy_s(returnable_, wcslen(folderString) + 1, folderString, wcslen(folderString));
	TString returnable = returnable_;
	delete[] returnable_;
	return returnable;
}

bool DirectoryInterface::AppendFolderString(TString& folder)
{
	if(!folderString || !folder)
		return false;

	folderString += L"\\";
	folderString += folder;
	return true;
	
}
