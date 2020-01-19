#include "TDirectory.h"


TDirectory::TDirectory(TString& path) : TFileShell(path)
{

}

bool TDirectory::IsDirectory()
{
	return true;
}

TDataArray<TrecPointer<TFileShell>> TDirectory::GetFileListing()
{
	TDataArray<TrecPointer<TFileShell>> ret;

	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	fileBrowser = FindFirstFileExW(path.GetConstantBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
		return ret;

	TString newPath;

	do
	{
		newPath.Set(this->path + data.cFileName);
		ret.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));

	FindClose(fileBrowser);
	fileBrowser = 0;

	return ret;
}

void TDirectory::GetFileListing(TDataArray<TrecPointer<TFileShell>>& files)
{
	files.RemoveAll();
	HANDLE fileBrowser = 0;
	WIN32_FIND_DATAW data;

	fileBrowser = FindFirstFileExW(path.GetConstantBuffer(),
		FindExInfoStandard,
		&data,
		FindExSearchNameMatch,
		nullptr,
		0);

	if (fileBrowser == INVALID_HANDLE_VALUE)
		return;

	TString newPath;

	do
	{
		newPath.Set(this->path + data.cFileName);
		files.push_back(TFileShell::GetFileInfo(newPath));
	} while (FindNextFileW(fileBrowser, &data));

	FindClose(fileBrowser);
	fileBrowser = 0;
}