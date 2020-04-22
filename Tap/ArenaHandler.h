#pragma once
#include "EventHandler.h"

class ArenaHandler;
typedef void (ArenaHandler::* ArenaHandlerEvents)(TControl* tc, EventArgs ea);

class _TAP_DLL ArenaHandler :
	public EventHandler
{
public:
	ArenaHandler(TrecPointer<TInstance> instance, TString& name);
	~ArenaHandler();

	virtual void Initialize(TrecPointer<Page> page) override;
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

private:
	TrecSubPointer<TControl, TArena> arenaControl;
	TrecPointer<TArenaEngine> engine;
	TString name;
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	TDataArray<ArenaHandlerEvents> arenaHandlers;
};

