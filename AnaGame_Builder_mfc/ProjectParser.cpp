#include "stdafx.h"
#include "ProjectParser.h"


ProjectParser::ProjectParser()
{
}


ProjectParser::~ProjectParser()
{
}

bool ProjectParser::Obj(TString * v)
{
	if(!v)
		return false;

	if (!v->Compare(L"Solution"))
	{
		if (solution.get()) // One solution per file
			return false;
		solution = new Solution();
		ps = P_S_SOL;
		return true;
	}
	if (!v->Compare(L"Project"))
	{
		if (!solution.get())
			solution = new Solution();
		currentProject = new Project(0);
		solution->AddProject(currentProject);
	}
}

bool ProjectParser::Attribute(TrecPointer<TString> v, TString & e)
{
	if(!v.get())
		return false;

	if (!e.Compare(L"|Source"))
	{
		TString file = L"Source";
		file += L"\\";
		file += *v.get();
		TrecPointer<TFile> newFile;
		//newFile = new TFile()
	}
	else if (!e.Compare(L"|Header"))
	{
		TString file = L"Header";
		file += L"\\";
		file += *v.get();
		TrecPointer<TFile> newFile;
		//newFile = new TFile()
	}
	else if (!e.Compare(L"|Resource"))
	{
		TString file = L"Header";
		file += L"\\";
		file += *v.get();
		TrecPointer<TFile> newFile;
		//newFile = new TFile()
	}
	else if (!e.Compare(L"|ExternalFile"))
	{
		TrecPointer<TFile> newFile;
		newFile = new TFile(*v.get(), CFile::modeRead | CFile::modeWrite);
	}
	else if (!e.Compare(L"|ExternalFolder"))
	{

	}
	else if (!e.Compare(L"|"))
	{

	}
	else if (!e.Compare(L"|"))
	{

	}
	else if (!e.Compare(L"|"))
	{

	}
	else if (!e.Compare(L"|"))
	{

	}
}

bool ProjectParser::submitType(TString v)
{
	return false;
}

bool ProjectParser::submitEdition(TString v)
{
	return false;
}
