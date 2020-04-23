#pragma once
#include "EventHandler.h"
class _TAP_DLL FileHandler :	public EventHandler
{
public:
	FileHandler(TrecPointer<TInstance> instance);
	~FileHandler();

	virtual void Initialize(TrecPointer<Page> page) override;
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


protected:
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;
	
	TrecSubPointer<TControl, TTreeDataBind> browser;
};

