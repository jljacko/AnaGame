#pragma once
#include "Project.h"
class Solution : public Project
{
	friend class ProjectParser;
	friend class NewProjectDialogAG;
public:
	Solution();
	~Solution();

	TrecPointer<Project> GetProject(UINT c);
	TString GetName();
	void SaveFile(TFile & file)override;

private:
	void AddProject(TrecPointer<Project> p);
	void SetName(TString& t);

	TString name;
	TArray<Project> projects;
};

