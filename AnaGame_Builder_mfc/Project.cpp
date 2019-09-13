#include "stdafx.h"
#include "Project.h"


Project::Project(UINT type)
{
	this->type = type;
}


Project::~Project()
{
}

TrecPointer<TFile> Project::GetFile(UINT c)
{
	if (c < files.Count())
		return files.ElementAt(c);
	return TrecPointer<TFile>();
}

void Project::SaveFile(TFile & file)
{
	if (!file.IsOpen())
		return;
	file.WriteString(L"->Project\n-|Type:");

	switch (type)
	{
	case PROJECT_UNDETERMINED:
		file.WriteString(L"Undetermined\n");
		break;
	case PROJECT_ANAGAME_VIDEO:
		file.WriteString(L"Video\n");
		break;
	case PROJECT_ANAGAME_TAP:
		file.WriteString(L"TAP\n");
		break;
	case PROJECT_PROG_LANG:
		file.WriteString(L"Programming Language\n");
		break;
	case PROJECT_FRONT_END:
		file.WriteString(L"Front End\n");
		break;
	case PROJECT_BACK_END:
		file.WriteString(L"Back End\n");
		break;
	case PROJECT_LIBRARY:
		file.WriteString(L"Library\n");
		break;
	case PROJECT_APPLICATION:
		file.WriteString(L"Application\n");
		break;
	case PROJECT_ANAGAME_EXT:
		file.WriteString(L"Anagame Extension\n");
		break;
	}
	
	for (UINT c = 0; c < files.Count(); c++)
	{
		TrecPointer<TFile> file2 = files.ElementAt(c);
		//if (!file2IsOpen())
		//	continue;
	}
}

TString Project::GetName()
{
	return name;
}

void Project::SetName(TString & t)
{
	name = t;
}
