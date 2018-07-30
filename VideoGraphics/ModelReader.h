#pragma once
#include "ArenaModel.h"
#include <TFile.h>
#include <TrecPointer.h>

class ModelReader : public TObject
{
public:
	ModelReader(TrecPointer<TFile> file, TrecPointer<ArenaEngine> engine);
	~ModelReader();
	virtual UINT ReadFile();

	TrecPointer<ArenaModel> GetArenaModel(UINT c);
	UINT GetArenaModelCount();
	virtual UCHAR* GetAnaGameType()override;

protected:
	TArray<ArenaModel> models;
	TrecPointer<TFile> fileData;
	TrecPointer<ArenaEngine> targetEngine;
};

