#include "EventHandler.h"
#include "TInstance.h"

EventHandler::EventHandler(TrecPointer<TInstance> instance)
{
	app = instance;
}

EventHandler::EventHandler(TrecPointer<TInstance> instance, const TString& name)
{
	app = instance;
	this->name.Set(name);
}

EventHandler::~EventHandler()
{

}

bool EventHandler::ShouldProcessMessage(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;

	switch (message->GetMessageTransmission())
	{
	case message_transition_firm_id:
		return message->GetHandlerIdType() == id;
	case message_transition_firm_name:
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission_id_over_name:
		if (message->GetHandlerIdType() == id)
			return true;
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission_name_over_id:
		if (name.GetSize() && !name.Compare(message->GetHandlerName()))
			return true;
		return message->GetHandlerIdType() == id;
	case message_transmission_by_type:
		return this->ShouldProcessMessageByType(message);
	}
	return true;
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

void EventHandler::OnResize(D2D1_RECT_F newSize)
{
}

TDataArray<eventNameID>& EventHandler::GetEventNameList()
{
	return events;
}

void EventHandler::Draw()
{
}

void EventHandler::SetSelf(TrecPointer<EventHandler> handleSelf)
{
	if (handleSelf.Get() != this)
		throw L"Error! Needs to be set to Self";
	hSelf = TrecPointerKey::GetSoftPointerFromTrec<EventHandler>(handleSelf);
	if (instance.Get())
		instance->RegisterHandler(handleSelf);
}

UINT EventHandler::GetId()
{
	return id;
}
