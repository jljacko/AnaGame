#pragma once
#include "TObject.h"
#include "TString.h"
class _TREC_LIB_DLL TFileShell :
	public TObject
{
	friend class TrecPointerKey;
public:
	virtual bool IsDirectory();
	bool IsValid();
	TString GetPath();
	TString GetName();

	TrecPointer<TFileShell> static GetFileInfo(TString& path);

	FILETIME GetCreationDate();
	FILETIME GetLastAccessed();
	FILETIME GetLastWritten();

	ULONG64 GetSize();

	bool IsArchive();
	bool IsEncrypted();
	bool IsHidden();
	bool IsReadOnly();

protected:
	TFileShell(TString& path);

	void Refresh();

	bool deleted;

	TString path;
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
};

