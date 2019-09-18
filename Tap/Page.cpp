
#include "Page.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>

Page::Page()
{
	rt_type = render_target_unknown;
	windowHandle = nullptr;
	deviceH = nullptr;
	adjustMatrix = D2D1::Matrix3x2F::Identity();
	area = RECT{ 0,0,0,0 };
	instance = nullptr;
	scale = 1.0f;
}


Page::~Page()
{
	rootControl.Delete();
	// 3D mixing with 2D Resources
	device.Delete();
	gdiRender.Delete();
	bit.Delete();
	fact.Delete();
	regRenderTarget.Delete();
	
}

TrecPointer<Page> Page::Get2DPage(TrecComPointer<ID2D1Factory1> fact, HDC dc, TrecPointer<EventHandler> eh)
{
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";

	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";

	HWND windowHandle = WindowFromDC(dc);
	if (!windowHandle)
		throw L"Error! Window Handle from provided HDC is NULL!";

	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder renderDc;
	HRESULT res = fact->CreateDCRenderTarget(&props, renderDc.GetPointerAddress());

	if (FAILED(res))
	{
		throw L"Error! Failed to Generate DC Render Target for Page to use!";
	}

	TrecPointer<Page> ret = TrecPointerKey::GetNewTrecPointer<Page>();
	ret->deviceH = dc;
	ret->windowHandle = windowHandle;
	ret->instance = GetModuleHandle(nullptr);

	ret->regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>(renderDc);

	ret->rt_type = render_target_dc;

	GetClientRect(ret->windowHandle, &ret->area);
	ret->handler = eh;
	reinterpret_cast<ID2D1DCRenderTarget*>(ret->regRenderTarget.Get())->BindDC(ret->deviceH, &ret->area);
	return ret;
}

TrecPointer<Page> Page::GetWindowPage(TrecComPointer<ID2D1Factory1> fact,HWND window, TrecPointer<EventHandler> eh)
{
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	RECT area;

	GetClientRect(window, &area);

	D2D1_HWND_RENDER_TARGET_PROPERTIES hProps;
	ZeroMemory(&hProps, sizeof(hProps));

	hProps.hwnd = window;
	hProps.pixelSize = D2D1::SizeU(area.right - area.left,
		area.bottom - area.top);

	TrecComPointer<ID2D1HwndRenderTarget>::TrecComHolder renderHw;
	HRESULT res = fact->CreateHwndRenderTarget(props, hProps, renderHw.GetPointerAddress());

	if (FAILED(res))
		throw L"Error! Failed to Create Window Render Target!";
	TrecPointer<Page> ret = GetWindowPage(TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1HwndRenderTarget>(renderHw), window, eh);
	return ret;

}

TrecPointer<Page> Page::GetWindowPage(TrecComPointer<ID2D1RenderTarget> render, HWND window, TrecPointer<EventHandler> eh)
{
	if (!render.Get())
		throw L"Error! RenderTarget Object MUST be initialized!";
	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";

	HDC dc = GetWindowDC(window);

	if (!dc)
		throw L"Error! Got NULL DC from provided Window handle!";

	RECT area;

	GetClientRect(window, &area);

	TrecPointer<Page> ret = TrecPointerKey::GetNewTrecPointer<Page>();
	// ret->fact = fact;
	ret->windowHandle = window;
	ret->deviceH = GetWindowDC(window);
	ret->regRenderTarget = render;
	ret->instance = GetModuleHandle(nullptr);

	ret->rt_type = render_target_hwnd;

	ret->area = area;
	ret->handler = eh;
	return ret;
}

TrecPointer<Page> Page::Get3DPage(TrecComPointer<ID2D1Factory1> fact, TrecPointer<ArenaEngine> engine, TrecPointer<EventHandler> eh)
{
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	if (!engine.Get())
		throw L"Error! ArenaEngine Object MUST be initialized for a 3D enabled Page";
	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";


	TrecComPointer<IDXGISurface> surf = engine->GetSurface();
	if (!surf.Get())
		throw L"Error! Provided 3D Engine does not have a Surface to create a 2D Render Target with!";
	
	HWND window = engine->GetWindow();
	if(!window)
		throw L"Error! Window Handle from provided 3D Engine is NULL!";
	
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	TrecComPointer<ID2D1RenderTarget>::TrecComHolder dxgiRender;
	HRESULT res = fact->CreateDxgiSurfaceRenderTarget(surf.Get(), props, dxgiRender.GetPointerAddress());
	
	if (FAILED(res))
		throw L"ERROR! Failed to Generate 3D Compatiblie Render Target!";

	TrecPointer<Page> ret = TrecPointerKey::GetNewTrecPointer<Page>();
	ret->fact = fact;
	ret->windowHandle = window;
	ret->deviceH = GetWindowDC(window);
	ret->regRenderTarget = dxgiRender.Extract();
	ret->instance = GetModuleHandle(nullptr);

	ret->rt_type = render_target_dxgi;

	GetClientRect(ret->windowHandle, &ret->area);
	ret->handler = eh;
	return ret;
}
int Page::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	AnafaceParser parser(regRenderTarget, windowHandle, file->GetFileDirectory());

	if (eh.Get())
		parser.setEventSystem(eh->GetEventNameList());

	TML_Reader_ reader(file.Get(), &parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;





	rootControl = parser.getRootControl();
	if (rootControl.Get())
		rootControl->onCreate(area);
	if(handler.Get())
		handler->Initialize(this);
	return 0;
}

int Page::SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	AnafaceParser parser(regRenderTarget, windowHandle, file->GetFileDirectory());

	parser.setEventSystem(id);

	TML_Reader_ reader(file.Get(), &parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;

	rootControl = parser.getRootControl();
	if (rootControl.Get())
		rootControl->onCreate(area);
	if(handler.Get())
		handler->Initialize(this);
	return 0;
}

TrecPointer<TControl> Page::GetRootControl()
{
	return rootControl;
}

HWND Page::GetWindowHandle()
{
	return windowHandle;
}

void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonUp(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);
	
	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnRButtonUp(nFlags, point, mOut);
}

void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDown(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnLButtonDown(nFlags, point, mOut);
}

void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonDown(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnRButtonDown(nFlags, point, mOut);
}

void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnMouseMove(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnMouseMove(nFlags, point, mOut);
}

void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDblClk(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnLButtonDblClk(nFlags, point, mOut);
}

void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonUp(nFlags, point, mOut, eventAr);

	if (handler.Get())
		handler->HandleEvents(eventAr);

	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();

	if (handler.Get())
		handler->OnLButtonUp(nFlags, point, mOut);
}

bool Page::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut)
{
	TDataArray<EventID_Cred> eventAr;
	bool returnable = OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	if (handler.Get())
		handler->HandleEvents(eventAr);

	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		Draw();


	if (handler.Get())
		handler->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
	return returnable;
}

void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		rootControl->OnRButtonUp(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		rootControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
}

void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		rootControl->OnRButtonDown(nFlags, point, mOut, eventAr);
}

void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		rootControl->OnMouseMove(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		rootControl->OnLButtonDblClk(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	UINT curSize = clickedControl.Size();
	for (UINT c = 0; c < clickedControl.Size(); c++)
	{
		if (clickedControl[c])
			clickedControl[c]->SetNormalMouseState();
	}
	clickedControl.RemoveAll();

	if (rootControl.Get())
		rootControl->OnLButtonUp(nFlags, point, mOut, eventAr);

	if (curSize)
	{
		switch (*mOut)
		{
		case negative:
			*mOut = negativeUpdate;
			break;
		case positiveContinue:
			*mOut = positiveContinueUpdate;
			break;
		case positiveOverride:
			*mOut = positiveOverrideUpdate;
		}
	}
}

bool Page::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (rootControl.Get())
		return rootControl->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	return false;
}

bool Page::OnDestroy()
{
	if(handler.Get())
		return handler->OnDestroy();
	return true;
}

/*
int Page::Initialize2D(CDC * cdc)
{
	if (!fact.Get())
		return 1;
	if (!cdc)
		return 2;



	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder tempDC;
	HRESULT hr = fact->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), tempDC.GetPointerAddress());
	ASSERT(SUCCEEDED(hr));

	rt_type = render_target_dc;

	regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>(tempDC);

	HDC hdc = cdc->GetSafeHdc();
	this->cdc = cdc;

	UINT boundResult = cdc->GetBoundsRect(&area, 0);
	GetClientRect(windowHandle, &area);
//	width = area.right - area.left;
//	height = area.bottom - area.top;
	dynamic_cast<ID2D1DCRenderTarget*>(regRenderTarget.Get())->BindDC(hdc, &area);
	return 0;
}



int Page::Initialize3D(TString & engineName)
{
	if (!windowHandle)
		return false;

	if (!fact.Get())
		return 1;


	engine = ArenaEngine::GetArenaEngine(engineName, windowHandle, instance);

	if (!engine.Get())
		return 1;
	IDXGISwapChain* swapper = engine->getSwapChain().Get();
	if (!swapper)
		return 2;
	IDXGISurface* surf = nullptr;
	HRESULT res = swapper->GetBuffer(0, IID_PPV_ARGS(&surf));
	if (FAILED(res))
	{
		//*error = -5;
		return 3;
	}

	// Here, we diverge from the conventional DCRenderTarget to the Direct 2D Device and Context Device
	IDXGIDevice* dev = engine->getDeviceD_U().Get();

	D2D1_CREATION_PROPERTIES cr;
	cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
	TrecComPointer<ID2D1Device>::TrecComHolder d2Dev;
	res = fact->CreateDevice(dev, d2Dev.GetPointerAddress());
	device = d2Dev.Extract();
	if (FAILED(res))
	{
		//*error = -3;
		return false;
	}

	TrecComPointer<ID2D1DeviceContext>::TrecComHolder cd;

	res = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, cd.GetPointerAddress());

	if (FAILED(res))
	{
		device.Nullify();
		return false;
	}

	regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DeviceContext>(cd);

	D2D1_BITMAP_PROPERTIES1 bmp1;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	TrecComPointer<ID2D1Bitmap1>::TrecComHolder rawBit;
	res = dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get())->CreateBitmapFromDxgiSurface(surf, bmp1, rawBit.GetPointerAddress());
	bit = rawBit.Extract();

	if (FAILED(res))
	{

		device.Nullify();
		return false;
	}

	dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get())->SetTarget(bit.Get());
	TrecComPointer<ID2D1GdiInteropRenderTarget>::TrecComHolder gdiRender_raw;
	res = dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get())->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)gdiRender_raw.GetPointerAddress());
	gdiRender = gdiRender_raw.Extract();
	bit.Nullify();
	//renderTarget = TrecComPointer<ID2D1RenderTarget>(gdiRender_raw);

	deviceH = GetDC(windowHandle);

	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);

//	width = area.right - area.left;
//	height = area.bottom - area.top;

	rt_type = render_target_device_context;

	return 0;
}

int Page::Initialize3D(TString & engine, CDC * cdc)
{
	if (!cdc)
		return 1;
	deviceH = cdc->GetSafeHdc();
	windowHandle = WindowFromDC(deviceH);
	return Initialize3D(engine);
}*/

UCHAR * Page::GetAnaGameType()
{
	return nullptr;
}

void Page::OnSize(UINT nType, int cx, int cy)
{
	if (rt_type == render_target_unknown)
		return;

	HDC deviceHandle = GetWindowDC(windowHandle);
	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);
//	width = area.right - area.left;
//	height = area.bottom - area.top;
	if (rt_type == render_target_dc)
	{
		dynamic_cast<ID2D1DCRenderTarget*>(regRenderTarget.Get())->BindDC(deviceH, &area);
	}
	else if (rt_type == render_target_device_context)
	{
	
		if (!engine.Get())
			return;
		engine->OnWindowResize();

		IDXGISwapChain* swapper = engine->getSwapChain().Get();
		if (!swapper)
			return;
		HRESULT res/* = swapper->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)*/;

		//if (FAILED(res))
		//	return;

		IDXGISurface* surf = nullptr;
		res = swapper->GetBuffer(0, IID_PPV_ARGS(&surf));
		if (FAILED(res))
		{
			//*error = -5;
			return;
		}
		IDXGIDevice* dev = engine->getDeviceD_U().Get();

		D2D1_CREATION_PROPERTIES cr;
		cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
		cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
		TrecComPointer<ID2D1Device>::TrecComHolder d2Dev;
		res = fact->CreateDevice(dev, d2Dev.GetPointerAddress());
		
		if (FAILED(res))
		{
			//*error = -3;
			return;
		}
		device = d2Dev.Extract();
		D2D1_BITMAP_PROPERTIES1 bmp1;
		bmp1.colorContext = nullptr;
		bmp1.dpiX = bmp1.dpiY = 0;
		bmp1.pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE
		);
		bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
			| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

		TrecComPointer<ID2D1Bitmap1>::TrecComHolder rawBit;
		res = dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get())->CreateBitmapFromDxgiSurface(surf, bmp1, rawBit.GetPointerAddress());
		bit = rawBit.Extract();
		dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get())->SetTarget(bit.Get());
	}

	// Now update the Controls to their new change
	//rootControl->
}

TrecComPointer<ID2D1RenderTarget> Page::GetRenderTarget()
{
	return regRenderTarget;
}

TrecPointer<ArenaEngine> Page::GetArenaEngine()
{
	return engine;
}

void Page::CreateLayout()
{
	if (rootControl.Get())
		rootControl->onCreate(area);
}

void Page::Draw()
{
	if (!rootControl.Get() || !regRenderTarget.Get()) return;

	regRenderTarget->BeginDraw();
	regRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	regRenderTarget->SetTransform(adjustMatrix);
	rootControl->onDraw();
	regRenderTarget->EndDraw();

}

void Page::SetArea(RECT& loc)
{
	area = loc;
}


