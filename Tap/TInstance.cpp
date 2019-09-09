#include "TInstance.h"
#include <windowsx.h>
#include "TDialog.h"


static TString dialogClassName(L"TDialog");

TInstance::TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp)
{
	if (!wp)
		throw L"ERROR! Window Proc Function must be a valid pointer!";
	proctor = wp;
	instance = ins;
	command = commandShow;
	mainWindowClass.Set(winClass);
	mainWindowName.Set(name);
	mainStyle = style;

	TrecComPointer<ID2D1Factory1>::TrecComHolder factoryHolder;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, factoryHolder.GetPointerAddress());
	factory = factoryHolder.Extract();
	dialogAtom = 0;
}

LRESULT TInstance::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TWindow* win = nullptr;

	int windowIndex = -1;

	if (mainWindow.Get() && mainWindow->GetWindowHandle() == hWnd)
		win = mainWindow.Get();
	else
	{
		for (UINT c = 0; c < windowList.Size(); c++)
		{
			if (windowList[c].Get() && windowList[c]->GetWindowHandle() == hWnd)
			{
				win = windowList[c].Get();
				windowIndex = c;
				break;
			}
		}
	}

	if(!win)
		return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_PAINT:
		win->Draw();
		break;
	case WM_DESTROY:
		if(win->OnDestroy())
		{
			if(windowIndex == -1)
			{
				mainWindow.Delete();
				for(UINT c = 0; c < windowList.Size(); c++)
				{
					windowList[c].Delete();
				}

				PostQuitMessage(0);
			}
			else
			{
				windowList[windowIndex].Delete();
				windowList.RemoveAt(windowIndex);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		win->OnRButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_MBUTTONDOWN:
		win->OnLButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_MBUTTONDBLCLK:
		win->OnLButtonDblClk(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_MBUTTONUP:
		win->OnLButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_RBUTTONUP:
		win->OnRButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_NCMOUSEMOVE:
		win->OnMouseMove(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_CHAR:
		win->OnChar(true, wParam, lParam & 0x0000FFFF, 0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int TInstance::SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh)
{
	WORD regResult = RegisterClassExW(&wcex);
	if (!regResult)
	{
		int err = GetLastError();
		return 1;
	}
	mainWindow = TrecPointerKey::GetNewTrecPointer<TWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command, instance);

	mainWindow->PrepareWindow();
	return mainWindow->CompileView(file, factory, eh);
}

TrecPointer<TWindow> TInstance::GetWindowByName(TString& name)
{
	if(mainWindow.Get() && !mainWindow->GetWinName().Compare(name))
		return mainWindow;
	for(UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if(windowList[Rust].Get() && !windowList[Rust]->GetWinName().Compare(name))
			return windowList[Rust];
	}
	return TrecPointer<TWindow>();
}

bool TInstance::LaunchDialog(TString& file, TrecPointer<EventHandler> eh, HWND callingWindow)
{
	WNDCLASSEXW windowClassStructure;
	ZeroMemory(&windowClassStructure, sizeof(windowClassStructure));

	if (!GetClassInfoExW(instance, dialogClassName.GetConstantBuffer(), &windowClassStructure))
	{
		windowClassStructure.cbSize = sizeof(windowClassStructure);
		windowClassStructure.style = WS_CAPTION | WS_DLGFRAME | WS_POPUPWINDOW;
		windowClassStructure.lpfnWndProc = proctor;
		windowClassStructure.cbClsExtra = 0;
		windowClassStructure.cbWndExtra = 0;
		windowClassStructure.hInstance = instance;
		windowClassStructure.hIcon = nullptr;
		windowClassStructure.hCursor = nullptr;
		windowClassStructure.hbrBackground = nullptr;
		windowClassStructure.lpszMenuName = nullptr;
		windowClassStructure.lpszClassName = dialogClassName.GetConstantBuffer();
		windowClassStructure.hIconSm = nullptr;


		dialogAtom = RegisterClassExW(&windowClassStructure);

		assert(dialogAtom);
	}
	
	TrecPointer<TWindow> newDialog = TrecPointerKey::GetNewTrecPointerAlt<TWindow, TDialog>(mainWindowName, dialogClassName, mainStyle, mainWindowHandle, command, instance);

	newDialog->PrepareWindow();

	if (newDialog->CompileView(file, factory, eh))
		return false;
	windowList.push_back(newDialog);
	return true;
}
