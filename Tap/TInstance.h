#pragma once
#include <TObject.h>
#include "TWindow.h"



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
	TDataArray<TrecPointer<TWindow>> windowList;
	WNDPROC proctor;
	TrecPointerSoft<TInstance> self;
};

