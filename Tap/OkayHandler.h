#pragma once
#include "EventHandler.h"
class OkayHandler :
	public EventHandler
{
public:
	OkayHandler(TrecPointer<TInstance> instance);
	~OkayHandler();

	void Initialize(TrecPointer<Page> page) override;
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

protected:
	void OnOkay(TControl* control, EventArgs ea);
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
};

