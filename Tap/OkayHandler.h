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

protected:
	void OnOkay(TControl* control, EventArgs ea);

};

