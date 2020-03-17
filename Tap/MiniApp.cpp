#include "MiniApp.h"

MiniApp::MiniApp(TrecSubPointer<TWindow, TIdeWindow> win)
{
	this->win = win;
	if (this->win.Get())
		instance = this->win->GetInstance();
}
