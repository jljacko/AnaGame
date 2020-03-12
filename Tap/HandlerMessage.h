#pragma once
#include <TObject.h>
#include <TString.h>

typedef enum handler_type
{
	handler_type_generic,
	handler_type_console,
	handler_type_camera,
	handler_type_file_manager,
	handler_type_compiler,
	handler_type_interpreter,

	handler_type_other
}handler_type;

typedef enum message_transmission
{
	message_transition_firm_id,
	message_transition_firm_name,
	message_transmission_id_over_name,
	message_transmission_name_over_id,
	message_transmission_by_type,
	message_transmission_create_new
};


class HandlerMessage : public TObject
{
public:
	HandlerMessage(const TString& handlerName, handler_type type, UINT otherType, message_transmission transmission, UINT handlerIdTarget, const TString& message);

	TString GetHandlerName();
	TString GetMessage_();

	handler_type GetHandlerType();
	UINT GetHandlerOtherType();
	UINT GetHandlerIdType();
	message_transmission GetMessageTransmission();

private:
	TString handlerName;
	handler_type handlerType;
	UINT handlerOtherType;
	UINT handlerIdTarget;
	message_transmission transmission;
	TString message;
};

