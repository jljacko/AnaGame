#include "stdafx.h"
#include "EditObjectTDialog.h"
#include "MainFrm.h"

TString mln = L"OnModelLocationNear";
TString mlf = L"OnModelLocationFar";
TString mlu = L"OnModelLocationUp";
TString mld = L"OnModelLocationDown";
TString mll = L"OnModelLocationLeft";
TString mlr = L"OnModelLocationRight";

TString mrn = L"OnModelRotationNear";
TString mrf = L"OnModelRotationFar";
TString mru = L"OnModelRotationUp";
TString mrd = L"OnModelRotationDown";
TString mrl = L"OnModelRotationLeft";
TString mrr = L"OnModelRotationRight";

TString mrg = L"OnModelRotationGrow";
TString mrs = L"OnModelRotationShrink";

TString cln = L"OnCameraLocationNear";
TString clf = L"OnCameraLocationFar";
TString clu = L"OnCameraLocationUp";
TString cld = L"OnCameraLocationDown";
TString cll = L"OnCameraLocationLeft";
TString clr = L"OnCameraLocationRight";

TString crn = L"OnCameraRotationNear";
TString crf = L"OnCameraRotationFar";
TString cru = L"OnCameraRotationUp";
TString crd = L"OnCameraRotationDown";
TString crl = L"OnCameraRotationLeft";
TString crr = L"OnCameraRotationRight";

EditObjectTDialog::EditObjectTDialog():TDialog(1000, 950)
{


	bit = nullptr;
	CMainFrame* window = (CMainFrame*)AfxGetMainWnd();

	TString direct = window->getSpecialDirectory(bdi_user_appdata);

	for (UINT c = 0; c < direct.GetLength(); c++)
	{
		fileHolder += direct[c];
	}

	for (int c = 0; c < ED_3D_DIALOG_METHOD_COUNT; c++)
		_3DDialogMethods[c] = nullptr;

	// To Do: Set up action listeners, once they are added
	_3DDialogMethods[0] = &EditObjectTDialog::OnModelLocationNear;
	_3DDialogMethods[1] = &EditObjectTDialog::OnModelLocationFar;
	_3DDialogMethods[2] = &EditObjectTDialog::OnModelLocationUp;
	_3DDialogMethods[3] = &EditObjectTDialog::OnModelLocationDown;
	_3DDialogMethods[4] = &EditObjectTDialog::OnModelLocationLeft;
	_3DDialogMethods[5] = &EditObjectTDialog::OnModelLocationRight;

	_3DDialogMethods[6] = &EditObjectTDialog::OnModelRotationNear;
	_3DDialogMethods[7] = &EditObjectTDialog::OnModelRotationFar;
	_3DDialogMethods[8] = &EditObjectTDialog::OnModelRotationUp;
	_3DDialogMethods[9] = &EditObjectTDialog::OnModelRotationDown;
	_3DDialogMethods[10] = &EditObjectTDialog::OnModelRotationLeft;
	_3DDialogMethods[11] = &EditObjectTDialog::OnModelRotationRight;

	_3DDialogMethods[12] = &EditObjectTDialog::OnModelRotationGrow;
	_3DDialogMethods[13] = &EditObjectTDialog::OnModelRotationShrink;

	_3DDialogMethods[14] = &EditObjectTDialog::OnCameraLocationNear;
	_3DDialogMethods[15] = &EditObjectTDialog::OnCameraLocationFar;
	_3DDialogMethods[16] = &EditObjectTDialog::OnCameraLocationUp;
	_3DDialogMethods[17] = &EditObjectTDialog::OnCameraLocationDown;
	_3DDialogMethods[18] = &EditObjectTDialog::OnCameraLocationLeft;
	_3DDialogMethods[19] = &EditObjectTDialog::OnCameraLocationRight;

	_3DDialogMethods[20] = &EditObjectTDialog::OnCameraRotationNear;
	_3DDialogMethods[21] = &EditObjectTDialog::OnCameraRotationFar;
	_3DDialogMethods[22] = &EditObjectTDialog::OnCameraRotationUp;
	_3DDialogMethods[23] = &EditObjectTDialog::OnCameraRotationDown;
	_3DDialogMethods[24] = &EditObjectTDialog::OnCameraRotationLeft;
	_3DDialogMethods[25] = &EditObjectTDialog::OnCameraRotationRight;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = mln;
	idMatch.push_back(enid);

	enid.eventID = 1;
	enid.name = mlf;
	idMatch.push_back(enid);

	enid.eventID = 2;
	enid.name = mlu;
	idMatch.push_back(enid);

	enid.eventID = 3;
	enid.name = mld;
	idMatch.push_back(enid);

	enid.eventID = 4;
	enid.name = mll;
	idMatch.push_back(enid);

	enid.eventID = 5;
	enid.name = mlr;
	idMatch.push_back(enid);


	enid.eventID = 6;
	enid.name = mrn;
	idMatch.push_back(enid);

	enid.eventID = 7;
	enid.name = mrf;
	idMatch.push_back(enid);

	enid.eventID = 8;
	enid.name = mru;
	idMatch.push_back(enid);

	enid.eventID = 9;
	enid.name = mrd;
	idMatch.push_back(enid);

	enid.eventID = 10;
	enid.name = mrl;
	idMatch.push_back(enid);

	enid.eventID = 11;
	enid.name = mrr;
	idMatch.push_back(enid);


	enid.eventID = 12;
	enid.name = mrg;
	idMatch.push_back(enid);

	enid.eventID = 13;
	enid.name = mrs;
	idMatch.push_back(enid);


	enid.eventID = 14;
	enid.name = cln;
	idMatch.push_back(enid);

	enid.eventID = 15;
	enid.name = clf;
	idMatch.push_back(enid);

	enid.eventID = 16;
	enid.name = clu;
	idMatch.push_back(enid);

	enid.eventID = 17;
	enid.name = cld;
	idMatch.push_back(enid);

	enid.eventID = 18;
	enid.name = cll;
	idMatch.push_back(enid);

	enid.eventID = 19;
	enid.name = clr;
	idMatch.push_back(enid);


	enid.eventID = 20;
	enid.name = crn;
	idMatch.push_back(enid);

	enid.eventID = 21;
	enid.name = crf;
	idMatch.push_back(enid);

	enid.eventID = 22;
	enid.name = cru;
	idMatch.push_back(enid);

	enid.eventID = 23;
	enid.name = crd;
	idMatch.push_back(enid);

	enid.eventID = 24;
	enid.name = crl;
	idMatch.push_back(enid);

	enid.eventID = 25;
	enid.name = crr;
	idMatch.push_back(enid);
	// End to do
	fileHolder += L"\\3DObjectEdit.tml.txt";
	constructionWorked = setAnaface(fileHolder);

}


EditObjectTDialog::~EditObjectTDialog()
{
}

bool EditObjectTDialog::getValidConstruction()
{
	return constructionWorked;
}

bool EditObjectTDialog::InitializeEngine()
{
	if (!windowHandle)
		return false;

	if (engine.get())
		return false;
	engine = ArenaEngine::GetArenaEngine(TString(L"Dialog"), windowHandle, instance);
	if (!engine.get())
		return false;
	IDXGISwapChain* swapper = engine->getSwapChain().get();
	if (!swapper)
		return false;
	IDXGISurface* surf = nullptr;
	HRESULT res = swapper->GetBuffer(0, IID_PPV_ARGS(&surf));
	if (FAILED(res))
	{
		//*error = -5;
		return false;
	}

	// Here, we diverge from the conventional DCRenderTarget to the Direct 2D Device and Context Device
	IDXGIDevice* dev = engine->getDeviceD_U().get();

	D2D1_CREATION_PROPERTIES cr;
	
	cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
	//cr.
	ID2D1Device* d2Dev = nullptr;
	res = fact->CreateDevice(dev, &d2Dev);
	device = d2Dev;
	if (FAILED(res))
	{
		//*error = -3;
		return false;
	}

	ID2D1DeviceContext* cd = nullptr;

	res = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &cd);
	contextDevice = cd;
	if (FAILED(res))
	{
		device = nullptr;
		return false;
	}

	regRenderTarget = contextDevice.get();

	D2D1_BITMAP_PROPERTIES1 bmp1;
	
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;

	ID2D1Bitmap1* rawBit = nullptr;
	res = contextDevice->CreateBitmapFromDxgiSurface(surf, bmp1, &rawBit);
	bit = rawBit;

	if (FAILED(res))
	{

		device = nullptr;
		contextDevice = nullptr;
		return false;
	}

	contextDevice->SetTarget(bit.get());
	ID2D1GdiInteropRenderTarget* gdiRender_raw = nullptr;
	res = contextDevice->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)&gdiRender_raw);
	gdiRender = gdiRender_raw;
	bit = nullptr;
	/*
	float x = 0.0f, y = 0.0f;
	fact->GetDesktopDpi(&x, &y);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
	D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
	x,
	y,
	D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

	res = fact->CreateDxgiSurfaceRenderTarget(surf, &props, reinterpret_cast<ID2D1RenderTarget**>(&renderTarget));


	*/
	if (FAILED(res))
	{
		//*error = -3;

		//DXTrace(__FILEW__, __LINE__, res, nullptr, TRUE);
		return false;
	}
	return true;
}

void EditObjectTDialog::initializeControls()
{
	TrecPointer<TControl> dummy;
	// Get the Control for the Top Section
	try
	{
		dummy = dynamic_cast<TLayout*>(pointer.get())->GetLayoutChild(1, 0);
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nRoot Control needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nRoot Control Appears to be null", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}

	TrecPointer<TLayout> arenaStack;

	try
	{
		arenaStack = dynamic_cast<TLayout*>(dummy.get());
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\n Right Stack Control needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nRight Stack Control Appears to be null", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}

	dummy = arenaStack->GetLayoutChild(0, 1);

	try
	{
		arenaControl = dynamic_cast<TArena*>(dummy.get());
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nArena Control needed to be an arena control", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nArena Control Appears to be null", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
}

void EditObjectTDialog::onDraw()
{
	if (!pointer.get())
		return;
	if (contextDevice.get())
	{
		contextDevice->BeginDraw();
		if (engine.get())
			engine->PrepareScene(D2D1::ColorF(D2D1::ColorF::White, 1.0f));
		if (firstDraw)
		{
			contextDevice->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));
			firstDraw = false;
		}
		pointer->onDraw();

		HDC contDC = 0;
		if (gdiRender.get() && SUCCEEDED(gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
		{
			HDC windDC = GetDC(windowHandle);
			if (windDC)
			{
				SelectObject(windDC, GetStockObject(DC_BRUSH));
				SetDCBrushColor(windDC, RGB(255, 255, 255));
			}
			if (engine.get())
				engine->FinalizeScene();
			//RECT winLoc;
			//if (GetWindowRect(windowHandle, &winLoc))
			//	InvertRect(contDC, &winLoc);
			if (arenaControl.get())
			{
				RECT d3dRect = arenaControl->getSnip();
				int w = d3dRect.right - d3dRect.left;
				int h = d3dRect.bottom - d3dRect.top;
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = 0;
				bf.SourceConstantAlpha = 0xff;
				if (AlphaBlend(contDC, d3dRect.left, d3dRect.top, w, h, windDC, d3dRect.left, d3dRect.top, w, h, bf))
					int er = GetLastError();
			}
			int er = 0;
			if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, SRCCOPY))
			{
				er = GetLastError();
			}

			gdiRender->ReleaseDC(nullptr);
		}
		HRESULT res = contextDevice->EndDraw();
	}
}

void EditObjectTDialog::OnModelLocationNear(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Translate(-1.0f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
}

void EditObjectTDialog::OnModelLocationFar(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Translate(1.0f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
}

void EditObjectTDialog::OnModelLocationUp(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Translate(1.0f, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
}

void EditObjectTDialog::OnModelLocationDown(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	targetModel->Translate(-1.0f, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
}

void EditObjectTDialog::OnModelLocationLeft(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Translate(-1.0f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnModelLocationRight(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Translate(1.0f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::MessageHandler()
{
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < EventCred.Size(); c++)
	{
		tc = EventCred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < ED_3D_DIALOG_METHOD_COUNT)
		{
			// call method
			if (_3DDialogMethods[e_id])
				(this->*_3DDialogMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

void EditObjectTDialog::OnModelRotationNear(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Rotate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnModelRotationFar(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Rotate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnModelRotationUp(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	targetModel->Rotate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnModelRotationDown(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	targetModel->Rotate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnModelRotationLeft(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Rotate(-0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
}

void EditObjectTDialog::OnModelRotationRight(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Rotate(0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
}

void EditObjectTDialog::OnModelRotationGrow(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Enlarge(0.5);
}

void EditObjectTDialog::OnModelRotationShrink(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;

	targetModel->Enlarge(-0.5);
}

void EditObjectTDialog::OnCameraLocationNear(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Translate(-1.0f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
}

void EditObjectTDialog::OnCameraLocationFar(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Translate(1.0f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
}

void EditObjectTDialog::OnCameraLocationUp(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;


}

void EditObjectTDialog::OnCameraLocationDown(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;


}

void EditObjectTDialog::OnCameraLocationLeft(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Translate(-1.0f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnCameraLocationRight(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Translate(1.0f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnCameraRotationNear(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	///arenaControl->Rotate(-0.100000000f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnCameraRotationFar(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	///arenaControl->Rotate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
}

void EditObjectTDialog::OnCameraRotationUp(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Rotate(0.0, 0.1);
}

void EditObjectTDialog::OnCameraRotationDown(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Rotate(-0.0, -0.1);
}

void EditObjectTDialog::OnCameraRotationLeft(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Rotate(-0.1, 0.0);
}

void EditObjectTDialog::OnCameraRotationRight(TControl * tc, EventArgs ea)
{
	if (!arenaControl.get())
		return;

	arenaControl->Rotate(0.1, 0.0);
}
