#include "TWindow.h"
#include "TInstance.h"

TWindow::TWindow(TString& name, TString& winClass, UINT style, HWND parent, int commandShow, TrecPointer<TInstance> ins)
{
	currentWindow = CreateWindowW(winClass.GetConstantBuffer(),
		name.GetConstantBuffer(), style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, nullptr, ins->GetInstanceHandle(), nullptr);

	if (!currentWindow)
	{
		int lastError = GetLastError();
		throw L"Window ERROR! Failed to generate Window";
	}
	this->name.Set(name);
	this->parent = parent;
	this->winClass.Set(winClass);
	this->windowInstance = ins;
	this->command = commandShow;

	HDC dc = GetWindowDC(currentWindow);
	SetMapMode(dc, MM_LOENGLISH);

	locked = false;
	safeToDraw = 0;
}

TWindow::~TWindow()
{
	CleanUp();
	mainPage.Delete();

	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
		pages[Rust].Delete();

	d3dEngine.Delete();
}

int TWindow::PrepareWindow()
{
	if (!currentWindow)
		return 1;

	assert(windowInstance.Get());

	windowInstance->RegisterDialog(TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	ShowWindow(currentWindow, command);
	UpdateWindow(currentWindow);



	return 0;
}

int TWindow::CompileView(TString& file, TrecPointer<EventHandler> eh)
{
	if (!windowInstance.Get())
		return -1;
	if (!currentWindow)
		return -2;
	TrecPointer<TFile> aFile = TrecPointerKey::GetNewTrecPointer<TFile>(file, TFile::t_file_read | TFile::t_file_share_read | TFile::t_file_open_always);

	if (!aFile.Get() || !aFile->IsOpen())
		return 1;

	directFactory = windowInstance->GetFactory();

	mainPage = Page::GetWindowPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), eh);

	if (!mainPage.Get())
		return 2;

	mainPage->SetAnaface(aFile, eh);
	Draw();

	return 0;
}

bool TWindow::MovePageToTop(TrecPointer<Page> p)
{
	if (!p.Get())
		return false;

	if (!pages.Size())
		return false;

	if (pages[0].Get() == p.Get())
		return true;

	UINT loc = 0;
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get() == p.Get())
		{
			loc = Rust;
			break;
		}
	}
	if (!loc)
		return false;

	for (int assign = loc; assign - 1 > -1; assign--)
	{
		pages[assign] = pages[assign - 1];
	}
	pages[0] = p;
	return true;
}

TString TWindow::GetWinClass()
{
	return winClass;
}

TString TWindow::GetWinName()
{
	return name;
}

void TWindow::Draw()
{
	if (mainPage.Get())
	{
		TrecComPointer<ID2D1RenderTarget> rt = mainPage->GetRenderTarget();
		
		TWindowEngine* d3d = d3dEngine.Get();
		if (!rt.Get()) return;

		if (mainPage->GetType() == render_target_device_context && d3d)
		{
			rt->BeginDraw();
			//rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
			d3d->PrepareScene(D2D1::ColorF(D2D1::ColorF::Wheat));

			mainPage->Draw();
			HDC contDC = 0;
			ID2D1GdiInteropRenderTarget* gdiRender = mainPage->GetGDIRenderTarget().Get();
			if (gdiRender && SUCCEEDED(gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
			{
				HDC windDC = GetDC(currentWindow);
				SelectObject(windDC, GetStockObject(DC_BRUSH));
				SetDCBrushColor(windDC, RGB(0, 0, 0));

				
				RECT loc = mainPage->GetArea();

				int width = loc.right - loc.left, height = loc.bottom - loc.top;

				//InvertRect(contDC, &loc);
				int err = 0;
				if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, SRCCOPY))
					err = GetLastError();
				gdiRender->ReleaseDC(nullptr);
				d3d->FinalizeScene();
				
			}
			rt->EndDraw();
		}
		else
		{
			rt->BeginDraw();
			rt->Clear(D2D1::ColorF(D2D1::ColorF::White));
			mainPage->Draw(d3d);
			rt->EndDraw();
		}

	}
}

void TWindow::Draw(Page& draw)
{
	draw.Draw();
}

void TWindow::OnRButtonUp(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonUp(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnRButtonUp(nFlags, point, &mOut);
}

void TWindow::OnLButtonDown(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDown(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonDown(nFlags, point, &mOut);
}

void TWindow::OnRButtonDown(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnRButtonDown(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnRButtonDown(nFlags, point, &mOut);
}
void TWindow::OnMouseMove(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnMouseMove(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnMouseMove(nFlags, point, &mOut);
}

void TWindow::OnLButtonDblClk(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonDblClk(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonDblClk(nFlags, point, &mOut);
}

void TWindow::OnLButtonUp(UINT nFlags, TPoint point)
{
	if (locked) return;
	messageOutput mOut = negative;
	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			pages[c]->OnLButtonUp(nFlags, point, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		mainPage->OnLButtonUp(nFlags, point, &mOut);
}

bool TWindow::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (locked) return false;
	messageOutput mOut = negative;

	bool returnable = false;

	for(UINT c = 0; c < pages.Size() && (mOut == negative || mOut == negativeUpdate); c++)
	{
		if(pages[c].Get())
			returnable = pages[c]->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);
	}

	if(mOut == negative || mOut == negativeUpdate)
		returnable = mainPage->OnChar(fromChar, nChar, nRepCnt, nFlags, &mOut);
	return returnable;
}

void TWindow::OnWindowResize(UINT width, UINT height)
{
	if (!mainPage.Get())
		return;

	//safeToDraw = safeToDraw | 0b00000010;

	RECT newLoc;
	newLoc.top = newLoc.left = 0;
	newLoc.bottom = height;
	newLoc.right = width;

	if (d3dEngine.Get())
	{
		mainPage->Clean3D();
		d3dEngine->Resize();
	}

	mainPage->OnResize(newLoc, 0, d3dEngine);

	//safeToDraw = safeToDraw & 0b11111101;
}

bool TWindow::OnDestroy()
{
	if(mainPage.Get())
		return mainPage->OnDestroy();
	return true;
}

TrecPointer<Page> TWindow::GetHandlePage(bool singleton)
{
	if (singleton && handlePage.Get())
		return handlePage;

	TrecPointer<Page> ret = Page::Get2DPage(windowInstance, TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self), TrecPointer<EventHandler>());

	if (!ret.Get())
		return ret;

	pages.push_back(ret);

	if (singleton)
		handlePage = ret;

	return ret;
}

TrecPointer<Page> TWindow::GetHandlePage(const TString& name)
{
	TrecPointer<Page> ret = keyPages.retrieveEntry(name);
	if (ret.Get())
		return ret;

	ret = GetHandlePage(false);

	if (ret.Get())
		keyPages.addEntry(name, ret);

	return ret;
}

TrecPointer<Page> TWindow::Get3DPage(bool singleton, TString& engineId)
{
	if(!engineId.GetSize() && singleton)
		return _3DPage;



	if (!engineId.GetSize())
		return TrecPointer<Page>();

	if (!d3dEngine.Get())
	{
		if (!SetUp3D())return TrecPointer<Page>();
	}
	TrecPointer<TArenaEngine> engine = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine, engineId);//  ArenaEngine::GetArenaEngine(engineId, currentWindow, windowInstance->GetInstanceHandle());

	return Get3DPage(singleton, engine);
}

TrecPointer<Page> TWindow::Get3DPage(bool singleton, TrecPointer<TArenaEngine> engine)
{
	if (singleton)
	{
		if (!engine.Get())
			return _3DPage;
		if (_3DPage.Get()) {
			if(_3DPage->GetArenaEngine().Get() == engine.Get())
				return _3DPage;
			return TrecPointer<Page>();
		}
	}

	//TrecPointer<Page> ret = Page::Get3DPage(windowInstance, engine, TrecPointer<EventHandler>());

	//pages.push_back(ret);

	//if (singleton)
	//{
		//_3DPage = ret;
	//}

	return TrecPointer<Page>();//ret;
}

void TWindow::LockWindow()
{
	locked = true;
}

void TWindow::UnlockWindow()
{
	locked = false;
}

void TWindow::SetSelf(TrecPointer<TWindow> win)
{
	if (this != win.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	this->self = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(win);
}

TrecPointer<Page> TWindow::GetPageByArea(RECT r)
{
	for (UINT Rust = 0; Rust < pages.Size(); Rust++)
	{
		if (pages[Rust].Get() && pages[Rust]->GetArea() == r)
			return pages[Rust];
	}
	TrecPointer<Page> ret = Page::GetSmallPage(mainPage, r);
	pages.push_back(ret);
	return ret;
}

TrecPointer<TInstance> TWindow::GetInstance()
{
	return windowInstance;
}

bool TWindow::SetUp3D()
{
	if (d3dEngine.Get())
		return true;

	d3dEngine = TrecPointerKey::GetNewTrecPointer<TWindowEngine>(currentWindow, windowInstance->GetInstanceHandle());

	if (d3dEngine->Initialize())
	{
		d3dEngine.Delete();
		return false;
	}

	TrecPointer<Page> newPage = Page::Get3DPage(windowInstance, d3dEngine, TrecPointer<EventHandler>(), TrecPointerKey::GetTrecPointerFromSoft<TWindow>(self));

	if (newPage.Get())
	{
		newPage->SetAnaface(mainPage->ExtractRootControl());
		if(mainPage.Get())
			newPage->SetHandler(mainPage->GetHandler());
		deletePage = mainPage;
		mainPage = newPage;
		return true;
	}

	d3dEngine.Delete();
	return false;
}

void TWindow::CleanUp()
{
	deletePage.Delete();
}

TrecPointer<TWindowEngine> TWindow::GetWindowEngine()
{
	return d3dEngine;
}

TrecPointer<TArenaEngine> TWindow::GetNewArenaEngine()
{
	TrecPointer<TArenaEngine> ret;
	if (!d3dEngine.Get())
		return ret;

	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine);
	engines.push_back(ret);
	return ret;
}

TrecPointer<TArenaEngine> TWindow::GetNewArenaEngine(TString& name)
{
	TrecPointer<TArenaEngine> ret;
	if (!d3dEngine.Get())
		return ret;
	
	ret = TrecPointerKey::GetNewTrecPointer<TArenaEngine>(d3dEngine, name);
	engines.push_back(ret);
	return ret;
}

TrecComPointer<ID2D1RenderTarget> TWindow::GetRenderTarget()
{
	return TrecComPointer<ID2D1RenderTarget>();
}

HWND TWindow::GetWindowHandle()
{
	return currentWindow;
}
