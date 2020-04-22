#include "CameraHandler.h"
#include "TInstance.h"

TString on_TextDirectionX(L"TextDirectionX");
TString on_TextDirectionY(L"TextDirectionY");
TString on_TextDirectionZ(L"TextDirectionZ");
TString on_TextLocationX(L"TextLocationX");
TString on_TextLocationY(L"TextLocationY");
TString on_TextLocationZ(L"TextLocationZ");
TString on_Up(L"OnUp");
TString on_Down(L"OnDown");
TString on_Left(L"OnLeft");
TString on_Right(L"OnRight");
TString on_Far(L"OnFar");
TString on_Near(L"OnNear");
TString on_SetCameraRotate(L"OnSetCameraRotate");
TString on_SetCameraTranslate(L"OnSetCameraTranslate");
TString on_SelectCurrentObject(L"OnSelectObject");
TString on_GetDefaultObject(L"OnGetDefaultObject");
TString on_ToggleObjectAndCamera(L"OnToggleObjectAndCamera");

CameraHandler::CameraHandler(TrecPointer<TInstance> instance, TString& name): EventHandler(instance)
{
	arenaName.Set(name);
	
	arenaHandlers.push_back(&CameraHandler::TextDirectionX);
	arenaHandlers.push_back(&CameraHandler::TextDirectionY);
	arenaHandlers.push_back(&CameraHandler::TextDirectionZ);

	arenaHandlers.push_back(&CameraHandler::TextLocationX);
	arenaHandlers.push_back(&CameraHandler::TextLocationY);
	arenaHandlers.push_back(&CameraHandler::TextLocationZ);

	arenaHandlers.push_back(&CameraHandler::OnUp);
	arenaHandlers.push_back(&CameraHandler::OnDown);
	arenaHandlers.push_back(&CameraHandler::OnLeft);
	arenaHandlers.push_back(&CameraHandler::OnRight);
	arenaHandlers.push_back(&CameraHandler::OnFar);
	arenaHandlers.push_back(&CameraHandler::OnNear);
	arenaHandlers.push_back(&CameraHandler::OnSetCameraRotate);
	arenaHandlers.push_back(&CameraHandler::OnSetCameraTranslate);

	arenaHandlers.push_back(&CameraHandler::OnGetDefaultObject);
	arenaHandlers.push_back(&CameraHandler::OnSelectObject);
	arenaHandlers.push_back(&CameraHandler::OnToggleObjectAndCamera);

	// Now set the structure to link the listeners to their text name
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(on_TextDirectionX);
	events.push_back(enid);

	enid.eventID = 1;
	enid.name.Set(on_TextDirectionY);
	events.push_back(enid);

	enid.eventID = 2;
	enid.name.Set(on_TextDirectionZ);
	events.push_back(enid);

	enid.eventID = 3;
	enid.name.Set(on_TextLocationX);
	events.push_back(enid);

	enid.eventID = 4;
	enid.name.Set(on_TextLocationY);
	events.push_back(enid);

	enid.eventID = 5;
	enid.name.Set(on_TextLocationZ);
	events.push_back(enid);

	enid.eventID = 6;
	enid.name.Set(on_Up);
	events.push_back(enid);

	enid.eventID = 7;
	enid.name.Set(on_Down);
	events.push_back(enid);

	enid.eventID = 8;
	enid.name.Set(on_Left);
	events.push_back(enid);

	enid.eventID = 9;
	enid.name.Set(on_Right);
	events.push_back(enid);

	enid.eventID = 10;
	enid.name.Set(on_Far);
	events.push_back(enid);

	enid.eventID = 11;
	enid.name.Set(on_Near);
	events.push_back(enid);

	enid.eventID = 12;
	enid.name.Set(on_SetCameraRotate);
	events.push_back(enid);

	enid.eventID = 13;
	enid.name.Set(on_SetCameraTranslate);
	events.push_back(enid);

	enid.eventID = 14;
	enid.name.Set(on_GetDefaultObject);
	events.push_back(enid);

	enid.eventID = 15;
	enid.name.Set(on_SelectCurrentObject);
	events.push_back(enid);

	enid.eventID = 16;
	enid.name.Set(on_ToggleObjectAndCamera);
	events.push_back(enid);

	change = 0.1;
	t_dx = t_dy = t_dz = t_lx = t_ly = 0.0f;
	t_lz = 1.0f;
	rotateMode = true;
}

CameraHandler::~CameraHandler()
{
}

void CameraHandler::Initialize(TrecPointer<Page> page)
{
	if (!page.Get())
		return;

	auto control = page->GetRootControl();

	TrecSubPointer<TControl, TLayout> mainLay = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(control);

	assert(mainLay.Get());

	TrecSubPointer<TControl, TLayout> panelLay = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TLayout>(mainLay->GetLayoutChild(0, 0));

	assert(panelLay.Get());

	d_x = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 1));
	l_x = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 1));
	d_y = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 2));
	l_y = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 2));
	d_z = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(1, 3));
	l_z = TrecPointerKey::GetTrecSubPointerFromTrec<TControl, TTextField>(panelLay->GetLayoutChild(3, 3));

	assert(d_x.Get() && d_y.Get() && d_z.Get()&&
		l_x.Get() && l_y.Get() && l_z.Get());
}

void CameraHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < eventAr.Size(); c++)
	{
		tc = eventAr.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < arenaHandlers.Size())
		{
			// call method
			if (arenaHandlers[e_id])
				(this->*arenaHandlers[e_id])(tc, ea);
		}
	}

	//onDraw();
	eventAr.RemoveAll();
}

void CameraHandler::ProcessMessage(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return;

	TString messageStr = message->GetMessage_();

	auto pieces = messageStr.split(L" ");

	if (!pieces.Get() || pieces->Size() < 5 || pieces->at(0).Compare(L"Camera"))
		return;

	UINT mode = 0;

	for (UINT Rust = 1; Rust < pieces->Size(); Rust++)
	{
		if (!mode)
		{
			if (!pieces->at(Rust).Compare(L"Location"))
				mode = 1;
			else if (!pieces->at(Rust).Compare(L"Direction"))
				mode = 2;
		}

		if (mode == 1 || mode == 2)
		{
			auto data = pieces->at(Rust);

			auto datum = data.split(L";");

			if (!datum.Get() || datum->Size() != 3)
			{
				mode = 0;
				continue;
			}


			if (mode == 1)
			{
				datum->at(0).ConvertToFloat(&t_lx);
				datum->at(1).ConvertToFloat(&t_ly);
				datum->at(2).ConvertToFloat(&t_lz);
			}
			else
			{
				datum->at(0).ConvertToFloat(&t_dx);
				datum->at(1).ConvertToFloat(&t_dy);
				datum->at(2).ConvertToFloat(&t_dz);
			}
			mode = 0;
		}
	}

	UpdatePanelText();
}

bool CameraHandler::ShouldProcessMessageByType(TrecPointer<HandlerMessage> message)
{
	if (!message.Get())
		return false;
	return message->GetHandlerType() == handler_type::handler_type_camera;
}

void CameraHandler::SendMessageToArena(const TString& target, const TString& attribute, const TString& value)
{
	
	TString messageStr(target + L" " + attribute + L" " + value);
	TrecPointer<HandlerMessage> newMessage = TrecPointerKey::GetNewTrecPointer<HandlerMessage>(name, handler_type::handler_type_camera, 0, message_transmission::message_transmission_name_type, 0, messageStr);
	instance->DispatchAnagameMessage(newMessage);
}


void CameraHandler::UpdatePanelText()
{
	if (d_x.Get())
		d_x->setNumericText(t_dx);
	if (d_y.Get())
		d_y->setNumericText(t_dy);
	if (d_z.Get())
		d_z->setNumericText(t_dz);

	if (l_x.Get())
		l_x->setNumericText(t_lx);
	if (l_y.Get())
		l_y->setNumericText(t_ly);
	if (l_z.Get())
		l_z->setNumericText(t_lz);

}

void CameraHandler::TextDirectionX(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_dx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

void CameraHandler::TextLocationX(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_lx = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

void CameraHandler::TextDirectionY(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_dy = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

void CameraHandler::TextLocationY(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_ly = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

void CameraHandler::TextDirectionZ(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_dz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_dx, t_dy, t_dz);
		SendMessageToArena(TString(L"Camera"), TString(L"Direction"), formatedText);
	}
}

void CameraHandler::TextLocationZ(TControl* tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		t_lz = f;
		TString formatedText;
		formatedText.Format(L"%f;%f;%f", t_lx, t_ly, t_lz);
		SendMessageToArena(TString(L"Camera"), TString(L"Location"), formatedText);
	}
}

void CameraHandler::OnUp(TControl* tc, EventArgs ea)
{
	TString formatedText;
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 1.0f, 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

void CameraHandler::OnDown(TControl* tc, EventArgs ea)
{
	TString formatedText;
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 1.0f, 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}

	UpdatePanelText();
}

void CameraHandler::OnLeft(TControl* tc, EventArgs ea)
{
	TString formatedText;
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", change, 0.0f);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 1.0f, 0.0f, 0.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

void CameraHandler::OnRight(TControl* tc, EventArgs ea)
{
	TString formatedText;
	if (rotateMode)
	{
		formatedText.Format(L"%f;%f", -change, 0.0f);
		SendMessageToArena(TString(L"Camera"), TString(L"Rotate"), formatedText);
	}
	else
	{
		formatedText.Format(L"%f;%f;%f;%f", 1.0f, 0.0f, 0.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}

	UpdatePanelText();
}

void CameraHandler::OnNear(TControl* tc, EventArgs ea)
{
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

void CameraHandler::OnFar(TControl* tc, EventArgs ea)
{
	if (!rotateMode)
	{
		TString formatedText;
		formatedText.Format(L"%f;%f;%f;%f", 0.0f, 0.0f, 1.0f, -change);
		SendMessageToArena(TString(L"Camera"), TString(L"Translate"), formatedText);
	}
	UpdatePanelText();
}

void CameraHandler::OnSetCameraRotate(TControl* tc, EventArgs ea)
{
	rotateMode = true;
}

void CameraHandler::OnSetCameraTranslate(TControl* tc, EventArgs ea)
{
	rotateMode = false;
}

void CameraHandler::OnSelectObject(TControl* tc, EventArgs ea)
{
	//if (!modelCollection.Get()) return;

	//currentModel = modelCollection->GetModel(ea.arrayLabel);
}

void CameraHandler::OnToggleObjectAndCamera(TControl*, EventArgs ea)
{
	//focusOnModel = !focusOnModel;

	//if (!camModToggleSign.Get())
	//	return;

	//if (focusOnModel)
	//	camModToggleSign->getText(1)->setCaption(TString(L"Model"));
	//else
	//	camModToggleSign->getText(1)->setCaption(TString(L"Camera"));

	//UpdatePanelText();
}

void CameraHandler::OnGetDefaultObject(TControl* tc, EventArgs ea)
{
	//if (ea.arrayLabel >= 0 && ea.arrayLabel < basicModels.Size() && modelCollection.Get() && basicModels[ea.arrayLabel].Get())
	//{
	//	TrecPointer<ArenaModel> newModel = TrecPointerKey::GetNewSelfTrecPointer<ArenaModel>(*basicModels[ea.arrayLabel].Get());
	//	modelCollection->AddModel(newModel);
	//}
}