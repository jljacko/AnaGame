#include "stdafx.h"
#include "BuilderDocManager.h"


BuilderDocManager::BuilderDocManager(CAnimateBuilderApp* app)
{
	apper = app;
}


BuilderDocManager::~BuilderDocManager()
{
}

void BuilderDocManager::OnFileNew()
{
	int tempType;
	FileNewDialog fnd;
	fnd.givePointer(&tempType);
	if (fnd.DoModal() == IDOK)
	{
		POSITION p = apper->GetFirstDocTemplatePosition();
		CDocTemplate* pTemplate = apper->GetNextDocTemplate(p);
		if (!pTemplate)
		{
			TRACE0("ERROR: Template could not be acquired\n");
		}
		switch (tempType)
		{
		case 1:
			pTemplate = apper->pDoc_Anaface;
			break;
		case 2:
			pTemplate = apper->pDoc_Arena;
			break;
		case 4:
			pTemplate = apper->pDocCAnimate;
			break;
		case 5:
			pTemplate = apper->pDocJAnimate;
			break;
		case 6:
			pTemplate = apper->pDoc_AdaMate;
			break;
		default:
			pTemplate = apper->pDoc_Anaface;

		}

		pTemplate->OpenDocumentFile(NULL);
	}
}
