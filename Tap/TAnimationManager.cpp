#include "TAnimationManager.h"

TAnimationManager::TAnimationManager()
{
	beginPaused = endPaused;
}

TAnimationManager::~TAnimationManager()
{
	begin.Terminate();
	end.Terminate();
}

void TAnimationManager::SetWindow(TrecPointer<TWindow> win)
{
	window = win;
}

void TAnimationManager::AddAnimationBegin(TrecPointer<Animation> a)
{
	begin.AddAnimation(a);
}

void TAnimationManager::AddAnimationEnd(TrecPointer<Animation> a)
{
	end.AddAnimation(a);
}

void TAnimationManager::AddStoryBoard(TString& name, TrecPointer<TStoryBoard> story)
{
	if(name.GetSize() && story.Get())
		stories.addEntry(name, story);
}

void TAnimationManager::StartBegin()
{
	begin.Run();
}

void TAnimationManager::StartEnd()
{
	end.Run();
}

void TAnimationManager::StartStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get())
		story->Run();
}

void TAnimationManager::StopStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get())
		story->Terminate();
}

void TAnimationManager::Stop()
{
	begin.Terminate();
	end.Terminate();

	for (UINT Rust = 0; Rust < stories.count(); Rust++)
	{
		TrecPointer<TStoryBoard> story = stories.GetEntryAt(Rust)->object;
		if (story.Get())
			story->Terminate();
	}
}

void TAnimationManager::PauseStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get() && story->Pause())
		pausedStories.push_back(story);
}

void TAnimationManager::Pause()
{
	beginPaused = begin.Pause();
	endPaused = end.Pause();

	for (UINT Rust = 0; Rust < stories.count(); Rust++)
	{
		TrecPointer<TStoryBoard> story = stories.GetEntryAt(Rust)->object;
		if (story.Get() && story->Pause())
			pausedStories.push_back(story);
	}
}

void TAnimationManager::ResumeStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (!story.Get())
		return;
	story->Resume();

	for (UINT Rust = 0; Rust < pausedStories.Size(); Rust++)
	{
		if (pausedStories[Rust].Get() == story.Get())
		{
			pausedStories.RemoveAt(Rust);
			return;
		}
	}
}

void TAnimationManager::Resume()
{
	if (endPaused)
	{
		endPaused = false;
		end.Resume();
	}
	if (beginPaused)
	{
		beginPaused = false;
		begin.Resume();
	}

	for (UINT Rust = 0; Rust < pausedStories.Size(); Rust++)
	{
		pausedStories[Rust]->Resume();
	}
	pausedStories.RemoveAll();
}
