#include "TInstance.h"
#include <windowsx.h>
#include "TDialog.h"
#include "TIdeWindow.h"

// Various Built-in handlers Anagame offers
#include "TCodeHandler.h"
#include "TerminalHandler.h"

static TString dialogClassName(L"TDialog");

TInstance::TInstance(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, HINSTANCE ins, WNDPROC wp)
{
	handlerID = 1;
	messageStack = 0;
	if (!wp)
		throw L"ERROR! Window Proc Function must be a valid pointer!";
	proctor = wp;
	instance = ins;
	command = commandShow;
	mainWindowClass.Set(winClass);
	mainWindowName.Set(name);
	mainStyle = style;

	TrecComPointer<ID2D1Factory1>::TrecComHolder factoryHolder;

	D2D1_FACTORY_OPTIONS d2dDebugLevel = { D2D1_DEBUG_LEVEL_ERROR };

	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, d2dDebugLevel, factoryHolder.GetPointerAddress());
	factory = factoryHolder.Extract();
	dialogAtom = 0;
}

LRESULT TInstance::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TWindow* win = nullptr;
	if (message == WM_LBUTTONDOWN)
		int e = 4;
	int windowIndex = -1;

	if (mainWindow.Get() && mainWindow->GetWindowHandle() == hWnd)
		win = mainWindow.Get();
	else
	{
		for (UINT c = 0; c < windowList.Size(); c++)
		{
			if (windowList[c].Get() && windowList[c]->window.Get()->GetWindowHandle() == hWnd)
			{
				win = windowList[c]->window.Get();
				windowIndex = c;
				windowList[c]->messageStack++;
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
				if (windowList[windowIndex].Get())
				{
					windowList[windowIndex]->destroy = true;
					windowList[windowIndex]->messageStack--;
				}
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		win->OnRButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDOWN:
		win->OnLButtonDown(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONDBLCLK:
		win->OnLButtonDblClk(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_LBUTTONUP:
		win->OnLButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_RBUTTONUP:
		win->OnRButtonUp(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_NCMOUSEMOVE:
	case WM_MOUSEMOVE:
		win->OnMouseMove(wParam, TPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;
	case WM_CHAR:
		win->OnChar(true, wParam, lParam & 0x0000FFFF, 0);
		break;
	case WM_KEYDOWN:
		win->OnChar(false, wParam, lParam & 0x0000FFFF, 0);
		break;
	case WM_SIZE:
		win->OnWindowResize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}

	if (windowIndex != -1 && windowIndex < windowList.Size() && windowList[windowIndex].Get())
	{
		windowList[windowIndex]->messageStack--;
	}
	CleanWindows();

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int TInstance::SetMainWindow(WNDCLASSEXW& wcex, TString& file, TrecPointer<EventHandler> eh, t_window_type winType)
{
	WORD regResult = RegisterClassExW(&wcex);
	if (!regResult)
	{
		int err = GetLastError();
		return 1;
	}
	switch (winType)
	{
	case t_window_type::t_window_type_ide:
		mainWindow = TrecPointerKey::GetNewSelfTrecPointerAlt<TWindow, TIdeWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command,
			TrecPointerKey::GetTrecPointerFromSoft(self), 150, 30);
		break;
	case t_window_type::t_window_type_plain:
		mainWindow = TrecPointerKey::GetNewSelfTrecPointer<TWindow>(mainWindowName, mainWindowClass, mainStyle, mainWindowHandle, command, 
			TrecPointerKey::GetTrecPointerFromSoft(self));
	}
	

	mainWindow->PrepareWindow();
	return mainWindow->CompileView(file, eh);
}

TrecPointer<TWindow> TInstance::GetWindowByName(TString& name)
{
	if(mainWindow.Get() && !mainWindow->GetWinName().Compare(name))
		return mainWindow;
	for(UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if(windowList[Rust].Get() && windowList[Rust]->window.Get() && !windowList[Rust]->window->GetWinName().Compare(name))
			return windowList[Rust]->window;
	}
	return TrecPointer<TWindow>();
}

void TInstance::AssertDialogRegistered()
{
	if (dialogAtom)
		return;
	WNDCLASSEXW windowClassStructure;
	ZeroMemory(&windowClassStructure, sizeof(windowClassStructure));

	TString winClass(L"Dialog");

	windowClassStructure.cbSize = sizeof(windowClassStructure);
	windowClassStructure.style = CS_HREDRAW | CS_VREDRAW;
	windowClassStructure.lpfnWndProc = proctor;
	windowClassStructure.cbClsExtra = 0;
	windowClassStructure.cbWndExtra = 0;
	windowClassStructure.hInstance = instance;
	windowClassStructure.hIcon = nullptr;
	windowClassStructure.hCursor = nullptr;
	windowClassStructure.hbrBackground = nullptr;
	windowClassStructure.lpszMenuName = nullptr;
	windowClassStructure.lpszClassName = winClass.GetConstantBuffer();
	windowClassStructure.hIconSm = nullptr;

	dialogAtom = RegisterClassExW(&windowClassStructure);
	if (!dialogAtom)
	{
		int err = GetLastError();
		throw err;
	}
}

/*bool TInstance::LaunchDialog(TString& file, TrecPointer<EventHandler> eh, HWND callingWindow)
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
}*/

WNDPROC TInstance::GetProc()
{
	return proctor;
}


TrecPointer<TWindow> TInstance::GetMainWindow()
{
	return mainWindow;
}

TrecPointer<TWindow> TInstance::GetWindow(HWND h)
{
	if (h == mainWindow->GetWindowHandle())
		return mainWindow;

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == h)
			return windowList[Rust]->window;
	}
	return TrecPointer<TWindow>();
}

void TInstance::LockWindow(HWND win)
{
	if (mainWindow.Get() && mainWindow->GetWindowHandle() == win)
	{
		mainWindow->LockWindow();
		return;
	}

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == win)
		{
			windowList[Rust]->window->LockWindow();
			return;
		}
	}
}

void TInstance::UnlockWindow(HWND win)
{
	if (mainWindow.Get() && mainWindow->GetWindowHandle() == win)
	{
		mainWindow->UnlockWindow();
		return;
	}

	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->window.Get() && windowList[Rust]->window->GetWindowHandle() == win)
		{
			windowList[Rust]->window->UnlockWindow();
			return;
		}
	}
}

HINSTANCE TInstance::GetInstanceHandle()
{
	return instance;
}

TrecComPointer<ID2D1Factory1> TInstance::GetFactory()
{
	return factory;
}

void TInstance::SetSelf(TrecPointer<TInstance> i)
{
	if (this != i.Get() || i.Get() != this)
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TInstance>(i);

	AssertDialogRegistered();
}

void TInstance::DispatchAnagameMessage(TrecPointer<HandlerMessage> message)
{
	CleanHandlerList();
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (registeredHandlers[Rust].Get() && registeredHandlers[Rust]->ShouldProcessMessage(message))
			registeredHandlers[Rust]->ProcessMessage(message);
	}
}

TrecPointer<EventHandler> TInstance::GetHandler(const TString& name, anagame_page pageType)
{
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (registeredHandlers[Rust].Get())
		{
			if (registeredHandlers[Rust]->name.Compare(name))
				continue;
			switch (pageType)
			{
			case anagame_page::anagame_page_command_prompt:
				if (dynamic_cast<TerminalHandler*>(registeredHandlers[Rust].Get()))
					return registeredHandlers[Rust];
				break;
			case anagame_page::anagame_page_code_file:
				if (dynamic_cast<TCodeHandler*>(registeredHandlers[Rust].Get()))
					return registeredHandlers[Rust];
				break;

				// To-Do: As more Handlers are added, mention them here
			}
		}
	}
	return TrecPointer<EventHandler>();
}
 
void TInstance::RegisterDialog(TrecPointer<TWindow> win)
{
	TrecPointer<WindowContainer> contain = TrecPointerKey::GetNewTrecPointer<WindowContainer>();
	contain->window = win;
	windowList.push_back(contain);
}

void TInstance::CleanWindows()
{
	for (UINT Rust = 0; Rust < windowList.Size(); Rust++)
	{
		if (windowList[Rust].Get() && windowList[Rust]->destroy && !windowList[Rust]->messageStack)
		{
			windowList[Rust]->window.Delete();
			windowList.RemoveAt(Rust);
		}
	}
}

void TInstance::UnregisterHandler(TrecPointer<EventHandler> handler)
{
	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (handler.Get() == registeredHandlers[Rust].Get())
		{
			registeredHandlers.RemoveAt(Rust);
			break;
		}
	}
}

bool TInstance::RegisterHandler(TrecPointer<EventHandler> handler)
{
	if(!handler.Get())
		return false;

	for (UINT Rust = 0; Rust < registeredHandlers.Size(); Rust++)
	{
		if (handler.Get() == registeredHandlers[Rust].Get())
			return false;
	}

	registeredHandlers.push_back(handler);
	handler->id = handlerID++;
	return true;
}

void TInstance::CleanHandlerList()
{
	for (int Rust = static_cast<int>(registeredHandlers.Size()) - 1; Rust >= 0; Rust--)
	{
		if (!registeredHandlers[Rust].Get())
			registeredHandlers.RemoveAt(Rust);
	}
}

WindowContainer::WindowContainer()
{
	destroy = false;
	messageStack = 0;
}
