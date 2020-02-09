#pragma once
#include "TFileShell.h"

/*
 * class TDirectory
 * Purpose: Represents a Folder in the file system
 */
class _TREC_LIB_DLL TDirectory :
	public TFileShell
{
	friend class TrecPointerKey;
protected:
	TDirectory(const TString& path);
public:
	bool IsDirectory()override;

	TDataArray<TrecPointer<TFileShell>> GetFileListing();
	void GetFileListing(TDataArray<TrecPointer<TFileShell>>& files);
};

