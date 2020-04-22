#pragma once
#include "EventHandler.h"

class CameraHandler;
typedef void (CameraHandler::* CameraHandlerEvents)(TControl* tc, EventArgs ea);

class _TAP_DLL CameraHandler :
	public EventHandler
{
public:
	CameraHandler(TrecPointer<TInstance> instance, TString& name);
	~CameraHandler();

	virtual void Initialize(TrecPointer<Page> page) override;
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


protected:
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	void SendMessageToArena(const TString& target, const TString& attribute, const TString& value);
	// Data to work with
	bool rotateMode;
	TString arenaName;
	float t_lx, t_ly, t_lz, t_dx, t_dy, t_dz;
	float change;

	// Controls
	TrecSubPointer<TControl, TTextField> d_x, d_y, d_z, l_x, l_y, l_z;
	TrecPointer<TControl> camModToggleSign;
	void UpdatePanelText();

	// Handler Resources
	TDataArray<CameraHandlerEvents> arenaHandlers;

	// Handler Methods
	void TextDirectionX(TControl* tc, EventArgs ea);
	void TextLocationX(TControl* tc, EventArgs ea);
	void TextDirectionY(TControl* tc, EventArgs ea);
	void TextLocationY(TControl* tc, EventArgs ea);
	void TextDirectionZ(TControl* tc, EventArgs ea);
	void TextLocationZ(TControl* tc, EventArgs ea);
	void OnUp(TControl* tc, EventArgs ea);
	void OnDown(TControl* tc, EventArgs ea);
	void OnLeft(TControl* tc, EventArgs ea);
	void OnRight(TControl* tc, EventArgs ea);
	void OnNear(TControl* tc, EventArgs ea);
	void OnFar(TControl* tc, EventArgs ea);
	void OnSetCameraRotate(TControl* tc, EventArgs ea);
	void OnSetCameraTranslate(TControl* tc, EventArgs ea);

	void OnSelectObject(TControl* tc, EventArgs ea);
	void OnToggleObjectAndCamera(TControl*, EventArgs ea);

	void OnGetDefaultObject(TControl* tc, EventArgs ea);

};

