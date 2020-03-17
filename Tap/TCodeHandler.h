#pragma once
#include "DocumentHandler.h"
class _TAP_DLL TCodeHandler : public DocumentHandler
{
public:
	TCodeHandler(TrecPointer<TInstance> in);
	TCodeHandler(TrecPointer<TInstance> in, const TString& name);

	virtual void Initialize(TrecPointer<Page> page) override;
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) override;

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) override;

protected:
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	TrecSubPointer<TControl, TTextField> code, lines;

	void OnSave(TFile& file)override;
	virtual void OnLoad(TFile&)override;
};

