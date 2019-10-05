
#include "Page.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include "TInstance.h"
#include "TWindow.h"
#include "MiniHandler.h"

Page::Page()
{
	rt_type = render_target_unknown;
	deviceH = nullptr;
	adjustMatrix = D2D1::Matrix3x2F::Identity();
	area = RECT{ 0,0,0,0 };
	convertCRectToD2DRect(&area, &dRect);
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

TrecPointer<Page> Page::Get2DPage(TrecPointer<TInstance> in, TrecPointer<TWindow> dc, TrecPointer<EventHandler> eh)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";

	TrecComPointer<ID2D1Factory1> fact = in->GetFactory();

	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";

	if (!dc.Get())
		throw L"Error! Window Object MUST be initialized";

	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";

	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder renderDc;
	HRESULT res = fact->CreateDCRenderTarget(&props, renderDc.GetPointerAddress());

	if (FAILED(res))
	{
		throw L"Error! Failed to Generate DC Render Target for Page to use!";
	}

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>();
	ret->deviceH = GetWindowDC(dc->GetWindowHandle());
	ret->windowHandle = dc;
	ret->instance = in;

	ret->regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>(renderDc);

	ret->rt_type = render_target_dc;

	GetClientRect(dc->GetWindowHandle(), &ret->area);
	convertCRectToD2DRect(&ret->area, &ret->dRect);
	ret->handler = eh;
	reinterpret_cast<ID2D1DCRenderTarget*>(ret->regRenderTarget.Get())->BindDC(ret->deviceH, &ret->area);

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder paintHolder;
	ret->regRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), paintHolder.GetPointerAddress());
	ret->clearBursh = paintHolder.Extract();


	return ret;
}

TrecPointer<Page> Page::GetWindowPage(TrecPointer<TInstance> in , TrecPointer<TWindow> window, TrecPointer<EventHandler> eh)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";



	TrecComPointer<ID2D1Factory1> fact = in->GetFactory();
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";
	
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();

	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	RECT area;

	GetClientRect(window->GetWindowHandle(), &area);

	D2D1_HWND_RENDER_TARGET_PROPERTIES hProps;
	ZeroMemory(&hProps, sizeof(hProps));

	hProps.hwnd = window->GetWindowHandle();
	hProps.pixelSize = D2D1::SizeU(area.right - area.left,
		area.bottom - area.top);
	
	

	TrecComPointer<ID2D1HwndRenderTarget>::TrecComHolder renderHw;
	HRESULT res = fact->CreateHwndRenderTarget(props, hProps, renderHw.GetPointerAddress());

	if (FAILED(res))
		throw L"Error! Failed to Create Window Render Target!";
	TrecPointer<Page> ret = GetWindowPage(in, TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1HwndRenderTarget>(renderHw), window, eh);

	convertCRectToD2DRect(&area, &ret->dRect);

	return ret;

}

TrecPointer<Page> Page::GetWindowPage(TrecPointer<TInstance> in, TrecComPointer<ID2D1RenderTarget> render, TrecPointer<TWindow> window, TrecPointer<EventHandler> eh)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";



	TrecComPointer<ID2D1Factory1> fact = in->GetFactory();
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";

	HDC dc = GetWindowDC(window->GetWindowHandle());

	if (!dc)
		throw L"Error! Got NULL DC from provided Window handle!";

	RECT area;

	GetClientRect(window->GetWindowHandle(), &area);

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>();
	// ret->fact = fact;
	ret->windowHandle = window;
	ret->deviceH = dc;
	ret->regRenderTarget = render;
	ret->instance = in;

	ret->rt_type = render_target_hwnd;

	ret->area = area;
	convertCRectToD2DRect(&area, &ret->dRect);
	ret->handler = eh;

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder paintHolder;
	ret->regRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), paintHolder.GetPointerAddress());
	ret->clearBursh = paintHolder.Extract();

	return ret;
}

TrecPointer<Page> Page::Get3DPage(TrecPointer<TInstance> in, TrecPointer<TWindowEngine> engine, TrecPointer<EventHandler> eh, TrecPointer<TWindow> window)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";



	TrecComPointer<ID2D1Factory1> fact = in->GetFactory();
	if (!fact.Get())
		throw L"Error! Factory Object MUST be initialized!";

	if (!engine.Get())
		throw L"Error! ArenaEngine Object MUST be initialized for a 3D enabled Page";
	//if (!eh.Get())
	//	throw L"Error! Event Handler MUST be instantiated!";


	TrecComPointer<IDXGISurface> surf = engine->GetSurface();
	if (!surf.Get())
		throw L"Error! Provided 3D Engine does not have a Surface to create a 2D Render Target with!";
	
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));

	props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	props.pixelFormat = D2D1::PixelFormat();
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.dpiX = props.dpiY = 0.0f;
	props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	TrecComPointer<ID2D1RenderTarget>::TrecComHolder dxgiRender;
	HRESULT res = fact->CreateDxgiSurfaceRenderTarget(surf.Get(), props, dxgiRender.GetPointerAddress());
	
	if (FAILED(res))
		throw L"ERROR! Failed to Generate 3D Compatiblie Render Target!";

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>();
	ret->fact = fact;
	ret->windowHandle = window;
	ret->deviceH = GetWindowDC(window->GetWindowHandle());
	ret->regRenderTarget = dxgiRender.Extract();
	ret->instance = in;

	ret->rt_type = render_target_dxgi;

	GetClientRect(window->GetWindowHandle(), &ret->area);
	convertCRectToD2DRect(&ret->area, &ret->dRect);
	ret->handler = eh;


	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder paintHolder;
	ret->regRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), paintHolder.GetPointerAddress());
	ret->clearBursh = paintHolder.Extract();

	return ret;
}
TrecPointer<Page> Page::GetSmallPage(TrecPointer<Page> p, RECT area)
{
	if(!p.Get())
	return TrecPointer<Page>();

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>();

	ret->area = area;
	ret->bit = p->bit;
	ret->clearBursh = p->clearBursh;
	ret->device = p->device;
	ret->deviceH = p->deviceH;
	ret->engine = p->engine;
	ret->fact = p->fact;
	ret->gdiRender = p->gdiRender;
	ret->instance = p->instance;
	ret->regRenderTarget = p->regRenderTarget;
	ret->rt_type = p->rt_type;
	ret->scale = 1.0f;
	ret->windowHandle = p->windowHandle;
	return ret;
}
int Page::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	AnafaceParser parser(regRenderTarget, windowHandle->GetWindowHandle(), file->GetFileDirectory());

	if (eh.Get())
		parser.setEventSystem(eh->GetEventNameList());

	TML_Reader_ reader(file.Get(), &parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;





	rootControl = parser.getRootControl();
	if (rootControl.Get())
		rootControl->onCreate(area, windowHandle->GetWindowEngine());
	if(handler.Get())
		handler->Initialize(TrecPointerKey::GetTrecPointerFromSoft<Page>(self));
	return 0;
}

int Page::SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	AnafaceParser parser(regRenderTarget, windowHandle->GetWindowHandle(), file->GetFileDirectory());

	parser.setEventSystem(id);

	TML_Reader_ reader(file.Get(), &parser);
	int result = 0;
	if (!reader.read(&result))
		throw result;

	rootControl = parser.getRootControl();
	if (rootControl.Get())
		rootControl->onCreate(area, windowHandle->GetWindowEngine());
	if(handler.Get())
		handler->Initialize(TrecPointerKey::GetTrecPointerFromSoft<Page>(self));
	return 0;
}

void Page::SetAnaface(TrecPointer<TControl> newRoot)
{
	if (!newRoot.Get())
		throw L"Error! Root Control Already expected to be initialized!";

	rootControl = newRoot;

	rootControl->SetNewRenderTarget(regRenderTarget);
}

TrecPointer<TControl> Page::GetRootControl()
{
	return rootControl;
}

TrecPointer<TControl> Page::ExtractRootControl()
{
	TrecPointer<TControl> ret(rootControl);
	rootControl.Nullify();
	return ret;
}

TrecPointer<TWindow> Page::GetWindowHandle()
{
	return windowHandle;
}

void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonUp(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnRButtonUp(nFlags, point, mOut);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDown(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonDown(nFlags, point, mOut);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnRButtonDown(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnRButtonDown(nFlags, point, mOut);
	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnMouseMove(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnMouseMove(nFlags, point, mOut);
	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonDblClk(nFlags, point, mOut, eventAr);
	
	if(handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonDblClk(nFlags, point, mOut);

	if( *mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
	TDataArray<EventID_Cred> eventAr;
	OnLButtonUp(nFlags, point, mOut, eventAr);

	if (handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnLButtonUp(nFlags, point, mOut);

	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return;
}

bool Page::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut)
{
	TDataArray<EventID_Cred> eventAr;
	bool returnable = OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	if (handler.Get())
		handler->HandleEvents(eventAr);

	if (miniHandler.Get())
		miniHandler->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);

	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return false;
	return returnable;
}

void Page::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;


	if (rootControl.Get())
		rootControl->OnRButtonUp(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
}

void Page::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnRButtonDown(nFlags, point, mOut, eventAr);
}

void Page::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnMouseMove(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

	if (rootControl.Get())
		rootControl->OnLButtonDblClk(nFlags, point, mOut, eventAr);
}

void Page::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &area))
		return;

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

void Page::SetSelf(TrecPointer<Page> s)
{
	if (!s.Get() || s.Get() != this)
		throw L"Error! Must Set sef with a valid pointer!";
	self = TrecPointerKey::GetSoftPointerFromTrec<Page>(s);
}

TrecPointer<TInstance> Page::GetInstance()
{
	return instance;
}

void Page::SetMiniHandler(TrecPointer<MiniHandler> mh)
{
	miniHandler = mh;
}



UCHAR * Page::GetAnaGameType()
{
	return nullptr;
}

void Page::OnSize(UINT nType, int cx, int cy)
{
	if (rt_type == render_target_unknown)
		return;

	HDC deviceHandle = GetWindowDC(windowHandle->GetWindowHandle());
	GetBoundsRect(deviceH, &area, 0);
	GetClientRect(windowHandle->GetWindowHandle(), &area);

	convertCRectToD2DRect(&area, &dRect);

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
		//engine->OnWindowResize();

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
			D2D1_ALPHA_MODE_PREMULTIPLIED
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

TrecPointer<TArenaEngine> Page::GetArenaEngine()
{
	return engine;
}

void Page::CreateLayout()
{
	if (rootControl.Get() && windowHandle.Get())
		rootControl->onCreate(area, windowHandle->GetWindowEngine());
}

void Page::Draw()
{
	if (!rootControl.Get() || !regRenderTarget.Get() || !clearBursh.Get()) return;

	//regRenderTarget->BeginDraw();
	//regRenderTarget->Clear(D2D1::ColorF(1.0f,1.0f,1.0f,0.0f));
	regRenderTarget->FillRectangle(dRect, clearBursh.Get());
	
	regRenderTarget->SetTransform(adjustMatrix);
	rootControl->onDraw();
	//regRenderTarget->EndDraw();

	if (handler.Get())
		handler->Draw();
}

RECT Page::GetArea()
{
	return area;
}

void Page::SetArea(RECT& loc)
{
	area = loc;
	convertCRectToD2DRect(&area, &dRect);


	if (rt_type == render_target_dc)
	{
		reinterpret_cast<ID2D1DCRenderTarget*>(regRenderTarget.Get())->BindDC(this->deviceH, &area);
	}
}


