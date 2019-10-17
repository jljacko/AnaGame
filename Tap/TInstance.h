#pragma once
#include <TObject.h>
#include "TWindow.h"

class WindowContainer
{
public:
	WindowContainer();
	TrecPointer<TWindow> window;
	bool destroy;
	int messageStack;
};

class _TAP_DLL TInstance : public TObject
{
	friend class TWindow;
	friend class TDialog;
public:
	TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp);

	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh);

	TrecPointer<TWindow> GetWindowByName(TString&);

	//bool LaunchDialog(TString& file, TrecPointer<EventHandler> eh, HWND callingWindow);

	WNDPROC GetProc();

	TrecPointer<TWindow> GetMainWindow();

	TrecPointer<TWindow> GetWindow(HWND);

	void LockWindow(HWND win);
	void UnlockWindow(HWND win);

	HINSTANCE GetInstanceHandle();
	TrecComPointer<ID2D1Factory1> GetFactory();
	void SetSelf(TrecPointer<TInstance> i);

protected:
	void AssertDialogRegistered();
	ATOM dialogAtom;

	void RegisterDialog(TrecPointer<TWindow>);

	HINSTANCE instance;
	TString mainWindowName, mainWindowClass;
	UINT mainStyle;
	int command;
	HWND mainWindowHandle;
	TrecPointer<TWindow> mainWindow;
	TrecComPointer<ID2D1Factory1> factory;
	
	TDataArray<TrecPointer<WindowContainer>> windowList;

	void CleanWindows();

	WNDPROC proctor;
	TrecPointerSoft<TInstance> self;

	UINT messageStack;
};

