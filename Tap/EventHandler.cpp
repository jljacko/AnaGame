#include "EventHandler.h"
#include "TInstance.h"
#include <DirectoryInterface.h>

EventHandler::EventHandler(TrecPointer<TInstance> instance)
{
	app = instance;
}

EventHandler::EventHandler(TrecPointer<TInstance> instance, const TString& name)
{
	app = instance;
	this->name.Set(name);
}

EventHandler::~EventHandler()
{

}

bool EventHandler::ShouldProcessMessage(TrecPointer<HandlerMessage> message)
{
	if(!message.Get())
		return false;

	switch (message->GetMessageTransmission())
	{
	case message_transmission::message_transmission_firm_id:
		return message->GetHandlerIdType() == id;
	case message_transmission::message_transmission_firm_name:
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission::message_transmission_id_over_name:
		if (message->GetHandlerIdType() == id)
			return true;
		return name.GetSize() && !name.Compare(message->GetHandlerName());
	case message_transmission::message_transmission_name_over_id:
		if (name.GetSize() && !name.Compare(message->GetHandlerName()))
			return true;
		return message->GetHandlerIdType() == id;
	case message_transmission::message_transmission_by_type:
		return this->ShouldProcessMessageByType(message);
	case message_transmission::message_transmission_name_type:
		return this->ShouldProcessMessageByType(message) && !name.Compare(message->GetHandlerName());
	}
	return true;
}

bool EventHandler::OnDestroy()
{
    return true;
}

void EventHandler::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut)
{
}

void EventHandler::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

void EventHandler::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut)
{
}

bool EventHandler::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut)
{
	return false;
}

void EventHandler::OnResize(D2D1_RECT_F newSize)
{
}

TDataArray<eventNameID>& EventHandler::GetEventNameList()
{
	return events;
}

void EventHandler::Draw()
{
}

void EventHandler::SetSelf(TrecPointer<EventHandler> handleSelf)
{
	if (handleSelf.Get() != this)
		throw L"Error! Needs to be set to Self";
	hSelf = TrecPointerKey::GetSoftPointerFromTrec<EventHandler>(handleSelf);
	if (app.Get())
		app->RegisterHandler(handleSelf);
}

UINT EventHandler::GetId()
{
	return id;
}

TrecPointer<Page> EventHandler::GetPage()
{
	return page;
}

void EventHandler::SetMiniApp(TrecPointer<MiniApp> mApp)
{
	miniApp = mApp;
}

void EventHandler::OnFocus()
{
	if (!page.Get() || !page->GetWindowHandle().Get())
	{
		return;
	}

	if (miniApp.Get())
	{
		auto win = TrecPointerKey::GetTrecSubPointerFromTrec<TWindow, TIdeWindow>(page->GetWindowHandle());
		if (win.Get())
			win->SetCurrentApp(miniApp);
	}
}

void EventHandler::OnSave()
{
}

void EventHandler::SetSaveFile()
{
	if (filePointer.Get())
		return;

	if (!page.Get() || !page->GetWindowHandle().Get())
		return;

	auto win = page->GetWindowHandle();

	TString initialSearch(GetDirectory(CentralDirectories::cd_Documents));


	OPENFILENAMEW fileInfo;
	ZeroMemory(&fileInfo, sizeof(fileInfo));

	fileInfo.lStructSize = sizeof(OPENFILENAMEW);
	fileInfo.hwndOwner = win->GetWindowHandle();
	fileInfo.hInstance = win->GetInstance()->GetInstanceHandle();
	fileInfo.lpstrFilter = nullptr;
	fileInfo.lpstrInitialDir = initialSearch.GetConstantBuffer();
	fileInfo.lpstrFile = new WCHAR[255];
	fileInfo.nMaxFile = 230;

	bool gotName = false;
	if (gotName = GetSaveFileNameW(&fileInfo))
	{
		filePointer = TFileShell::GetFileInfo(TString(fileInfo.lpstrFile));
	}

	delete[] fileInfo.lpstrFile;
	if (!gotName) return;
	
}
