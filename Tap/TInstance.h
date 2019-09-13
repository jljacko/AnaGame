#pragma once
#include <TObject.h>
#include "TWindow.h"



class _TAP_DLL TInstance : public TObject
{
public:
	TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp);

	LRESULT Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh);

	TrecPointer<TWindow> GetWindowByName(TString&);

	bool LaunchDialog(TString& file, TrecPointer<EventHandler> eh, HWND callingWindow);

	TrecPointer<TWindow> GetMainWindow();

protected:
	HINSTANCE instance;
	TString mainWindowName, mainWindowClass;
	UINT mainStyle;
	int command;
	HWND mainWindowHandle;
	TrecPointer<TWindow> mainWindow;
	TrecComPointer<ID2D1Factory1> factory;
	TDataArray<TrecPointer<TWindow>> windowList;
	WNDPROC proctor;
	ATOM dialogAtom;
};

