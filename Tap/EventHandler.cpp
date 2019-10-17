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

void EventHandler::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut)
{
}

void EventHandler::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

bool EventHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	return false;
}

TDataArray<eventNameID>& EventHandler::GetEventNameList()
{
	return events;
}

void EventHandler::Draw()
{
}
