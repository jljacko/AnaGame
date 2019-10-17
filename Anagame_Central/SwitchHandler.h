#pragma once
#include <EventHandler.h>


#define SWITCH_HANDLER_METHOD_COUNT 11

class SwitchHandler;

typedef void (SwitchHandler::* methodArray)(TControl* tc, EventArgs ea);

class SwitchHandler :
	public EventHandler
{
public:
	SwitchHandler(TrecPointer<TInstance> ins);
	~SwitchHandler();

	void Initialize(TrecPointer<Page> page);
	void HandleEvents(TDataArray<EventID_Cred>& eventAr);
protected:
	TrecPointer<Page> page;
	methodArray handlers[SWITCH_HANDLER_METHOD_COUNT];
	TLayout* rootLayout;

	TrecPointer<TControl> changeControl;

	void OnSelect1(TControl* tc, EventArgs ea);
	void OnSelect2(TControl* tc, EventArgs ea);
	void OnSelect3(TControl* tc, EventArgs ea);
	void OnSelectRows(TControl* tc, EventArgs ea);
	void OnSelectColumns(TControl* tc, EventArgs ea);
	void OnSelectGrid(TControl* tc, EventArgs ea);
	void OnSelectLayers(TControl* tc, EventArgs ea);
	void OnSelectGadget(TControl* tc, EventArgs ea);
	void OnSelectText(TControl* tc, EventArgs ea);
	void OnSelectCombo(TControl* tc, EventArgs ea);
	void OnSelectScroll(TControl* tc, EventArgs ea);

};

