#pragma once
#include "Tap_dll.h"
#include <AnafaceParser.h>
#include "HandlerMessage.h"

class Page;
class TInstance;
class MiniApp;

class _TAP_DLL EventHandler
{
	friend class TInstance;
public:
    EventHandler(TrecPointer<TInstance> instance);
	EventHandler(TrecPointer<TInstance> instance, const TString& name);
    virtual ~EventHandler();

    virtual void Initialize(TrecPointer<Page> page) = 0;
    virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) = 0;
	bool ShouldProcessMessage(TrecPointer<HandlerMessage> message);

	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) = 0;


    virtual bool OnDestroy();

	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut);
	afx_msg virtual void OnResize(D2D1_RECT_F newSize);

	TDataArray<eventNameID>& GetEventNameList();

	virtual void Draw();

	virtual void SetSelf(TrecPointer<EventHandler> handleSelf);
	UINT GetId();

	TrecPointer<Page> GetPage();

	// Mini App related methods
	void SetMiniApp(TrecPointer<MiniApp> mApp);
	void OnFocus();
	virtual void OnSave();

protected:
	TrecPointerSoft<EventHandler> hSelf;
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) = 0;
	UINT id;
	TString name;
	TrecPointer<TInstance> app;
	TDataArray<eventNameID> events;
	TrecPointer<Page> page;

	TrecPointer<MiniApp> miniApp;

	// Resources for Saving
	TrecPointer<TFileShell> filePointer;

	void SetSaveFile();
};