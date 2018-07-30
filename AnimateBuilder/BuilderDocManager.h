#pragma once
#include "afxwin.h"
#include "FileNewDialog.h"
#include "AnimateBuilder.h"
class BuilderDocManager :	public CDocManager
{
public:
	BuilderDocManager(CAnimateBuilderApp*);
	~BuilderDocManager();
	void OnFileNew() override;
protected:
	CAnimateBuilderApp* apper;
};

