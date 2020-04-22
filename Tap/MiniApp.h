#pragma once
#include <TObject.h>
#include "HandlerMessage.h"
#include "TIdeWindow.h"
class _TAP_DLL MiniApp : public TObject
{
public:
	MiniApp(TrecSubPointer<TWindow, TIdeWindow> win);

	virtual void DispatchAnagameMessage(TrecPointer<HandlerMessage> message) = 0;

	virtual UINT Initialize() = 0;

	virtual bool ShouldDestroy();

	void SetSelf(TrecPointer<MiniApp> s);

protected:
	TrecSubPointer<TWindow, TIdeWindow> win;

	TrecSubPointer<Page, IDEPage> mainPage;

	TrecPointer<TInstance> instance;
	TrecPointerSoft<MiniApp> self;
};

