// ModelDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "ModelDoc.h"
#include "NewObjectTDialog.h"
#include "EditObjectTDialog.h"
#include "MainFrm.h"

#include <TString.h>
#include <AnafaceParser.h>
#include <TML_Reader_.h>
// ModelDoc

static int counter = 0;

IMPLEMENT_DYNCREATE(ModelDoc, CDocument)

ModelDoc::ModelDoc()
{
	anafaceSet = false;

	counter++;
	CWnd* pWnd = AfxGetMainWnd();
	ASSERT_VALID(pWnd);
	windowHandle = pWnd->m_hWnd;
	enginePointer = ArenaEngine::GetArenaEngine(TString(L"ModelEngine"),windowHandle, AfxGetInstanceHandle());

	D2D1_FACTORY_OPTIONS factOpts;
	
	factOpts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	ID2D1Factory1* rawFact = nullptr;
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factOpts, &rawFact)))
		fact = nullptr;
	else
		fact = rawFact;

	IDXGISwapChain* swapper = enginePointer->getSwapChain().get();
	if (!swapper)
		return;
	IDXGISurface* surf = nullptr;
	HRESULT res = swapper->GetBuffer(0, IID_PPV_ARGS(&surf));
	if (FAILED(res))
	{
		//*error = -5;
		return;
	}

	IDXGIDevice* dev = enginePointer->getDeviceD_U().get();

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
		return;
	}

	ID2D1DeviceContext* cd = nullptr;

	res = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &cd);
	contextDevice = cd;
	if (FAILED(res))
	{
		device = nullptr;
		return;
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
		return ;
	}

	contextDevice->SetTarget(bit.get());
	ID2D1GdiInteropRenderTarget* gdiRender_raw = nullptr;
	res = contextDevice->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)&gdiRender_raw);
	gdiRender = gdiRender_raw;
	bit = nullptr;

	// Set up the Anaface
	CMainFrame* window = (CMainFrame*)AfxGetMainWnd();

	TString direct = window->getSpecialDirectory(bdi_user_appdata);

	for (UINT c = 0; c < direct.GetLength(); c++)
	{
		anafaceFile += direct[c];
	}

	anafaceFile += L"\\ModelDocAnaface.tml";
}
/*
void ModelDoc::Initialize(std::vector<DirectX::XMFLOAT3>& x3, std::vector<int>& ind, DirectX::XMFLOAT4 color)
{
	if (modelPointer.get()) // using a smart pointer
	{
		modelPointer->PrepModelWithSupplies(x3, color);
		modelPointer->PrepIndicees(ind);
	}
}*/

BOOL ModelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

ModelDoc::~ModelDoc()
{
	pointer.Delete();

}


BEGIN_MESSAGE_MAP(ModelDoc, CDocument)
	ON_COMMAND(ID_BUTTON4, &ModelDoc::OnNew3DObject)
	ON_COMMAND(ID_BUTTON2, &ModelDoc::OnEdit3DModelCommand)
END_MESSAGE_MAP()


// ModelDoc diagnostics

#ifdef _DEBUG
void ModelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void ModelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
void ModelDoc::OnDraw(CDC *pDC)
{
	if (!anafaceSet)
	{
		if (!pDC)
			return;
		HDC hdc = pDC->GetSafeHdc();
		windowHandle = WindowFromDC(hdc);
		pDC->GetBoundsRect(&area,0);
		GetClientRect(windowHandle, &area);

		CFile file;
		if (!file.Open(anafaceFile, CFile::modeRead))
			return;
		// Now Create THe Anaface that lays it out
		TString directory = file.GetFilePath();
		TString fileName = file.GetFileName();
		int ind = directory.Find(fileName, 0);
		if (ind > 0)
			directory.Delete(ind, fileName.GetLength());

		AnafaceParser* aParse = new AnafaceParser(regRenderTarget, windowHandle, &directory);
		//aParse->setEventSystem(idMatch);

		CArchive* archer = new CArchive(&file, CArchive::load);

		TML_Reader_* reader = new TML_Reader_(archer, aParse);
		int readingError;
		if (!reader->read(&readingError))
		{
			
			return ;
		}
		else
		{
			TrecPointer<TControl> rootControl = aParse->getRootControl();
			if (rootControl.get())
			{
				pointer = rootControl;

				pointer->onCreate(area);
				try
				{
					arenaControl = dynamic_cast<TArena*>(pointer.get());
				}
				catch (std::bad_cast& e)
				{
					TRACE(e.what());
				}
			}
			else
			{
				
				return ;
			}
		}
		file.Close();
		anafaceSet = true;
	}

	UINT width = area.right - area.left;
	UINT height = area.bottom - area.top;

	if (!pointer.get())
		return;
	if (contextDevice.get())
	{
		contextDevice->BeginDraw();
		if (enginePointer.get())
			enginePointer->PrepareScene(D2D1::ColorF(D2D1::ColorF::White, 1.0f));
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
			if (enginePointer.get())
				enginePointer->FinalizeScene();
			//RECT winLoc;
			int er = 0;
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
					er = GetLastError();
			}
			
			if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, SRCCOPY))
			{
				er = GetLastError();
			}

			gdiRender->ReleaseDC(nullptr);
		}
		HRESULT res = contextDevice->EndDraw();
	}
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// ModelDoc serialization

void ModelDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// ModelDoc commands


void ModelDoc::OnNew3DObject()
{
	// TODO: Add your command handler code here
	TrecPointer<NewObjectTDialog> nObj = new NewObjectTDialog();
	if (!nObj.get())
		return;
	bool validC = nObj->getValidConstruction();
	if (!validC)
	{
		nObj = NULL;
		return;
	}

	TString str = L"Test Dialog";
	nObj->setName(str);

	int error = 0;
	nObj->InitializeWindow(error);


	if (!error)
	{
		if (nObj->Initialize3D(str))
			return;
		if (!nObj->InitializeEngine())
			return;
		nObj->InitializeAnaface(error);
		if (error)
			return;
		nObj->initializeControls();
		nObj->DoModal();

		TrecPointer<ArenaModel> mod = nObj->GetModel();

		if (mod.get() && enginePointer.get())
		{

			mod->SetNewEngine(*enginePointer.get());
			//engine
		}
	}

	


	nObj.Delete();
	this->UpdateAllViews(NULL);
}


void ModelDoc::OnEdit3DModelCommand()
{
	// TODO: Add your command handler code here
	TrecPointer<EditObjectTDialog> nObj = new EditObjectTDialog();
	if (!nObj.get())
		return;
	bool validC = nObj->getValidConstruction();
	if (!validC)
	{
		nObj = NULL;
		return;
	}

	TString str = L"Test Dialog";
	nObj->setName(str);

	int error = 0;
	nObj->InitializeWindow(error);
	

	if (!error)
	{
		if (nObj->Initialize3D(str))
			return;
		if (!nObj->InitializeEngine())
			return;
		nObj->InitializeAnaface(error);
		if (error)
			return;
		//nObj->initializeControls();
		nObj->DoModal();
	}



	//delete nObj;
	this->UpdateAllViews(NULL);
}
