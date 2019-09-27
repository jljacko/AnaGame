#pragma once
#include "Tap_dll.h"
#include <AnafaceParser.h>

class Page;
class TInstance;

class _TAP_DLL EventHandler
{
public:
    EventHandler(TrecPointer<TInstance> instance);
    virtual ~EventHandler();

    virtual void Initialize(TrecPointer<Page> page) = 0;
    virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) = 0;
    virtual bool OnDestroy();

	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut);

	TDataArray<eventNameID>& GetEventNameList();

	virtual void Draw();

protected:
	TrecPointer<TInstance> app;
	TDataArray<eventNameID> events;
	TrecPointer<TInstance> instance;
	TrecPointer<Page> page;
};