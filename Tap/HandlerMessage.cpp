#include "HandlerMessage.h"

HandlerMessage::HandlerMessage(const TString& handlerName, handler_type type, UINT otherType, message_transmission transmission, UINT handlerIdTarget, const TString& message)
{
	this->handlerName.Set(handlerName);
	this->handlerType = type;
	this->handlerOtherType = otherType;
	this->transmission = transmission;
	this->handlerIdTarget = handlerIdTarget;
	this->message.Set(message);
}

TString HandlerMessage::GetHandlerName()
{
	return handlerName;
}

TString HandlerMessage::GetMessage_()
{
	return message;
}

handler_type HandlerMessage::GetHandlerType()
{
	return handlerType;
}

UINT HandlerMessage::GetHandlerOtherType()
{
	return handlerOtherType;
}

UINT HandlerMessage::GetHandlerIdType()
{
	return handlerIdTarget;
}

message_transmission HandlerMessage::GetMessageTransmission()
{
	return transmission;
}
