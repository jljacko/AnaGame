#include "stdafx.h"
#include "Solution.h"


Solution::Solution():Project(0)
{
}


Solution::~Solution()
{
}

TrecPointer<Project> Solution::GetProject(UINT c)
{
	if (c < projects.Count())
		return projects.ElementAt(c);
	return TrecPointer<Project>();
}

TString Solution::GetName()
{
	return name;
}

void Solution::SaveFile(TFile & file)
{
	if (!file.IsOpen())
		return;
	file.WriteString(L"->TML\n-|Type:AnaGame_Solution\n-|Version:0.1\n-/\n\n");

	file.WriteString(L"->Solution\n-|name:");
	file.WriteString(name);
	file.WriteString(L"\n");

	for (UINT c = 0; c < projects.Count(); c++)
	{
		TrecPointer<Project> proj = projects.ElementAt(c);
		if (!proj.get())
			continue;
		file.WriteString(L"-->Project\n--|name:");
		TString projName = proj->GetName();
		file.WriteString(projName);
		
		TString directory = file.GetFileDirectory();
		directory += projName;
		DWORD fileAtt = GetFileAttributesW(directory);
		if (fileAtt == INVALID_FILE_ATTRIBUTES)
		{
			int er = GetLastError();
			if (er == ERROR_PATH_NOT_FOUND || er == ERROR_FILE_NOT_FOUND
				|| er == ERROR_INVALID_NAME || er == ERROR_BAD_NETPATH)
			{
				CreateDirectory(directory, nullptr);
			}
			else
			{
				// To-Do: Handle error
			}
		}

		directory += L"\\";
		directory += projName;
		directory += L".agproj";
		TFile pFile(directory, CFile::modeReadWrite | CFile::modeCreate | CFile::shareDenyNone);
		if (!pFile.IsOpen())
			continue;
		proj->SaveFile(pFile);
		pFile.Close();
	}
}

void Solution::AddProject(TrecPointer<Project> p)
{
	if (p.get())
		projects.Add(p);
}

void Solution::SetName(TString & t)
{
	name = t;
}
