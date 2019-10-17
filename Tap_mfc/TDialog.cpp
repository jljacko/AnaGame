#include "stdafx.h"
#include "TDialog.h"
#include "TML_Reader_.h"
#include "AnafaceParser.h"
#include <Windows.h>
#include <d2d1.h>

namespace
{
	TDataArray<TDialog*> dialogArray;
	//static int stLocation = 0;
	LRESULT CALLBACK WindowProctor(HWND wind, UINT message, WPARAM wPar, LPARAM lPar)
	{
		// Get the Dialog that contains the right window
		TDialog* td = nullptr;
		for (UINT c = 0; c < dialogArray.Size();c++)
		{
			td = dialogArray[c];
			if (td && td->getWindowHandle() == wind)
				break;
			td = nullptr;
		}

		if (!td)
			return DefWindowProc(wind, message, wPar, lPar);

		switch (message)
		{
		case WM_PAINT:
			td->onDraw();
			break;
		case WM_LBUTTONDOWN:
			td->onButtonDown(true, lPar);
			break;
		case WM_MOUSEMOVE:
			td->onMouseMove(lPar);
			break;
		case WM_LBUTTONUP:
			td->onButtonUp(true, lPar);
			break;
		case WM_RBUTTONDOWN:
			td->onButtonDown(false, lPar);
			break;
		case WM_RBUTTONUP:
			td->onButtonUp(false, lPar);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CHAR:
			td->onChar(lPar, wPar);
			break;
		default:
			return DefWindowProc(wind, message, wPar, lPar);
		}
		return DefWindowProc(wind,message,wPar,lPar);
	}


	

}


TDialog::TDialog(int width, int height)
{
	ZeroMemory(&windowFeatures, sizeof(windowFeatures));
	
	windowHandle = 0;
	instance = 0;



	D2D1_FACTORY_OPTIONS factOpts;
	
	factOpts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	TrecComPointer<ID2D1Factory1>::TrecComHolder rawFact;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factOpts, rawFact.GetPointerAddress())))
		fact.Nullify();
	else
		fact = rawFact.Extract();

	this->width = width;
	this->height = height;

	isReady = false;
	statusValue = 0;
	//TrecPointer<TDialog> td(this);
	
	bool notFound = true;
	for (UINT c = 0; c < dialogArray.Size(); c++)
	{
		if (!dialogArray[c])
		{
			dialogArray.setAt(this, c);
			notFound = false;
		}
	}
	if(notFound)
		dialogArray.push_back(this);
	firstDraw = true;
//	location = stLocation++;

}


TDialog::~TDialog()
{
	pointer.Delete();
	device.Delete();


	fact.Delete();


	gdiRender.Delete();

	for (UINT c = 0; c < dialogArray.Size(); c++)
	{
		if (dialogArray[c] == this)
			dialogArray.setAt(nullptr, c);
	}

}

bool TDialog::setAnaface(TString& path)
{
	if (!path.GetSize())
		return false;
	TFile file;
	FileOpened = file.Open(path, TFile::t_file_read);

	for (unsigned int c = 0; c < path.GetSize(); c++)
		filePath += path[c];

	if(FileOpened)
		file.Close();

	return FileOpened;
}

bool TDialog::setName(TString &n)
{
	if(n.IsEmpty())
	return false;
	name = n;
	return true;
}


bool TDialog::InitializeWindow(int& error)
{




		// Create the Window
		//ZeroMemory(&windowFeatures, sizeof(windowFeatures));
		//windowFeatures.cbSize = sizeof(windowFeatures);
		windowFeatures.style = CS_HREDRAW | CS_VREDRAW;
		windowFeatures.lpfnWndProc = WindowProctor;
		windowFeatures.cbClsExtra = 0;
		windowFeatures.cbWndExtra = 0;
		windowFeatures.hInstance = AfxGetInstanceHandle();
		instance = windowFeatures.hInstance;
		windowFeatures.hIcon = nullptr;//LoadIcon(NULL, IDI_APPLICATION);
		windowFeatures.hCursor = nullptr;//LoadCursor(NULL, IDC_ARROW);
		windowFeatures.hbrBackground = nullptr;//(HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
		windowFeatures.lpszMenuName = nullptr;
		windowFeatures.lpszClassName = name.GetConstantBuffer();

		//windowFeatures.hIconSm = nullptr;


	if (!at)
	{

		reRegister:
		at = RegisterClass(&windowFeatures);


		if (!at && GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
		{
			UnregisterClass(windowFeatures.lpszClassName, windowFeatures.hInstance);
			goto reRegister;
		}

		if (!at)
		{
			error = -2;
			DWORD err = GetLastError();
			return false;
		}
	}
	windowHandle = CreateWindow(name.GetConstantBuffer(),
		name.GetConstantBuffer(),
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		width,
		height,
		nullptr,
		nullptr,
		windowFeatures.hInstance,
		nullptr);

	if (!windowHandle)
	{
		error = -3;
		DWORD err = GetLastError();
		return false;
	}

	deviceH = GetDC(windowHandle);


}

bool TDialog::InitializeAnaface(int & error)
{
	FileOpened = file.Open(filePath, TFile::t_file_read);

	if (!FileOpened)
	{
		error = -1;
		return false;
	}


	// Now Create THe Anaface that lays it out
	TString directory = file.GetFilePath();
	TString fileName = file.GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetSize());
	
	AnafaceParser* aParse = new AnafaceParser(regRenderTarget, windowHandle, &directory);
	aParse->setEventSystem(idMatch);

	TML_Reader_* reader = new TML_Reader_(&file, aParse);
	int readingError;
	if (!reader->read(&readingError))
	{
		error = readingError;
		return false;
	}
	else
	{
		TrecPointer<TControl> rootControl = aParse->getRootControl();
		if (rootControl.Get())
		{
			pointer = rootControl;

			pointer->onCreate(area);
		}
		else
		{
			error = -4;
			return false;
		}
	}
	file.Close();
	isReady = true;
	return true;

}


bool TDialog::Initialize2D(int & er)
{
	if (!fact.Get())
	{
		er = 1;
		return false;
	}

	if (!windowHandle)
	{
		er = 2;
		return false;
	}
	
	TrecComPointer<ID2D1DCRenderTarget>::TrecComHolder tempDC;
	HRESULT hr = fact->CreateDCRenderTarget(&(D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	)), tempDC.GetPointerAddress());
	ASSERT(SUCCEEDED(hr));

	regRenderTarget = TrecPointerKey::GetComPointer<ID2D1RenderTarget, ID2D1DCRenderTarget>(tempDC);
	rt_type = render_target_dc;

	HDC hdc = GetDC(windowHandle);
	//UINT boundResult = cdc->GetBoundsRect(&area, 0);
	GetClientRect(windowHandle, &area);

	dynamic_cast<ID2D1DCRenderTarget*>(regRenderTarget.Get())->BindDC(hdc, &area);

	return true;
}







int TDialog::DoModal()
{
	if (!isReady)
		return -1;
	CWnd* parentWindow = AfxGetMainWnd();

	if (!parentWindow)
		return -1;

	if (parentWindow && parentWindow->IsWindowEnabled())
	{
		parentWindow->EnableWindow(false);
	}
	
	int result = Create();
	
	parentWindow->EnableWindow(true);
	return result;
}

int TDialog::Create()
{
	if (!isReady)
		return -1;

	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);

	MSG message;

	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}


	return (int) statusValue;
}

const HWND TDialog::getWindowHandle()
{
	return windowHandle;
}

void TDialog::onButtonDown(bool isLeft, LPARAM lp)
{
	TPoint point;
	point.x = GET_X_LPARAM(lp);
	point.y = GET_Y_LPARAM(lp);
	if (pointer.Get())
	{
		messageOutput mOut = negative;
		if (isLeft)
			pointer->OnLButtonDown(0, point, &mOut, EventCred);
		else
			pointer->OnRButtonDown(0, point, &mOut, EventCred);

		if (mOut == negativeUpdate || mOut == positiveContinueUpdate || mOut == positiveOverrideUpdate)
			onDraw();
	}
	ResetComboBoxes();
	MessageHandler();
}

void TDialog::onButtonUp(bool isLeft, LPARAM lp)
{
	TPoint point;
	point.x = GET_X_LPARAM(lp);
	point.y = GET_Y_LPARAM(lp);
	if (pointer.Get())
	{
		messageOutput mOut = negative;
		if (isLeft)
			pointer->OnLButtonUp(0, point, &mOut, EventCred);
		else
			pointer->OnRButtonUp(0, point, &mOut, EventCred);

		if (mOut == negativeUpdate || mOut == positiveContinueUpdate || mOut == positiveOverrideUpdate)
			onDraw();
	}
	MessageHandler();
}

void TDialog::onMouseMove(LPARAM lp)
{
	TPoint point;
	point.x = GET_X_LPARAM(lp);
	point.y = GET_Y_LPARAM(lp);
	if (pointer.Get())
	{
		messageOutput mOut = negative;
		pointer->OnMouseMove(0, point, &mOut, EventCred);

		if (mOut == negativeUpdate || mOut == positiveContinueUpdate || mOut == positiveOverrideUpdate)
			onDraw();
	}
	MessageHandler();
}

void TDialog::onDraw()
{
	if (!pointer.Get())
		return;

	if (rt_type == render_target_unknown) return;

	if (rt_type == render_target_dc)
	{
		//renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		regRenderTarget->BeginDraw();
		regRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		pointer->onDraw();
		DrawActiveComboBox(regRenderTarget.Get());
		HRESULT res = regRenderTarget->EndDraw();
		int e = 3;
	}
	else if (rt_type == render_target_device_context)
	{
		ID2D1DeviceContext* contextDevice = dynamic_cast<ID2D1DeviceContext*>(regRenderTarget.Get());

		contextDevice->BeginDraw();
		if (engine.Get())
			engine->PrepareScene(D2D1::ColorF(D2D1::ColorF::White));
		if (firstDraw)
		{
			contextDevice->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));
			firstDraw = false;
		}
		pointer->onDraw();
		DrawActiveComboBox(contextDevice);

		HDC contDC = 0;
		if (gdiRender.Get() && SUCCEEDED(gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
		{
			HDC windDC = GetDC(windowHandle);
			if (windDC)
			{
				SelectObject(windDC, GetStockObject(DC_BRUSH));
				SetDCBrushColor(windDC, RGB(255, 255, 255));
			}
			if (engine.Get())
				engine->FinalizeScene();
			RECT winLoc;
			if (GetWindowRect(windowHandle, &winLoc))
				InvertRect(contDC, &winLoc);
			if (windDC)
			{
				if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, MERGEPAINT))
				{
					int er = GetLastError();
				}
			}

			gdiRender->ReleaseDC(nullptr);
		}
		HRESULT res = contextDevice->EndDraw();
	}
}

void TDialog::onChar(LPARAM lp, WPARAM wp)
{
	//WCHAR ch = static_cast<WCHAR>(wp);
	short times = static_cast<short>(lp);

	if (pointer.Get())
	{
		messageOutput mOut = negative;
		pointer->OnChar(true, wp, times, 0, &mOut, EventCred);

		if (mOut == negativeUpdate || mOut == positiveContinueUpdate || mOut == positiveOverrideUpdate)
			onDraw();
	}
	MessageHandler();
}

UCHAR * TDialog::GetAnaGameType()
{
	return nullptr;
}

void TDialog::ShutdownWindow()
{
	if(windowFeatures.hInstance)
		UnregisterClass(name.GetConstantBuffer(), windowFeatures.hInstance);
	DestroyWindow(windowHandle);
}

// Use this as A template
void TDialog::MessageHandler()
{
	/*
	
	
	*/
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < EventCred.Size();c++)
	{
		tc = EventCred[c].control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id != -1)
		{
			// call method
		}
	}


	EventCred.RemoveAll();
}
