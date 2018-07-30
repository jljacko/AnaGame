#include "stdafx.h"
#include "Page.h"


Page::Page()
{

}

Page::Page(HWND window)
{
	D2D1_FACTORY_OPTIONS factOpts;
	ZeroMemory(&factOpts, sizeof(factOpts));
	factOpts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	ID2D1Factory1* rawFact = nullptr;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factOpts, &rawFact)))
		fact = nullptr;
	else
		fact = rawFact;
}


Page::~Page()
{
	regRenderTarget = nullptr;
	// 3D mixing with 2D Resources
	device.Delete();
	contextDevice.Delete();
	gdiRender.Delete();
	bit.Delete();
	fact.Delete();
	renderTarget.Delete();
	
}

int Page::Initialize2D(CDC * cdc)
{
	if (!fact.get())
		return 1;
	if (!cdc)
		return 2;



	ID2D1DCRenderTarget* tempDC = nullptr;
	HRESULT hr = fact->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), &tempDC);
	ASSERT(SUCCEEDED(hr));

	renderTarget = tempDC;
	regRenderTarget = TrecComPointer<ID2D1RenderTarget>(tempDC);

	HDC hdc = cdc->GetSafeHdc();
	this->cdc = cdc;

	UINT boundResult = cdc->GetBoundsRect(&area, 0);
	GetClientRect(windowHandle, &area);
//	width = area.right - area.left;
//	height = area.bottom - area.top;
	renderTarget->BindDC(hdc, &area);
	return 0;
}



int Page::Initialize3D(TString & engineName)
{
	if (!windowHandle)
		return false;

	if (!fact.get())
		return 1;


	engine = ArenaEngine::GetArenaEngine(engineName, windowHandle, instance);

	if (!engine.get())
		return 1;
	IDXGISwapChain* swapper = engine->getSwapChain().get();
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
	IDXGIDevice* dev = engine->getDeviceD_U().get();

	D2D1_CREATION_PROPERTIES cr;
	cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
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
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

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
	//renderTarget = TrecComPointer<ID2D1RenderTarget>(gdiRender_raw);

	deviceH = GetDC(windowHandle);

	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);

//	width = area.right - area.left;
//	height = area.bottom - area.top;

	return 0;
}

int Page::Initialize3D(TString & engine, CDC * cdc)
{
	if (!cdc)
		return 1;
	deviceH = cdc->GetSafeHdc();
	windowHandle = WindowFromDC(deviceH);
	return Initialize3D(engine);
}

UCHAR * Page::GetAnaGameType()
{
	return nullptr;
}

void Page::OnSize(UINT nType, int cx, int cy)
{
	HDC deviceHandle = GetWindowDC(windowHandle);
	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);
//	width = area.right - area.left;
//	height = area.bottom - area.top;
	if (renderTarget.get())
	{
		renderTarget->BindDC(deviceH, &area);
	}
	else if (contextDevice.get())
	{
	
		if (!engine.get())
			return;
		engine->OnWindowResize();

		IDXGISwapChain* swapper = engine->getSwapChain().get();
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
		IDXGIDevice* dev = engine->getDeviceD_U().get();

		D2D1_CREATION_PROPERTIES cr;
		cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
		cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
		ID2D1Device* d2Dev = nullptr;
		res = fact->CreateDevice(dev, &d2Dev);
		
		if (FAILED(res))
		{
			//*error = -3;
			return;
		}
		device = d2Dev;
		D2D1_BITMAP_PROPERTIES1 bmp1;
		bmp1.colorContext = nullptr;
		bmp1.dpiX = bmp1.dpiY = 0;
		bmp1.pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE
		);
		bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
			| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

		ID2D1Bitmap1* rawBit = nullptr;
		res = contextDevice->CreateBitmapFromDxgiSurface(surf, bmp1, &rawBit);
		bit = rawBit;
		contextDevice->SetTarget(bit.get());
	}

	// Now update the Controls to their new change
	//rootControl->
}


bool Page::Initialize2D(HDC deviceH)
{
	if (!fact.get())
		return false;




	ID2D1DCRenderTarget* tempDC = nullptr;
	HRESULT hr = fact->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), &tempDC);
	ASSERT(SUCCEEDED(hr));

	renderTarget = tempDC;
	regRenderTarget = TrecComPointer<ID2D1RenderTarget>(tempDC);
	deviceH = GetDC(windowHandle);

	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle, &area);
	renderTarget->BindDC(deviceH, &area);

	//width = area.right - area.left;
	//height = area.bottom - area.top;

	return true;
}
