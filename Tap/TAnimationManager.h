#pragma once
#include <TObject.h>


#include "TStoryBoard.h"
#include <TMap.h>
class TWindow;

class TAnimationManager :public TObject
{
public:
	TAnimationManager();
	~TAnimationManager();

	void SetWindow(TrecPointer<TWindow> win);

	void AddAnimationBegin(TrecPointer<Animation> a);
	void AddAnimationEnd(TrecPointer<Animation> a);

	void AddStoryBoard(TString& name, TrecPointer<TStoryBoard> stpry);
	


	void StartBegin();
	void StartEnd();
	void StartStory(TString& name);

	void StopStory(TString& name);
	void Stop();

	void PauseStory(TString& name);
	void Pause();

	void ResumeStory(TString& name);
	void Resume();
protected:
	TStoryBoard begin, end;
	bool beginPaused, endPaused;

	TMap<TStoryBoard> stories;
	TTrecPointerArray<TStoryBoard> pausedStories;
	TrecPointer<TWindow> window;
};

