#pragma once
#include "Tap_dll.h"
#include <AnafaceParser.h>

class Page;
class TInstance;

class _TAP_DLL EventHandler
{
public:
    EventHandler(TrecPointer<TInstance> instance);
    virtual ~EventHandler();

    virtual void Initialize(Page* page) = 0;
    virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) = 0;
    virtual bool OnDestroy();

	TDataArray<eventNameID>& GetEventNameList();

protected:
	TrecPointer<TInstance> app;
	TDataArray<eventNameID> events;
};