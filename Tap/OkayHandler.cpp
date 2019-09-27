#include "OkayHandler.h"
#include "Page.h"

OkayHandler::OkayHandler(TrecPointer<TInstance> instance): EventHandler(instance)
{
	eventNameID enid;

	enid.eventID = 0;
	enid.name.Set(L"OnOkay");
	events.push_back(enid);

}

OkayHandler::~OkayHandler()
{
}

void OkayHandler::Initialize(TrecPointer<Page> page)
{
	assert(page.Get());
	this->page = page;
	instance = page->GetInstance();
}

void OkayHandler::HandleEvents(TDataArray<EventID_Cred>& eventAr)
{
	bool markDestroy = false; EventArgs ea;
	for (UINT Rust = 0; Rust < eventAr.Size(); Rust++)
	{
		TControl* cont = eventAr[Rust].control;
		if (!cont) continue;

		ea = cont->getEventArgs();

		if (ea.methodID == 0)
		{
			markDestroy = true;
		}
	}

	if (markDestroy)
		OnOkay(nullptr, ea);
}

void OkayHandler::OnOkay(TControl* control, EventArgs ea)
{
	if (!page.Get())
		throw L"Error! Handler expected pointer to a page!";
	HWND windHandle = page->GetWindowHandle();
	if (!windHandle)
		throw L"Error! Handler's Page Object returned a NULL window handle";

	DestroyWindow(windHandle);
}
