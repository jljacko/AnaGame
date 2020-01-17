#pragma once
#include "EventHandler.h"
class TerminalHandler :
	public EventHandler
{
public:
	TerminalHandler(TrecPointer<TInstance> instance);
	~TerminalHandler();

	virtual void Initialize(TrecPointer<Page> page) override;
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;



private:
	TMap<TControl> terminals;
	TrecPointer<TControl> currentTerminal;
};

