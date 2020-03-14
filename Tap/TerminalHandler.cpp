#include "TerminalHandler.h"

TerminalHandler::TerminalHandler(TrecPointer<TInstance> instance): EventHandler(instance)
{
}

TerminalHandler::~TerminalHandler()
{
}

void TerminalHandler::Initialize(TrecPointer<Page> page)
{
}

void TerminalHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
}

void TerminalHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
}

bool TerminalHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;
	return message->GetHandlerType() == handler_type_console;
}
