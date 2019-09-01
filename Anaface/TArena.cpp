#include "stdafx.h"
#include "TArena.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

/*
* Method: TArena) (Constructor) 
* Purpose: Sets up the Arena
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> st - list of Anaface styles
*				HWND h - the window to attach to
*				CAMERA_TYPE type - type of camera to start as
* Returns: void
*/
TArena::TArena(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> st, HWND h, CAMERA_TYPE type):TControl(rt, st,false)
{
	cameraType = type;
	windowHandle = h;
	instanceHandle = AfxGetInstanceHandle();
	viewport = NULL;
	projector = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, 800.0f / 600.0f, 1.0f, 1000.0f);

	location_3 = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	//location = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	//rotX = rotY = 0.0f;

	direction_3 = DirectX::XMFLOAT3(0.001f, 0.0f, 0.0f);
	up_3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	RefreshVectors();
	//camera = DirectX::XMMatrixLookToLH(location, direction, up);
	lookTo = true;
}

/*
* Method: TArena) (Destructor)
* Purpose: Cleans up arena
* Parameters: void
* Returns: void
*/
TArena::~TArena()
{
	if (viewport)
		delete viewport;
}

/*
* Method: TArena - onCreate
* Purpose: Sets up Arena specific attributes
* Parameters: RECT r - the location on screen where arena is to show
* Returns: bool - ignore
*/
bool TArena::onCreate(RECT r )
{
	TControl::onCreate(r);

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|EngineID"));

	float aspect = static_cast<float>(r.right - r.left) / static_cast<float>(r.bottom - r.top);

	projector = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, aspect,1.0f, 1000.0f);
	

	if (valpoint.Get())
	{
		arenaEngine = ArenaEngine::GetArenaEngine(*(valpoint.Get()),windowHandle,AfxGetInstanceHandle());
		if (!arenaEngine.Get())
			return false;

		arenaEngine->initialize(windowHandle,instanceHandle);

		//viewport = (D3DEngine::getViewPort(valpoint->GetBuffer(), r));
		viewport = new D3D11_VIEWPORT;
		viewport->TopLeftX = r.left;
		viewport->TopLeftY = r.top;
		viewport->Height = r.bottom - r.top;
		viewport->Width = r.right - r.left;
		viewport->MinDepth = 0.0f;
		viewport->MaxDepth = 1.0f;
		//valpoint->ReleaseBuffer();

		
	}

	valpoint = attributes.retrieveEntry(TString(L"|CameraType"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"LookAt"))
			lookTo = false;
	}

	valpoint = attributes.retrieveEntry(TString(L"|StartingDirection"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float numbers3[3] = { 0.0f, 0.0f, 0.0f };

			TString curString = numbers->at(0);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[0]))
					direction_3.x = numbers3[0];
			}
			
			curString = numbers->at(1);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[1]))
					direction_3.y = numbers3[1];
			}

			curString = numbers->at(2);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[2]))
					direction_3.z = numbers3[2];
			}

			IF_IS_LOOK_TO(lookTo)
			{
				float maxVal = max(abs(direction_3.x), max(abs(direction_3.y), abs(direction_3.z)));
				if (maxVal > 1.0f)
				{
					direction_3.x /= maxVal;
					direction_3.y /= maxVal;
					direction_3.z /= maxVal;
				}
			}
		}
	}


	valpoint = attributes.retrieveEntry(TString(L"|StartingLocation"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float numbers3[3] = { 0.0f,0.0f,0.0f };

			TString curString = numbers->at(0);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[0]))
					location_3.x = numbers3[0];
			}

			curString = numbers->at(1);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[1]))
					location_3.y = numbers3[1];
			}
			curString = numbers->at(2);
			if (curString.GetSize())
			{
				if (!curString.ConvertToFloat(&numbers3[2]))
					location_3.z = numbers3[2];
			}
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Up"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> numbers = valpoint->split(TString(L","));
		if (numbers.Get() && numbers->Size() > 2)
		{
			float* numbers3[3] = { &up_3.x, &up_3.y,&up_3.z };
			for (UINT c = 0; c < 3; c++)
			{
				TString curString = numbers->at(c);
				if (curString.GetSize())
				{
					if (curString.ConvertToFloat(numbers3[c]))
					{
						if (c == 1)
							*numbers3[c] = 1.0f;
						else
							*numbers3[c] = 0.0f;
					}
				}
			}

		}
	}
	RefreshVectors();

	return arenaEngine.Get() && viewport;

	
}

/*
* Method: TArena - getEngine
* Purpose: Retrieves the engine the Arena control is attached to
* Parameters: void
* Returns: TrecPointer<ArenaEngine> - the engine arena control is attached to
*/
TrecPointer<ArenaEngine> TArena::getEngine()
{
	return arenaEngine;
}

/*
* Method: TArena - setEngine
* Purpose: Sets the engine for the Arena control, if one is not already set
* Parameters: TrecPointer<ArenaEngine> e - the engine to set
* Returns: bool - success of function
*/
bool TArena::setEngine(TrecPointer<ArenaEngine> e)
{
	if(e.Get() || !arenaEngine.Get())
	return false;
	arenaEngine = e;
	return true;
}

/*
* Method: TArena - removeEngine
* Purpose: Extracts the engine from the control
* Parameters: void
* Returns: void
*/
void TArena::removeEngine()
{
	arenaEngine->Release();
	arenaEngine.Nullify();
}

/*
* Method: TArena - onDraw
* Purpose: Draws the Control as well as activates the 3D engine to draw
* Parameters: void
* Returns: void
*/
void TArena::onDraw(TObject* obj)
{
	//TControl::onDraw();
	if (content1.Get())
		content1->onDraw(TControl::location, snip);
	
	/*if (renderTarget)
	{
		ID2D1SolidColorBrush* brush = nullptr;
		
		if (SUCCEEDED(renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f,0.0f,0.0f,0.0f), &brush)))
		{
			D2D1_RECT_F df;
			if(convertCRectToD2DRect(&snip, &df))
				renderTarget->FillRectangle(&df, brush);
			brush->Release();
		}
	}*/

	if (arenaEngine.Get() && viewport)
	{
		arenaEngine->RenderScene(projector,camera, *viewport);
	}

	if (text1.Get())
		text1->onDraw(TControl::location, snip, obj);
	if (border1.Get())
		border1->onDraw(TControl::location, snip);
	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->onDraw(obj);
	}
}


/*
* Method: TArena - Rotate
* Purpose: Roates the camera
* Parameters: float leftRight - the horizontal angle to rotate
*				float upDown - the virtical angle to rotate
* Returns: void
*/
void TArena::Rotate(float leftRight, float upDown)
{
	// Create a Float4 structure to hold w value (1.0f) as well as the current direction
	DirectX::XMFLOAT4 directLook{ direction_3.x, direction_3.y,direction_3.z,1.0f };

	// Adjust temporary direction if Camera is lookTo so it represents a transformable direction
	IF_IS_LOOK_AT(lookTo)
	{
		directLook.x -= location_3.x;
		directLook.y -= location_3.y;
		directLook.z -= location_3.z;
	}

	// Create the rotation matrix
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(upDown, leftRight, 0.0f);

	// Apply the rotation matrix to the direction
	DirectX::XMVECTOR lookAtXM = DirectX::XMVector4Transform(DirectX::XMVectorSet(
		directLook.x, directLook.y, directLook.z, directLook.w), rotation);

	// Retireve the results of the camera rotation
	DirectX::XMStoreFloat3(&direction_3, lookAtXM);

	// If this is a lookAt camera, make sure that highest value is a 1
	IF_IS_LOOK_TO(lookTo)
	{
		float maxVal = max(direction_3.x, max(direction_3.y, direction_3.z));
		direction_3.x /= maxVal;
		direction_3.y /= maxVal;
		direction_3.z /= maxVal;
	}

	// Make sure the new direction would not cause conflict with up
	if (direction_3.y && !direction_3.x && !direction_3.z)
	{
		up_3.x += 0.02;
	}
	else if(up_3.x)
	{
		up_3.x = 0.0f;
	}
	RefreshVectors();
}

/*
* Method: TArena - Translate
* Purpose: Translates the camera across the coordinate space
* Parameters: float degree - the distance to translate
*			DirectX::XMFLOAT3 direction - the direction to translate
* Returns: void
*/
void TArena::Translate(float degree, DirectX::XMFLOAT3 direction)
{
	//direction.x = direction.x * degree;
	///direction.y = direction.y * degree;
	//direction.z = direction.z * degree;

	location_3.x += (degree * direction.x);
	location_3.y += (degree * direction.y);
	location_3.z += (degree * direction.z);

	//direction_3.x += direction.x;
	//direction_3.y += direction.y;
	//direction_3.z += direction.z;
	RefreshVectors();
}

/*
* Method: TArena - ChangeToLookAt
* Purpose: Turns camera into a Look At camera
* Parameters: void
* Returns: void
*/
void TArena::ChangeToLookAt()
{
	IF_IS_LOOK_AT(lookTo)
		return;
	direction_3.x += location_3.x;
	direction_3.y += location_3.y;
	direction_3.z += location_3.z;
	lookTo = LOOK_AT;
}

/*
* Method: TArena - ChangeToLookAt
* Purpose: Converts camera to a loot at camera with an object to look at
* Parameters: ArenaModel& am - the object to look at
*			float distance - the distance between the camera and the object
*			bool changeAngle -  adjusting the angle of the camera
* Returns:
*/
void TArena::ChangeToLookAt(ArenaModel& am, float distance, bool changeAngle)
{
	DirectX::XMFLOAT3 newLoc = am.GetLocation();
	DirectX::XMFLOAT3 rawDistanceCoords;
	rawDistanceCoords.x = newLoc.x - location_3.x;
	rawDistanceCoords.y = newLoc.y - location_3.y;
	rawDistanceCoords.z = newLoc.z - location_3.z;

	float currentDist = sqrt(pow(rawDistanceCoords.x, 2) + pow(rawDistanceCoords.y, 2)
		+ pow(rawDistanceCoords.z, 2));

	if (distance > 0.0)
	{
		float difDistance = currentDist - distance;
	}
}

/*
* Method: TArena - ChangeToLookTo
* Purpose: Converts camera to a look to camera
* Parameters: void
* Returns: void
*/
void TArena::ChangeToLookTo()
{
	IF_IS_LOOK_TO(lookTo)
		return;
	float max = max(abs(direction_3.x), max(abs(direction_3.y), abs(direction_3.z)));
	DirectX::XMFLOAT3 atTo = direction_3;
	atTo.x -= location_3.x;
	atTo.y -= location_3.y;
	atTo.z -= location_3.z;
	direction_3.x = atTo.x / max;
	direction_3.y = atTo.y / max;
	direction_3.z = atTo.z / max;
	lookTo = LOOK_TO;
	RefreshVectors();
}

/*
* Method: TArena - ChangeToLookTo
* Purpose: Change to LookTo camera with parameters
* Parameters: float x - x-coordindate
*				float y - y-coordinate
*				float z - z-coordinates
* Returns: void
*/
void TArena::ChangeToLookTo(float x, float y, float z)
{
	lookTo = LOOK_TO;
	float max = max(abs(x), max(abs(y), abs(z)));
	direction_3.x = x / max;
	direction_3.y = y / max;
	direction_3.z = z / max;
	RefreshVectors();
}

/*
* Method: TArena - GetAnaGameType
* Purpose: Retrieves the AnaGame type for the TArena class
* Parameters: void
* Returns: UCHAR* - AnaGame representation of the type
*/
UCHAR * TArena::GetAnaGameType()
{
	return nullptr;
}

void TArena::UpdatePos(float f, UCHAR dir)
{
	switch (dir)
	{
	case 0:
		location_3.x = f;
		break;
	case 1:
		location_3.y = f;
		break;
	case 2:
		location_3.z = f;
	}
	RefreshVectors();
}

void TArena::UpdateDir(float f, UCHAR dir)
{
	switch (dir)
	{
	case 0:
		direction_3.x = f;
		break;
	case 1:
		direction_3.y = f;
		break;
	case 2:
		direction_3.z = f;
	}
	RefreshVectors();
}

void TArena::Resize(RECT r)
{
	TControl::Resize(r);
	if (!viewport)
		return;
	viewport->TopLeftX = r.left;
	viewport->TopLeftY = r.top;
	viewport->Height = r.top - r.bottom;
	viewport->Width = r.right - r.left;
}

DirectX::XMFLOAT3 TArena::GetCameraLocation()
{
	return location_3;
}

DirectX::XMFLOAT3 TArena::GetCameraDirection()
{
	return direction_3;
}

/*
* Method: TArena - RefreshVectors
* Purpose: Refreshes the location and direction vectors
* Parameters: void
* Returns: void
*/
void TArena::RefreshVectors()
{
	//DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(direction_3.x, direction_3.y, direction_3.z);

	direction = DirectX::XMVectorSet(direction_3.x, direction_3.y, direction_3.z, 1.0f);

	location = DirectX::XMVectorSet(location_3.x, location_3.y, location_3.z, 1.0f);
	//direction = DirectX::XMVectorSet(direction_3.x, direction_3.y, direction_3.z, 1.0f);
	up = DirectX::XMVectorSet(up_3.x, up_3.y,up_3.z, 1.0f);
	UpdateCamera();
}

/*
* Method: TArena - UpdateCamera
* Purpose: Resets the camera matrix
* Parameters: void
* Returns: void
*/
void TArena::UpdateCamera()
{
	
	camera = DirectX::XMMatrixLookToLH(location, direction, up);
}
