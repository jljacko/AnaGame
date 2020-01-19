#include "TFileShell.h"
#include "TDirectory.h"

bool TFileShell::IsDirectory()
{
	return false;
}

bool TFileShell::IsValid()
{
	return path.GetSize() && !deleted;
}

TString TFileShell::GetPath()
{
	return path;
}

TString TFileShell::GetName()
{
	int slashLoc = path.FindLastOneOf(TString(L"/\\"));
	if (slashLoc == -1)
		return path;
	TString cutPath(path);
	while (slashLoc == cutPath.GetSize() - 1)
	{
		cutPath.Set(cutPath.SubString(0, slashLoc));
		slashLoc = cutPath.FindLastOneOf(TString(L"/\\"));
	}
	return cutPath;
}

TrecPointer<TFileShell> TFileShell::GetFileInfo(TString& path)
{
	DWORD ftyp = GetFileAttributesW(path.GetConstantBuffer());

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return TrecPointer<TFileShell>(); // if Invalid, simply return a Null


	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return TrecPointerKey::GetNewTrecPointerAlt<TFileShell, TDirectory>(path);
	else
		return TrecPointerKey::GetNewTrecPointer<TFileShell>(path);
}

FILETIME TFileShell::GetCreationDate()
{
	Refresh();
	return fileInfo.ftCreationTime;
}

FILETIME TFileShell::GetLastAccessed()
{
	Refresh();
	return fileInfo.ftLastAccessTime;
}

FILETIME TFileShell::GetLastWritten()
{
	Refresh();
	return fileInfo.ftLastWriteTime;
}

ULONG64 TFileShell::GetSize()
{
	Refresh();
	return (static_cast<ULONG64>(fileInfo.nFileSizeHigh) << 32) + static_cast<ULONG64>(fileInfo.nFileSizeLow);
}

bool TFileShell::IsArchive()
{
	Refresh();
	return fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

bool TFileShell::IsEncrypted()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED;
}

bool TFileShell::IsHidden()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
}

bool TFileShell::IsReadOnly()
{
	Refresh();
	return  fileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
}

TFileShell::TFileShell(TString& path)
{
	if (GetFileAttributesExW(path.GetConstantBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
		this->path.Set(path);

	if (this->path.GetSize() && (this->path[this->path.GetSize() - 1] != L'\\' && this->path[this->path.GetSize() - 1] != L'/'))
		this->path.AppendChar(L'\\');

	deleted = false;
}

void TFileShell::Refresh()
{
	if (!GetFileAttributesExW(path.GetConstantBuffer(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &this->fileInfo))
	{
		if (path.GetSize())
			deleted = true;
	}
}