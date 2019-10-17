#pragma once
#include <TFile.h>
#include <TArray.h>
#include <TString.h>

#define PROJECT_UNDETERMINED  0 // Use if type hasn't been set (or is a Solution)
#define PROJECT_ANAGAME_VIDEO 1 // Ideal For Video Editing
#define PROJECT_ANAGAME_TAP   2 // Ideal for writing Basic Taps for Anagame
#define PROJECT_PROG_LANG     3 // Ideal for developing your own programming langauge
#define PROJECT_FRONT_END     4 // Ideal for developing a basic website
#define PROJECT_BACK_END      5 // Ideal for developing a back-end web-service
#define PROJECT_LIBRARY       6 // Ideal for developing a library that will be used by other projects
#define PROJECT_APPLICATION   7 // Ideal for developing a regular application
#define PROJECT_ANAGAME_EXT   8 // Ideal for extending Anagame functionality
#define PROJECT_MOBILE        9 // Ideal for targeting mobile platforms

class Project
{
	friend class ProjectParser;
	friend class NewProjectDialogAG;
public:

	Project(UINT type);
	~Project();
	TrecPointer<TFile> GetFile(UINT c);
	
	virtual void SaveFile(TFile& file);
	TString GetName();
protected:
	void SetName(TString& t);

	TString name;
	TArray<TFile> files;
private:
	UINT type;
};

