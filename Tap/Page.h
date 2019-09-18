#pragma once
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>
#include "EventHandler.h"

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

	static TrecPointer<Page> Get2DPage(TrecComPointer<ID2D1Factory1>, HDC, TrecPointer<EventHandler>);
	static TrecPointer<Page> GetWindowPage(TrecComPointer<ID2D1Factory1>, HWND,  TrecPointer<EventHandler>);
	static TrecPointer<Page> GetWindowPage(TrecComPointer<ID2D1RenderTarget>, HWND, TrecPointer<EventHandler>);
	static TrecPointer<Page> Get3DPage(TrecComPointer<ID2D1Factory1>, TrecPointer<ArenaEngine>, TrecPointer<EventHandler>);

	int SetAnaface(TrecPointer<TFile> file, TrecPointer<EventHandler> eh);
	int SetAnaface(TrecPointer<TFile> file, TDataArray<eventNameID>& id);

	TrecPointer<TControl> GetRootControl();

	HWND GetWindowHandle();

	virtual UCHAR* GetAnaGameType()override;
	afx_msg void OnSize(UINT nType, int cx,	int cy);

	TrecComPointer<ID2D1RenderTarget> GetRenderTarget();
	TrecPointer<ArenaEngine> GetArenaEngine();

	void CreateLayout();

	void Draw();

	void SetArea(RECT& loc);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut);

	afx_msg void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnDestroy();
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

	// Regular Resources
	HWND windowHandle;		// the Window attached to the page
	HINSTANCE instance;		// the Inatance that was used during page construction
	HDC deviceH;			// Handle to the Context Device used by the Window
	RECT area;				// Area Within the Window to draw

	// Anagame Specific Resources
	TrecPointer<ArenaEngine> engine;	// Incase we are using 3D Resources
	TrecPointer<TControl> rootControl;	// The Control to Draw when
	 TrecPointer<EventHandler> handler; // The class that manages specific actions
	D2D1_MATRIX_3X2_F adjustMatrix;

	// Anaface Resources
	TDataArray<TControl*> clickedControl;
};

