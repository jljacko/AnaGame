#pragma once
#include "Tap_dll.h"
#include "TObject.h"
#include "stdafx.h"
#include <TString.h>
#include <AnafaceUI.h>
#include "AnafaceParser.h"
#include "Page.h"
#include <TrecReference.h>
typedef struct EventLine
{
	EventID_Cred eventID;
	unsigned int responseID;
}EventLine;


class _TAP_DLL TDialog : public Page
{
	friend class TDialog;
public:
	TDialog(int width, int height);
	~TDialog();

	bool setAnaface(TString& path);       // Sets the Anaface UI that will be targeted
	bool setName(TString&);             // Sets the name that will appear in the Top of the Window

	bool InitializeWindow(int& error);
	bool InitializeAnaface(int& error);
	bool Initialize2D(int& er);

	virtual int DoModal();          // Creates a New Window, similarly to how DoModal works on a CDialog
	virtual int Create();          // Creates a New Window, but allows User to continue with previous app


	const HWND getWindowHandle();

	void onButtonDown(bool isLeft, LPARAM lp);
	void onButtonUp(bool isLeft, LPARAM lp);
	void onMouseMove(LPARAM lp);
	virtual void onDraw();
	void onChar(LPARAM lp, WPARAM wp);

	virtual UCHAR* GetAnaGameType()override;

protected:
	//ID2D1DCRenderTarget* renderer;  // Direct 2D member that Anaface Uses
	TString name;                   // Name to attach to Window of Dialog box
	WNDCLASS windowFeatures;      // Structure to create the Window
	/*HWND windowHandle;
	HINSTANCE instance;
	int width, height;*/
	//HDC deviceH;
	bool isReady;
	bool firstDraw;
	ATOM at = 0;

	// Anaface Specific resources
	TrecPointer<TControl> pointer;
	TFile file;


	void ShutdownWindow();
	/*TrecComPointer<ID2D1Factory1> fact;
	// Regular Direct2D/Write method
	TrecComPointer<ID2D1DCRenderTarget> renderTarget;
	TrecComPointer<ID2D1RenderTarget> regRenderTarget;
	// Direct2D/Write and Direct3D
	TrecComPointer<ID2D1Device> device;
	TrecComPointer<ID2D1DeviceContext> contextDevice;
	TrecComPointer<ID2D1GdiInteropRenderTarget> gdiRender;*/
	TrecPointer<ArenaEngine> engine;
	int location;
	//RECT area;
	bool FileOpened;

	TDataArray<eventNameID> idMatch;

protected:
	unsigned int statusValue;
	TDataArray<EventID_Cred> EventCred;
	TDataArray<EventLine> EventLine;

	virtual void MessageHandler();
	TString filePath;
};

