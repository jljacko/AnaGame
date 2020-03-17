#pragma once
#include <TObject.h>
#include "HandlerMessage.h"
#include "TIdeWindow.h"
class _TAP_DLL MiniApp :	public TObject
{
public:
	MiniApp(TrecSubPointer<TWindow, TIdeWindow> win);

	virtual void DispatchAnagameMessage(TrecPointer<HandlerMessage> message) = 0;

	virtual UINT Initialize() = 0;

protected:
	TrecSubPointer<TWindow, TIdeWindow> win;
	TrecPointer<TInstance> instance;
};

