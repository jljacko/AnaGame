#pragma once
#include "Tap_dll.h"
#include <AnafaceParser.h>
#include "HandlerMessage.h"

class Page;
class TInstance;
class MiniApp;

/**
 * Class: EventHandler
 * Purpose: Base Class for Handling Events generated by a TControl
 */
class _TAP_DLL EventHandler
{
	friend class TInstance;
public:

	/**
	 * Method: EventHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 * Returns: New EventHandler Object
	 */
    EventHandler(TrecPointer<TInstance> instance);
	/**
	 * Method: EventHandler::EventHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - instance associated with this handler
	 *				const TString& name - the Name to give this handler
	 * Returns: New EventHandler Object
	 */
	EventHandler(TrecPointer<TInstance> instance, const TString& name);
	/**
	 * Method: EventHandler::~EventHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
    virtual ~EventHandler();

	/**
	 * Method: EventHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
    virtual void Initialize(TrecPointer<Page> page) = 0;
	/**
	 * Method: EventHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
    virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr) = 0;
	/**
	 * Method: EventHandler::ShouldProcessMessage
	 * Purpose: Reports whether the Message is to be processed by this handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to process
	 * Returns: bool - whether this is the handler for the submitted message
	 */
	bool ShouldProcessMessage(TrecPointer<HandlerMessage> message);

	/**
	 * Method: EventHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message) = 0;


	/**
	 * Method: EventHandler::OnDestroy
	 * Purpose: Reports whether the Handler is ready to be destroyed
	 * Parameters: void
	 * Returns: bool - true
	 */
    virtual bool OnDestroy();

	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut);
	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	afx_msg virtual void OnResize(D2D1_RECT_F newSize);

	/**
	 * Method: EventHandler::GetEventNameList
	 * Purpose: Provides the list of event names and their associated IDs
	 * Parameters: void 
	 * Returns: TDataArray<eventNameID>& - list of event names and ids
	 */
	TDataArray<eventNameID>& GetEventNameList();

	/**
	 * Method: EventHandler::
	 * Purpose:
	 * Parameters:
	 * Returns:
	 *
	 * Note: DEPRICATED - should be handled by Instance, Window, Page and TControls, not by the Handler
	 */
	virtual void Draw();

	/**
	 * Method: EventHandler::SetSelf
	 * Purpose: Allows the Event Handler to supply a reference to itself
	 * Parameters: TrecPointer<EventHandler> handleSelf - the reference generated by the TrecPointerKey
	 * Returns: void
	 */
	virtual void SetSelf(TrecPointer<EventHandler> handleSelf);
	/**
	 * Method: EventHandler::GetId
	 * Purpose: Retirves the ID of this Handler
	 * Parameters: void
	 * Returns: UINT - the id of the Handler
	 *
	 * Note: id is not currently set, so this is a redundant method for the time being
	 */
	UINT GetId();

	/**
	 * Method: EventHandler::GetPage
	 * Purpose: Retrieves the Page associated with this Handler
	 * Parameters: void 
	 * Returns: TrecPointer<Page> - the page provided when Initialize was called
	 */
	TrecPointer<Page> GetPage();

	// Mini App related methods


	/**
	 * Method: EventHandler::SetMiniApp
	 * Purpose: Sets the MiniApp associted with this Handler
	 * Parameters: TrecPointer<MiniApp> mApp - the miniApp that called for this Handler
	 * Returns: void
	 */
	void SetMiniApp(TrecPointer<MiniApp> mApp);
	/**
	 * Method: EventHandler::OnFocus
	 * Purpose: Lets the handler know that this is the current focus of the User. Used to Set the Main MiniApp of the
	 *		IDE Window, if one is set
	 * Parameters: void
	 * Returns: void
	 */
	void OnFocus();
	/**
	 * Method: EventHandler::OnSave
	 * Purpose: Provides a Save method in case there is a way to save
	 * Parameters: void
	 * Returns: void
	 */
	virtual void OnSave();

protected:
	/**
	 * Reference to "this" Handler
	 */
	TrecPointerSoft<EventHandler> hSelf;

	/**
	 * Method: EventHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) = 0;

	/**
	 * The id of the Handler (not used yet)
	 */
	UINT id;

	/**
	 * The Name of the Handler for identification
	 */
	TString name;

	/**
	 * The instance associated with the Handler
	 */
	TrecPointer<TInstance> app;

	/**
	 * list of Events organized by Name and id
	 */
	TDataArray<eventNameID> events;

	/**
	 * The Page that the Handler is attached to
	 */
	TrecPointer<Page> page;

	/**
	 * The MiniApp that generated this handler (not always used
	 */
	TrecPointer<MiniApp> miniApp;

	// Resources for Saving

	/**
	 * The File attached to this Handler (if applicable)
	 */
	TrecPointer<TFileShell> filePointer;

	/**
	 * Method: EventHandler::SetSaveFile
	 * Purpose: Sets up the file to save if OnSave is called
	 * Parameters: void
	 * Returns: void
	 */
	void SetSaveFile();
};