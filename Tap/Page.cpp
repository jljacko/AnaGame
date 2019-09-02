
#include "Page.h"


Page::Page()
{
	rt_type = render_target_unknown;
}

Page::Page(HWND window)
{
	D2D1_FACTORY_OPTIONS factOpts;
	ZeroMemory(&factOpts, sizeof(factOpts));
	factOpts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	TrecComPointer<ID2D1Factory1>::TrecComHolder rawFact;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factOpts, rawFact.GetPointerAddress())))
		fact.Nullify();
	else
		fact = rawFact.Extract();

	rt_type = render_target_unknown;
}


Page::~Page()
{
	// 3D mixing with 2D Resources
	device.Delete();
	gdiRender.Delete();
	bit.Delete();
	fact.Delete();
	regRenderTarget.Delete();
	
}

Page Page::Get2DPage()
{
	return Page();
}

Page Page::GetWindowPage()
{
	return Page();
}

Page Page::Get3DPage()
{
	return Page();
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


bool Page::Initialize2D(HDC deviceH)
{
	if (!fact.Get())
		return false;




	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder tempDC;
	HRESULT hr = fact->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), tempDC.GetPointerAddress());
	assert(SUCCEEDED(hr));

	regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>(tempDC);

	deviceH = GetDC(windowHandle);

	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);
	dynamic_cast<ID2D1DCRenderTarget*>(regRenderTarget.Get())->BindDC(deviceH, &area);

	//width = area.right - area.left;
	//height = area.bottom - area.top;

	return true;
}
