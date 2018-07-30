#pragma once
#include <ArenaEngine.h>
#include <ArenaModel.h>
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
class _ANAFACE_DLL TArena : public TControl
{
public:
	TArena(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND h, CAMERA_TYPE lookTo = LOOK_TO);
	~TArena();

	bool onCreate(RECT) override;
	TrecPointer<ArenaEngine> getEngine();
	bool setEngine(TrecPointer<ArenaEngine> e);
	void removeEngine();

	void onDraw() override;

	void Rotate(float leftRight, float upDown);
	void Translate(float degree, DirectX::XMFLOAT3 direction);

	void ChangeToLookAt();
	void ChangeToLookAt(ArenaModel& am, float distance, bool changeAngle);
	void ChangeToLookTo();
	void ChangeToLookTo(float x, float y, float z);

	virtual UCHAR* GetAnaGameType()override;

	void UpdatePos(float f, UCHAR dir);
	void UpdateDir(float f, UCHAR dir);

	void Resize(RECT r) override;

	DirectX::XMFLOAT3 GetCameraLocation();
	DirectX::XMFLOAT3 GetCameraDirection();

private:
	TrecPointer<ArenaEngine> arenaEngine;
	HWND windowHandle;
	HINSTANCE instanceHandle;

	void RefreshVectors();
	void UpdateCamera();

	// View Attributes
	D3D11_VIEWPORT *viewport;
	DirectX::XMMATRIX projector;

	//float rotX, rotY;

	// Since this effectively holds the camera, attributes for the camera matrix
	CAMERA_TYPE cameraType;
	DirectX::XMMATRIX camera; // camera matrix (view in model-view-projection)
	bool lookTo;              // use a look to if true, a look at if false
	DirectX::XMFLOAT3 location_3, direction_3, up_3;
	DirectX::XMVECTOR location, direction, up;

};

