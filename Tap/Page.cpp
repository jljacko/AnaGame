
#include "Page.h"
#include <AnafaceParser.h>
#include <TML_Reader_.h>
#include "TInstance.h"
#include "TWindow.h"
#include "MiniHandler.h"

Page::Page(TrecPointer<DrawingBoard> board)
{
	drawingBoard = board;
	deviceH = nullptr;
	adjustMatrix = D2D1::Matrix3x2F::Identity();
	area = D2D1_RECT_F{ 0,0,0,0 };
	scale = 1.0f;
}


Page::~Page()
{
	rootControl.Delete();
	// 3D mixing with 2D Resources
	solidBrush.Delete();
}


TrecPointer<Page> Page::GetWindowPage(TrecPointer<TInstance> in , TrecPointer<TWindow> window, TrecPointer<EventHandler> eh)
{
	if (!in.Get())
		throw L"Error! Instance Object MUST ne initialized";

	RECT area;

	GetClientRect(window->GetWindowHandle(), &area);

	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>(window->GetDrawingBoard());

	convertCRectToD2DRect(&area, &ret->area);
	ret->instance = in;
	ret->handler = eh;
	ret->windowHandle = window;
	ret->deviceH = GetWindowDC(window->GetWindowHandle());

	return ret;

}


TrecPointer<Page> Page::GetSmallPage(TrecPointer<TInstance> in, TrecPointer<TWindow> window, D2D1_RECT_F area)
{
	TrecPointer<Page> ret = TrecPointerKey::GetNewSelfTrecPointer<Page>(window->GetDrawingBoard());

	ret->area = area;


	ret->instance = in;
	ret->windowHandle = window;
	ret->deviceH = GetWindowDC(window->GetWindowHandle());
	return ret;
}

int Page::SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh)
{
	if (!file.Get())
		return -1;
	if (!file->IsOpen())
		return -2;
	AnafaceParser parser(drawingBoard, windowHandle->GetWindowHandle(), file->GetFileDirectory());

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
	AnafaceParser parser(drawingBoard, windowHandle->GetWindowHandle(), file->GetFileDirectory());

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
	rootControl = newRoot;

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

TrecPointer<EventHandler> Page::GetHandler()
{
	return handler;
}

void Page::SetHandler(TrecPointer <EventHandler> eh)
{
	handler = eh;
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
	bool returnable = false;
	if (miniHandler.Get())
		returnable = miniHandler->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut);
	if (handler.Get())
		handler->HandleEvents(eventAr);

	if(!returnable)
		returnable = OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);

	if (*mOut == negativeUpdate || *mOut == positiveContinueUpdate || *mOut == positiveOverrideUpdate)
		if (windowHandle.Get())windowHandle->Draw(); else return false;
	return returnable;
}

void Page::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine> engine)
{
	// Only Resize if basic resources are in order
	if (!instance.Get() || !windowHandle.Get())
		return;

	area = newLoc;

	if (rootControl.Get())
	{
		rootControl->Resize(area);
	}

	if (miniHandler.Get())
		miniHandler->OnResize(area);
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

	

	if (rootControl.Get())
		rootControl->OnLButtonUp(nFlags, point, mOut, eventAr);
	UINT curSize = clickedControl.Size();
	for (UINT c = 0; c < clickedControl.Size(); c++)
	{
		if (clickedControl[c])
			clickedControl[c]->SetNormalMouseState();
	}
	clickedControl.RemoveAll();
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

void Page::OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr)
{
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


	area.top = area.left = 0.0f;
	area.bottom = cy;
	area.right = cx;



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

void Page::Draw(TWindowEngine* twe)
{
	if (!rootControl.Get() || !drawingBoard.Get()) return;
	
	//drawingBoard->SetTransform(adjustMatrix);
	rootControl->onDraw();
	//regRenderTarget->EndDraw();

	if (handler.Get())
		handler->Draw();
}

D2D1_RECT_F Page::GetArea()
{
	return area;
}

void Page::SetArea(const D2D1_RECT_F& loc)
{
	area = loc;
}


