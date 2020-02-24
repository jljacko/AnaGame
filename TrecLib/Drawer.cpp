#include "Drawer.h"


HWND Drawer::GetWindowHandle()
{
	return currentWindow;
}

TrecComPointer<ID2D1Factory1> Drawer::GetFactory()
{
	return directFactory;
}
