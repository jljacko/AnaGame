#pragma once
#include <TControl.h>
#include <TFile.h>
#include <TString.h>
#include <TrecPointer.h>

typedef enum ProjectFileType
{
	ft_text,
	ft_anaface,
	ft_model,
	ft_arena
}ProjectFileType;


class ProjectFile
{
public:
	ProjectFile(ProjectFileType pft);
	~ProjectFile();


private:
	TrecPointer<TControl> presentation;
	TFile file;
	ProjectFileType fileType;
};

