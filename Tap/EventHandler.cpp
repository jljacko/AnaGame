#include "EventHandler.h"
#include "TInstance.h"

EventHandler::EventHandler(TrecPointer<TInstance> instance)
{
	app = instance;
}

EventHandler::~EventHandler()
{

}

bool EventHandler::OnDestroy()
{
    return true;
}

TDataArray<eventNameID>& EventHandler::GetEventNameList()
{
	return events;
}
