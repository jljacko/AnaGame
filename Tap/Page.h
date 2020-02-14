#pragma once
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>
#include "EventHandler.h"
//#include "MiniHandler.h"
#include <TWindowEngine.h>

class TWindow;

class MiniHandler;

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
	~Page();

	static TrecPointer<Page> Get2DPage(TrecPointer<TInstance>, TrecPointer<TWindow>, TrecPointer<EventHandler>);
	static TrecPointer<Page> GetWindowPage(TrecPointer<TInstance>, TrecPointer<TWindow>,  TrecPointer<EventHandler>);
	static TrecPointer<Page> GetWindowPage(TrecPointer<TInstance>, TrecComPointer<ID2D1RenderTarget>, TrecPointer<TWindow>, TrecPointer<EventHandler>);
	static TrecPointer<Page> Get3DPage(TrecPointer<TInstance>, TrecPointer<TWindowEngine>, TrecPointer<EventHandler>, TrecPointer<TWindow> window);
	static TrecPointer<Page> GetSmallPage(TrecPointer<Page>, RECT area);

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

	TrecComPointer<ID2D1RenderTarget> GetRenderTarget();
	TrecPointer<TArenaEngine> GetArenaEngine();

	void CreateLayout();

	virtual void Draw(TWindowEngine* twe = nullptr);

	RECT GetArea();
	void SetArea(const RECT& loc);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut);
	afx_msg void OnResize(RECT& newLoc, UINT nFlags, TrecPointer<TWindowEngine>);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnResize(RECT& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnDestroy();

	void SetSelf(TrecPointer<Page>);
	TrecPointer<TInstance> GetInstance();

	void Clean3D();

	void SetMiniHandler(TrecPointer<MiniHandler> mh);
	RenderTargetType GetType();
	TrecComPointer<ID2D1GdiInteropRenderTarget> GetGDIRenderTarget();

protected:
	Page();

	float scale;

	// DirectX Resources
	TrecComPointer<ID2D1Factory1> fact;						// D2D Factory, provided by our AnaGame Instance Class
	RenderTargetType rt_type;								// Keeps track of the type of RenderTarget this Page is using
	TrecComPointer<ID2D1RenderTarget> regRenderTarget;		// The render target which will be used
	// 3D mixing with 2D Resources
	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;
	TrecComPointer<ID2D1Bitmap1> bit;
	D2D1_RECT_F dRect;
	TrecComPointer<ID2D1SolidColorBrush> clearBursh;

	// Regular Resources
	
	//HINSTANCE instance;		// the Inatance that was used during page construction
	HDC deviceH;			// Handle to the Context Device used by the Window
	RECT area;				// Area Within the Window to draw

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

	UINT SetUpDeviceContextTarget(TrecPointer<TWindowEngine> winEnginie);
	void SetUpHwndRenderTarget();
};

