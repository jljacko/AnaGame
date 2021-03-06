#pragma once
#include "TObject.h"
#include <Animation.h>
#include <TTrecPointerArray.h>
#include "Tap_dll.h"

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

class _TAP_DLL TStoryBoard :
	public TObject
{
	friend class TAnimationManager;
public:
	TStoryBoard();
	~TStoryBoard();
	bool AddAnimation(TrecPointer<Animation> a);
	void SetWindow(TrecPointer<TWindow> w);

	

	void SetPersistant();
	bool IsPersistant();

	bool Run();
	bool Pause();
	void Resume();
	void Terminate();

protected:
	ThreadData threadData;
	DWORD threadID;
	void Empty();
};

DWORD WINAPI ProcessAnimations(LPVOID param);