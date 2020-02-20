#pragma once
#include <TCamera.h>
#include "TControl.h"

// Macros for managing Look At vs Look To camera
#define CAMERA_TYPE bool
#define LOOK_TO true
#define LOOK_AT false

#define IF_IS_LOOK_TO(camType)   if(camType)
#define IF_IS_LOOK_AT(camType)   if(!camType)

/*
* class TArena
* Purpose: Holds the Camera for view into a 3D Arena
*/
class _ANAFACE_DLL TArena : public TControl, public TCamera
{
public:
	TArena(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND h, CAMERA_TYPE lookTo = LOOK_TO);
	~TArena();

	bool onCreate(D2D1_RECT_F, TrecPointer<TWindowEngine> d3d) override;
	TrecPointer<TArenaEngine> getEngine();


	void onDraw(TObject* obj) override;

	virtual UCHAR* GetAnaGameType()override;

	void Resize(D2D1_RECT_F& r) override;


};

