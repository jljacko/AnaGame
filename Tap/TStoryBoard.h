#pragma once
#include "TObject.h"
#include <Animation.h>
#include <TTrecPointerArray.h>


class TWindow;

class ThreadData
{
public:
	TTrecPointerArray<Animation> animations;
	bool persist;
	UINT shortestInterval, longestAni;
	TrecPointer<TWindow> win;
	HANDLE animationThread;
	
};

class TStoryBoard :
	public TObject
{
public:
	TStoryBoard();
	~TStoryBoard();
	bool AddAnimation(TrecPointer<Animation> a);
	void SetWindow(TrecPointer<TWindow> w);

	void SetPersistant();

	bool Run();
	bool Pause();
	void Resume();
	void Terminate();

protected:
	ThreadData threadData;
	DWORD threadID;

};

DWORD WINAPI ProcessAnimations(LPVOID param);