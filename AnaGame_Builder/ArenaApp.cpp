#include "stdafx.h"
#include "ArenaApp.h"
#include <DirectoryInterface.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>

TString on_TextDirectionX = L"TextDirectionX";
TString on_TextDirectionY = L"TextDirectionY";
TString on_TextDirectionZ = L"TextDirectionZ";
TString on_TextLocationX = L"TextLocationX";
TString on_TextLocationY = L"TextLocationY";
TString on_TextLocationZ = L"TextLocationZ";
TString on_Up = L"OnUp";
TString on_Down = L"OnDown";
TString on_Left = L"OnLeft";
TString on_Right = L"OnRight";
TString on_Far = L"OnFar";
TString on_Near = L"OnNear";
TString on_SetCameraRotate = L"OnSetCameraRotate";
TString on_SetCameraTranslate = L"OnSetCameraTranslate";

static UINT arenaCount = 0;

ArenaApp::ArenaApp(TrecPointer<AnafaceUI> m, TrecPointer<AnafaceUI>o, TrecPointer<AnafaceUI> e, HWND w) : BuilderApp(m,o,e,w)
{
	for (UINT rust = 0; rust < ARENA_APP_COUNT; rust++)
		handlers[rust] = nullptr;

	handlers[0] = &ArenaApp::TextDirectionX;
	handlers[1] = &ArenaApp::TextDirectionY;
	handlers[2] = &ArenaApp::TextDirectionZ;

	handlers[3] = &ArenaApp::TextLocationX;
	handlers[4] = &ArenaApp::TextLocationY;
	handlers[5] = &ArenaApp::TextLocationZ;

	handlers[6] = &ArenaApp::OnUp;
	handlers[7] = &ArenaApp::OnDown;
	handlers[8] = &ArenaApp::OnLeft;
	handlers[9] = &ArenaApp::OnRight;
	handlers[10] = &ArenaApp::OnFar;
	handlers[11] = &ArenaApp::OnNear;
	handlers[12] = &ArenaApp::OnSetCameraRotate;
	handlers[13] = &ArenaApp::OnSetCameraTranslate;

	// Time to make sure parameters are valid
	if (!m.get() || !o.get() || !w || !e.get())
		return;

	// Now set the structure to link the listeners to their text name
	eventNameID enid;

	enid.eventID = 0;
	enid.name = on_TextDirectionX;
	handleList.push_back(enid);

	enid.eventID = 1;
	enid.name = on_TextDirectionY;
	handleList.push_back(enid);

	enid.eventID = 2;
	enid.name = on_TextDirectionZ;
	handleList.push_back(enid);

	enid.eventID = 3;
	enid.name = on_TextLocationX;
	handleList.push_back(enid);

	enid.eventID = 4;
	enid.name = on_TextLocationY;
	handleList.push_back(enid);

	enid.eventID = 5;
	enid.name = on_TextLocationZ;
	handleList.push_back(enid);

	enid.eventID = 6;
	enid.name = on_Up;
	handleList.push_back(enid);
	
	enid.eventID = 7;
	enid.name = on_Down;
	handleList.push_back(enid);
	
	enid.eventID = 8;
	enid.name = on_Left;
	handleList.push_back(enid);
	
	enid.eventID = 9;
	enid.name = on_Right;
	handleList.push_back(enid);
	
	enid.eventID = 10;
	enid.name = on_Far;
	handleList.push_back(enid);
	
	enid.eventID = 11;
	enid.name = on_Near;
	handleList.push_back(enid);

	enid.eventID = 12;
	enid.name = on_SetCameraRotate;
	handleList.push_back(enid);

	enid.eventID = 13;
	enid.name = on_SetCameraTranslate;
	handleList.push_back(enid);

	TString fileBase;
	fileBase = GetDirectory(cd_AppData);
	TString fileBody = fileBase;
	fileBody += L"\\AnaGame\\ArenaView.tml";

	if (!m.get())
		return;
	TrecComPointer<ID2D1RenderTarget> ren = m->getRenderTarget();
	if (!ren.get())
		return;
	TFile file;
	bool fileOpened = file.Open(fileBody, CFile::modeRead);
	if (!fileOpened)
		return;

	// Now Create THe Anaface that lays it out
	TString directory = file.GetFilePath();
	TString fileName = file.GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetLength());

	AnafaceParser* aParse = new AnafaceParser(ren, w, directory);
	aParse->setEventSystem(handleList);

	TML_Reader_* reader = new TML_Reader_(&file, aParse);
	int readingError = 0;
	if (!reader->read(&readingError))
	{
		file.Close();
		delete reader;
		delete aParse;
		return;
	}
	mainPage = aParse->getRootControl();
	if (mainPage.get())
	{
		TString name;
		name.Format(L"Arena %d", arenaCount++);
		m->addControl(mainPage, name);
		//mainPage->onCreate(m->GetControlArea());
	}

	file.Close();
	delete reader;
	delete aParse;

	// To do! Repeat process 

	fileBody = fileBase;
	fileBody += L"\\AnaGame\\ArenaViewPanel.txt";

	fileOpened = file.Open(fileBody, CFile::modeRead);
	if (!fileOpened)
		return;

	// Now Create THe Anaface that lays it out
	directory = file.GetFilePath();
	fileName = file.GetFileName();
	ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetLength());

	aParse = new AnafaceParser(ren, w, directory);
	aParse->setEventSystem(handleList);

	reader = new TML_Reader_(&file, aParse);
	readingError = 0;
	if (reader->read(&readingError))
	{
		outputPane = aParse->getRootControl();
		o->addControl(outputPane, TString());
	}
	
	file.Close();
	delete reader;
	delete aParse;
}


ArenaApp::~ArenaApp()
{
	mainPage.Delete();
	outputPane.Delete();
	explorerPane.Delete();
}

bool ArenaApp::InitializeControls()
{
	if(!mainPage.get() || !outputUI.get() ||
		!mainUI.get() || !outputPane.get())	
		return false;

	try
	{
		TrecPointer<TLayout> mainLay = dynamic_cast<TLayout*>(outputPane.get());
		arena = dynamic_cast<TArena*>(mainPage.get());
		TrecPointer<TLayout> panelLay = dynamic_cast<TLayout*>(mainLay->GetLayoutChild(0, 0).get());
		d_x = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 1).get());
		l_x = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 1).get());
		d_y = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 2).get());
		l_y = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 2).get());
		d_z = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(1, 3).get());
		l_z = dynamic_cast<TTextField*>(panelLay->GetLayoutChild(3, 3).get());
		TrecPointer<TString> strName;
		strName = new TString(arenaName);
		arena->addAttribute(TString(L"|EngineID"), strName);

		mainPage->onCreate(mainUI->GetControlArea());
		outputPane->onCreate(outputUI->GetControlArea());
		

		modelCollection = arena->getEngine();

		TDataArray<DirectX::XMFLOAT3> scaleVertices;
		scaleVertices.push_back(DirectX::XMFLOAT3(100.0, 0.0, 0.0));   // 0
		scaleVertices.push_back(DirectX::XMFLOAT3(-100.0, 0.0, 0.0));  // 2

		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 100.0, 0.0));
		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, -100.0, 0.0));

		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, 100.0));
		scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, -100.0));

		TDataArray<float> floats;

		for (UINT c = 0; c < scaleVertices.Size(); c++)
		{
			floats.push_back(scaleVertices[c].x);
			floats.push_back(scaleVertices[c].y);
			floats.push_back(scaleVertices[c].z);
			//floats.push_back(1.0f);
		}

		TDataArray<UINT> uints;
		for (UINT c = 0; c < scaleVertices.Size(); c++)
			uints.push_back(c);

		scale = new ArenaModel(*modelCollection.get());
		scale->SetVertexData(floats, default_shader_Single_Color,D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		scale->SetIndices(uints);
		scale->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
		//scale->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

		//modelCollection->AddModel(*(scale.get()));
		UpdateCameraText();
	}
	catch (std::bad_cast& e)
	{
		return false;
	}
	
	return true;
}

TString ArenaApp::GetArenaName()
{
	return arenaName;
}

RECT ArenaApp::Get3DArea()
{
	if(!arena.get())
		return RECT{ 0,0,0,0 };
	return arena->getLocation();
}

void ArenaApp::SetColor(D2D1::ColorF c)
{
	if (!arena.get())
		return;
	if(arena->getContent(1).get())
		arena->getContent(1)->setColor(c);
}

TrecPointer<ArenaEngine> ArenaApp::GetArenaEngine()
{
	if(!arena.get())
		return TrecPointer<ArenaEngine>();
	return arena->getEngine();
}

void ArenaApp::SetArenaName(TString & name)
{
	if (!name.GetLength())
		return;
	arenaName = name;
}

void ArenaApp::UpdateCameraText()
{
	if (!arena.get())
		return;
	DirectX::XMFLOAT3 dir = arena->GetCameraDirection(),
		loc = arena->GetCameraLocation();

	if (d_x.get())
		d_x->setNumericText(dir.x);
	if (d_y.get())
		d_y->setNumericText(dir.y);
	if (d_z.get())
		d_z->setNumericText(dir.z);

	if (l_x.get())
		l_x->setNumericText(loc.x);
	if (l_y.get())
		l_y->setNumericText(loc.y);
	if (l_z.get())
		l_z->setNumericText(loc.z);
}

void ArenaApp::MessageHandler()
{
	if (!arena.get())
		return;
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < cred.Size(); c++)
	{
		tc = cred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < ARENA_APP_COUNT)
		{
			// call method
			if (handlers[e_id])
				(this->*handlers[e_id])(tc, ea);
		}
	}

	//onDraw();
	cred.RemoveAll();
}

void ArenaApp::TextDirectionX(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdateDir(f, 0);
	}
}

void ArenaApp::TextLocationX(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdatePos(f, 0);
	}
}

void ArenaApp::TextDirectionY(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdateDir(f, 1);
	}
}

void ArenaApp::TextLocationY(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdatePos(f, 1);
	}
}

void ArenaApp::TextDirectionZ(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdateDir(f, 2);
	}
}

void ArenaApp::TextLocationZ(TControl * tc, EventArgs ea)
{
	float f = 0.0f;
	if (!ea.text.ConvertToFloat(&f))
	{
		arena->UpdatePos(f, 2);
	}
}

void ArenaApp::OnUp(TControl * tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.0, 0.1);
	else
		arena->Translate(0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
	UpdateCameraText();
}

void ArenaApp::OnDown(TControl * tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.0, -0.1);
	else
		arena->Translate(-0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
	UpdateCameraText();
}

void ArenaApp::OnLeft(TControl * tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(0.1, 0.0);
	else
		arena->Translate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
	UpdateCameraText();
}

void ArenaApp::OnRight(TControl * tc, EventArgs ea)
{
	if (rotateMode)
		arena->Rotate(-0.1, 0.0);
	else
		arena->Translate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
	UpdateCameraText();
}

void ArenaApp::OnNear(TControl * tc, EventArgs ea)
{
	if (!rotateMode)
		arena->Translate(0.1, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
	UpdateCameraText();
}

void ArenaApp::OnFar(TControl * tc, EventArgs ea)
{
	if (!rotateMode)
		arena->Translate(-0.1, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
	UpdateCameraText();
}

void ArenaApp::OnSetCameraRotate(TControl * tc, EventArgs ea)
{
	rotateMode = true;
}

void ArenaApp::OnSetCameraTranslate(TControl * tc, EventArgs ea)
{
	rotateMode = false;
}
