#include "TStoryBoard.h"
#include "TWindow.h"

DWORD __stdcall ProcessAnimations(LPVOID param)
{
	ThreadData* data = reinterpret_cast<ThreadData*>(param);
	if (!data)
		return 1;

	float progress = 0.0f;

	do
	{
		if (progress >= 1.0f)
			progress = 0.0f;
		for (UINT c = 0; c < data->animations.Size(); c++)
			if(data->animations[c].Get())
				data->animations[c]->Update(progress);

		if (data->win.Get())
			data->win->InduceDraw();

		progress += (static_cast<float>(data->shortestInterval) / static_cast<float>(data->longestAni));

		// To-do Add Sleep
		Sleep(data->shortestInterval);
		
	} while (data->win.Get() && (data->persist ? true : progress < 0.999f));

	data->animationThread = 0;
	return 0;
}

TStoryBoard::TStoryBoard()
{
	threadData.animationThread = 0;
	threadData.longestAni = 0;
	threadData.persist = false;
	threadData.shortestInterval = 60000;
}

TStoryBoard::~TStoryBoard()
{
	Terminate();
}

bool TStoryBoard::AddAnimation(TrecPointer<Animation> a)
{
	if (!a.Get())
		return false;

	// Don't add an animation if an animation thread is underway
	if (threadData.animationThread)
		return false;

	if (threadData.longestAni < a->GetMillisecondLength())
		threadData.longestAni = a->GetMillisecondLength();

	if (threadData.shortestInterval > a->GetMillisecondRefresh())
		threadData.shortestInterval = a->GetMillisecondRefresh();

	for (UINT c = 0; c < threadData.animations.Size(); c++)
	{
		threadData.animations[c]->SetMillisecindStoryLength(threadData.longestAni);
	}

	threadData.animations.push_back(a);
	return true;
}

void TStoryBoard::SetWindow(TrecPointer<TWindow> w)
{
	if (w.Get())
		threadData.win = w;
}

void TStoryBoard::SetPersistant()
{
	threadData.persist = true;
}

bool TStoryBoard::Run()
{
	if (threadData.animationThread)
		return false;

	threadData.animationThread = CreateThread(nullptr,
		0,
		ProcessAnimations,
		&threadData,
		0,
		&threadID);

	return threadData.animationThread != 0;
}

bool TStoryBoard::Pause()
{
	if (threadData.animationThread)
	{
		SuspendThread(threadData.animationThread);
		return true;
	}
	return false;
}

void TStoryBoard::Resume()
{
	if (threadData.animationThread)
		ResumeThread(threadData.animationThread);
}


void TStoryBoard::Terminate()
{
	// TO-DO: Research safer way to terminate thread

	if (threadData.animationThread)
		TerminateThread(threadData.animationThread, 0);
	threadData.animationThread = 0;
}

void TStoryBoard::Empty()
{
	threadData.animations.RemoveAll();
}
