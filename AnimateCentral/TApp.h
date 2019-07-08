#pragma once
#include <TObject.h>
#include <TFile.h>
#include <TMap.h>

class TApp :
	public TObject
{
public:
	TApp();
	~TApp();

	static TApp* Initialize(TFile& file);

	virtual bool Initialize(TMap<TString>& properties, TString& directory) = 0;
};

