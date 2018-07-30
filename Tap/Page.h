#pragma once
#include "stdafx.h"
#include "Tap_dll.h"
#include <AnafaceUI.h>
#include <TObject.h>


/* Class: Page
* Purpose: Provide the foundation through which Drawing resources can be set up on a certain Window or device context,
*	Allows for refactoring of code that is essentially repeated
*/
class _TAP_DLL Page : public TObject
{
public:
	//Page();
	Page(HWND window);
	~Page();

	int Initialize2D(CDC*);
	//int Initialize2D();
	int Initialize3D(TString& engine);
	int Initialize3D(TString& engine, CDC*);

	virtual UCHAR* GetAnaGameType()override;
	afx_msg void OnSize(UINT nType, int cx,	int cy);
protected:
	Page();
	bool Initialize2D(HDC);

	// Common resources
	TrecComPointer<ID2D1RenderTarget> regRenderTarget; // The render target which will be used
	HWND windowHandle; // the Window attached to the page
	HINSTANCE instance;// the Inatance that was used during page construction
	int width, height; // The Current Dimentions of the page
	HDC deviceH;


	// 2D - only Resources
	CDC* cdc;  // The Device context to use
	TrecComPointer<ID2D1DCRenderTarget> renderTarget;
	RECT area;

	// 3D mixing with 2D Resources
	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1DeviceContext> contextDevice;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;
	TrecComPointer<ID2D1Bitmap1> bit;
	TrecComPointer<ID2D1Factory1> fact;
	TrecPointer<ArenaEngine> engine;
};

