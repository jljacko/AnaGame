#pragma once
#include "BuilderApp.h"
#include <AnafaceUI.h>

class AGDocument
{
public:
	AGDocument(BuilderApp* app);
	~AGDocument();

private:
	TrecPointer<TControl> mainContent;
	TrecPointer<TControl> classDiagram;
	TrecPointer<TControl> outputPanel;

	TrecPointer<BuilderApp> code;
};

