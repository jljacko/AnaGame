#pragma once
#include "TObject.h"
#include "TString.h"

/*
 * class TFileShell
 * Purpose: Represents a File that exists on the file System.
 *
 * Note: this class does not open a file for reading or writing, merely provides information about the file in question
 */
class _TREC_LIB_DLL TFileShell :
	public TObject
{
	friend class TrecPointerKey;
public:
	virtual bool IsDirectory();
	bool IsValid();
	TString GetPath();
	TString GetName();

	TrecPointer<TFileShell> static GetFileInfo(const TString& path);

	FILETIME GetCreationDate();
	FILETIME GetLastAccessed();
	FILETIME GetLastWritten();

	ULONG64 GetSize();

	bool IsArchive();
	bool IsEncrypted();
	bool IsHidden();
	bool IsReadOnly();

protected:
	TFileShell(const TString& path);

	void Refresh();

	bool deleted;

	TString path;
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
};

