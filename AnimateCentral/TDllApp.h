#pragma once
#include "TApp.h"

typedef UINT(*DRAW)();
typedef UINT(*MESSAGE)(CPoint&, UINT, WCHAR, UINT);

class TDllApp :	public TApp
{
public:
	TDllApp();
	~TDllApp();
	bool Initialize(TMap<TString>& properties, TString& directory);

private:
	HMODULE library;
	void ReleaseLibrary();

	DRAW onDraw;
	MESSAGE onMessage;
};

