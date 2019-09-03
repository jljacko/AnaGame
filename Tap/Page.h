#pragma once
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>

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
public:
	~Page();

	static TrecPointer<Page> Get2DPage(TrecComPointer<ID2D1Factory1>, HDC);
	static TrecPointer<Page> GetWindowPage(TrecComPointer<ID2D1Factory1>, HWND);
	static TrecPointer<Page> Get3DPage(TrecComPointer<ID2D1Factory1>, TrecPointer<ArenaEngine>);

	int SetAnaface(TrecPointer<TFile> file);

	virtual UCHAR* GetAnaGameType()override;
	afx_msg void OnSize(UINT nType, int cx,	int cy);

	void Draw();
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

	D2D1_MATRIX_3X2_F adjustMatrix;
};

