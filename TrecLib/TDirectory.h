#pragma once
#include "TFileShell.h"
class _TREC_LIB_DLL TDirectory :
	public TFileShell
{
	friend class TrecPointerKey;
protected:
	TDirectory(TString& path);
public:
	bool IsDirectory()override;

	TDataArray<TrecPointer<TFileShell>> GetFileListing();
	void GetFileListing(TDataArray<TrecPointer<TFileShell>>& files);
};

