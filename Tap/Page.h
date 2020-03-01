#pragma once
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>
#include "EventHandler.h"
//#include "MiniHandler.h"
#include <TWindowEngine.h>
#include <DrawingBoard.h>

class TWindow;

class MiniHandler;
class TAnimationManager;

typedef enum RenderTargetType
{
	render_target_unknown,
	render_target_device_context,
	render_target_hwnd,
	render_target_dc,
	render_target_dxgi
}RenderTargetType;

/* Class: Page
* Purpose: Provide the foundation through which Drawing resources can be set up on a certain Window or device context,
*	Allows for refactoring of code that is essentially repeated
*/
class _TAP_DLL Page : public TObject
{
	friend class TrecPointerKey;
public:
	virtual ~Page();


	static TrecPointer<Page> GetWindowPage(TrecPointer<TInstance>, TrecPointer<TWindow>,  TrecPointer<EventHandler>);


	static TrecPointer<Page> GetSmallPage(TrecPointer<TInstance> in, TrecPointer<TWindow> window, D2D1_RECT_F area);

	int SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh);
	int SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id);

	void SetAnaface(TrecPointer<TControl> newRoot);

	TrecPointer<TControl> GetRootControl();
	TrecPointer<TControl> ExtractRootControl();

	TrecPointer<TWindow> GetWindowHandle();

	TrecPointer<EventHandler> GetHandler();

	void SetHandler(TrecPointer<EventHandler> eh);

	virtual UCHAR* GetAnaGameType()override;
	afx_msg void OnSize(UINT nType, int cx,	int cy);

	TrecPointer<TArenaEngine> GetArenaEngine();

	void CreateLayout();

	virtual void Draw(TWindowEngine* twe = nullptr);

	D2D1_RECT_F GetArea();
	void SetArea(const D2D1_RECT_F& loc);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut);
	afx_msg void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TrecPointer<TWindowEngine>);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnDestroy();

	void SetSelf(TrecPointer<Page>);
	TrecPointer<TInstance> GetInstance();

	void PrepAnimations(TAnimationManager& aManager);

	void SetMiniHandler(TrecPointer<MiniHandler> mh);

protected:
	Page(TrecPointer<DrawingBoard>);

	TrecPointer<AnimationBuilder> GetAnimationByName(TString& name);

	float scale;

	TrecPointer<DrawingBoard> drawingBoard;
	TrecPointer<TBrush> solidBrush;

	// Regular Resources
	
	//HINSTANCE instance;		// the Inatance that was used during page construction
	HDC deviceH;			// Handle to the Context Device used by the Window
	D2D1_RECT_F area;				// Area Within the Window to draw

	// Anagame Specific Resources
	TrecPointer<TArenaEngine> engine;	// Incase we are using 3D Resources
	TrecPointer<TControl> rootControl;	// The Control to Draw when
	 TrecPointer<EventHandler> handler; // The class that manages specific actions
	TrecPointer<MiniHandler> miniHandler; // Intended for tabs whise contents have specific hanling specs
	 TrecPointer<TInstance> instance;
	D2D1_MATRIX_3X2_F adjustMatrix;
	TrecPointer<TWindow> windowHandle;		// the Window attached to the page

	TrecPointerSoft<Page> self;

	// Anaface Resources
	TDataArray<TControl*> clickedControl;


	// Animation Resources
	TDataArray<TrecPointer<AnimationBuilder>> animations;
	TDataArray<TString> basicStoryBoards;
	TDataArray<TString> persistentStoryBoards;


};

