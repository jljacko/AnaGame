#pragma once
#include <Parser_.h>
#include "Solution.h"

#define PROJ_SOL bool
#define P_S_PROJ true
#define P_S_SOL false

#define IS_PROJECT(x)   if(x)
#define IS_SOLUTION(X)  if(!x)

class ProjectParser
{
public:
	ProjectParser();
	~ProjectParser();

	// for the initial object type
	virtual bool Obj(TString *v);
	// for the attribute name
	
	virtual bool Attribute(TrecPointer<TString> v, TString& e);
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v);
	virtual bool submitEdition(TString v);

protected:
	TrecPointer<Solution> solution;
	TrecPointer<Project> currentProject;
	PROJ_SOL ps;
};

