#include "stdafx.h"
#include "NewObjectTDialog.h"
#include "MainFrm.h"
#include <TDataArray.h>
//#include "DxErr.h"

TString selectCube = L"OnSelectCube";
TString selectShpere = L"OnSelectSphere";
TString selectTPyramid = L"OnSelectTriPyramid";
TString selectSPyramid = L"OnSelectSquarePyramid";
TString selectCone = L"OnSelectCone";
TString selectCylinder = L"OnSelectCylinder";

TString setToSize = L"OnSetToSize";
TString setToRot = L"OnSetToRotate";
TString setToTran = L"OnSetToTranslate";

TString setNear = L"OnNear";
TString setUp = L"OnUp";
TString setFar = L"OnFar";

TString setLeft = L"OnLeft";
TString setDown = L"OnDown";
TString setRight = L"OnRight";


NewObjectTDialog::NewObjectTDialog() : TDialog(1000,800)
{
	targetModel = nullptr;
	engine = nullptr;
	arenaControl = nullptr;
	stack = nullptr; topGallary = nullptr; bottomGallary = nullptr;
	bit = nullptr;
	CMainFrame* window = (CMainFrame*)AfxGetMainWnd();

	TString direct = window->getSpecialDirectory(bdi_user_appdata);

	for (UINT c = 0; c <direct.GetLength(); c++)
	{
		fileHolder += direct[c];
	}

	for (int c = 0; c < T3D_DIALOG_METHOD_COUNT; c++)
		_3DDialogMethods[c] = nullptr;

	_3DDialogMethods[0] = &NewObjectTDialog::OnSelectCube;
	_3DDialogMethods[1] = &NewObjectTDialog::OnSelectSphere;
	_3DDialogMethods[2] = &NewObjectTDialog::OnSelectTrianglePyramid;
	_3DDialogMethods[3] = &NewObjectTDialog::OnSelectSquarePyramid;
	_3DDialogMethods[4] = &NewObjectTDialog::OnSelectCone;
	_3DDialogMethods[5] = &NewObjectTDialog::OnSelectCylinder;

	_3DDialogMethods[6] = &NewObjectTDialog::OnSetToSize;
	_3DDialogMethods[7] = &NewObjectTDialog::OnSetToRotate;
	_3DDialogMethods[8] = &NewObjectTDialog::OnSetToTranslate;

	_3DDialogMethods[9] = &NewObjectTDialog::OnFar;
	_3DDialogMethods[10] = &NewObjectTDialog::OnUp;
	_3DDialogMethods[11] = &NewObjectTDialog::OnNear;

	_3DDialogMethods[12] = &NewObjectTDialog::OnLeft;
	_3DDialogMethods[13] = &NewObjectTDialog::OnDown;
	_3DDialogMethods[14] = &NewObjectTDialog::OnRight;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = selectCube;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 1;
	enid.name = selectShpere;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 2;
	enid.name = selectTPyramid;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 3;
	enid.name = selectSPyramid;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 4;
	enid.name = selectCone;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 5;
	enid.name = selectCylinder;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 6;
	enid.name = setToSize;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 7;
	enid.name = setToRot;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 8;
	enid.name = setToTran;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 9;
	enid.name = setNear;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 10;
	enid.name = setUp;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 11;
	enid.name = setFar;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 12;
	enid.name = setLeft;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 13;
	enid.name = setDown;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	enid.eventID = 14;
	enid.name = setRight;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	fileHolder += L"\\3DObjectDialog.tml";
	constructionWorked = setAnaface(fileHolder);


}


NewObjectTDialog::~NewObjectTDialog()
{
}

void NewObjectTDialog::OnSelectCube(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;
	
	TDataArray<DirectX::XMFLOAT3> cubeVertices;
	/*
	cubeVertices.push_back(DirectX::XMFLOAT3(-0.025, -0.025, 0.025));   // 0
	cubeVertices.push_back(DirectX::XMFLOAT3(0.025, -0.025, 0.025));  // 1
	cubeVertices.push_back(DirectX::XMFLOAT3(0.025, 0.025, 0.025));  // 2
	cubeVertices.push_back(DirectX::XMFLOAT3(-0.025, 0.025, 0.025)); // 3
	cubeVertices.push_back(DirectX::XMFLOAT3(-0.025, -0.025, -0.025));  // 4
	cubeVertices.push_back(DirectX::XMFLOAT3(0.025, -0.025, -0.025)); // 5
	cubeVertices.push_back(DirectX::XMFLOAT3(0.025, 0.025, -0.025)); // 6
	cubeVertices.push_back(DirectX::XMFLOAT3(-0.025, 0.025, -0.025));// 7
	*/
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, -1.0));   // 0
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, -1.0));  // 1
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, -1.0));  // 2
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, -1.0)); // 3
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));  // 4
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, 1.0)); // 5
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, 1.0)); // 6
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));// 7
	/*
	TDataArray<int> indices = {
		// Plane of 0,1,2,3
		0, 3, 2,
		0, 2, 1,
		// Plane of 0,1,4,5
		0, 4, 5,
		0, 5, 1,
		// Plane of 0,3,4,7
		4, 7, 3,
		4, 3, 0,
		// Plane of 1,2,5,6
		1, 2, 6,
		1, 6, 5,
		// Plane of 2,3,6,7
		2, 3, 7,
		2, 7, 6,
		// Plane of 4,5,6,7
		4, 7, 6,
		4, 6, 5
	};*/

	 std::vector<UINT> v = {
		0,1,2, // front face
		0,2,3,
		4,6,5, // back face
		4,7,6,
		4,5,1, // left face
		4,1,0,
		3,2,6, // right face
		3,6,7,
		1,5,6, // top face
		1,6,2,
		4,0,3, // bottom face
		4,3,7
	};
	 TDataArray<UINT> indices;
		 indices = v;
		 v.clear();

	TDataArray<float> floats;

	for (UINT c = 0; c < cubeVertices.Size(); c++)
	{
		floats.push_back(cubeVertices[c].x);
		floats.push_back(cubeVertices[c].y);
		floats.push_back(cubeVertices[c].z);
		//floats.push_back(1.0f);
	}

	targetModel->SetVertexData(floats, default_shader_Single_Color);
	targetModel->SetIndices(indices);
	targetModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	targetModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);
	
/*
	if (targetModel->PrepModelWithSupplies(cubeVertices, DirectX::XMFLOAT4(1.0, 1.0, 1.0, 6.0)))
		MessageBox(windowHandle, L"Model was not prepped properly", L"Model Shader issue", MB_OK | MB_ICONHAND);
	targetModel->PrepIndicees(indices);*/



}

void NewObjectTDialog::OnSelectSphere(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;

	//TDataArray<DirectX::XMFLOAT3> cubeVertices;

	//cubeVertices.push_back(DirectX::XMFLOAT3())
}

void NewObjectTDialog::OnSelectTrianglePyramid(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;

	TDataArray<DirectX::XMFLOAT3> cubeVertices;

	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, -1.0, -1.0));
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, 1.0, 1.0));

	std::vector<UINT> v = {
		0,1,2,
		0,3,2,
		2,3,1,
		1,3,0
	};
	TDataArray<UINT> indices;
		indices= v;
	TDataArray<float> floats;

	for (UINT c = 0; c < cubeVertices.Size(); c++)
	{
		floats.push_back(cubeVertices[c].x);
		floats.push_back(cubeVertices[c].y);
		floats.push_back(cubeVertices[c].z);
		//floats.push_back(1.0f);
	}

	targetModel->SetVertexData(floats, default_shader_Single_Color);
	targetModel->SetIndices(indices);
	targetModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	targetModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);
	/*
	if (targetModel->PrepModelWithSupplies(cubeVertices, DirectX::XMFLOAT4(1.0, 1.0, 1.0, 6.0)))
		MessageBox(windowHandle, L"Model was not prepped properly", L"Model Shader issue", MB_OK | MB_ICONHAND);
	targetModel->PrepIndicees(indices);*/
}

void NewObjectTDialog::OnSelectSquarePyramid(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;

	TDataArray<DirectX::XMFLOAT3> cubeVertices;
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));   // 0
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, -1.0));  // 1
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));    // 2
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, -1.0));   // 3
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, 1.0, 0.0));     // 4
	
	std::vector<UINT> v = {
		0,1,2,
		2,1,3,
		0,4,1,
		1,4,3,
		3,4,2,
		2,4,0
	};
	TDataArray<UINT> indices;
	indices= v;
	v.clear();
	TDataArray<float> floats;

	for (UINT c = 0; c < cubeVertices.Size(); c++)
	{
		floats.push_back(cubeVertices[c].x);
		floats.push_back(cubeVertices[c].y);
		floats.push_back(cubeVertices[c].z);
		//floats.push_back(1.0f);
	}

	targetModel->SetVertexData(floats, default_shader_Single_Color);
	targetModel->SetIndices(indices);
	targetModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	targetModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

	/*
	if (targetModel->PrepModelWithSupplies(cubeVertices, DirectX::XMFLOAT4(1.0, 1.0, 1.0, 6.0)))
	MessageBox(windowHandle, L"Model was not prepped properly", L"Model Shader issue", MB_OK | MB_ICONHAND);
	targetModel->PrepIndicees(indices);*/
}

void NewObjectTDialog::OnSelectCone(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;

	TDataArray<DirectX::XMFLOAT3> cubeVertices;
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));   // 0
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 0.0));   // 1
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, -1.0));  // 2
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, -1.0, -1.0));   // 3
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, -1.0));   // 4
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, -1.0, 1.0));    // 5
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));    // 6
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 0.0));    // 7

	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, 1.0, 0.0));     // 8

	 std::vector<UINT> v = {
		0,1,4,
		1,2,5,
		2,3,6,
		3,4,7,
		4,5,0,
		5,6,1,
		6,7,2,
		7,0,3,

		0,1,8,
		1,2,8,
		2,3,8,
		3,4,8,
		4,5,8,
		5,6,8,
		6,7,8,
		7,0,8
	};
	 TDataArray<UINT> indices;
	 indices = v;
	 v.clear();
	TDataArray<float> floats;

	for (UINT c = 0; c < cubeVertices.Size(); c++)
	{
		floats.push_back(cubeVertices[c].x);
		floats.push_back(cubeVertices[c].y);
		floats.push_back(cubeVertices[c].z);
		//floats.push_back(1.0f);
	}

	targetModel->SetVertexData(floats, default_shader_Single_Color);
	targetModel->SetIndices(indices);
	targetModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	targetModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

	/*
	if (targetModel->PrepModelWithSupplies(cubeVertices, DirectX::XMFLOAT4(1.0, 1.0, 1.0, 6.0)))
	MessageBox(windowHandle, L"Model was not prepped properly", L"Model Shader issue", MB_OK | MB_ICONHAND);
	targetModel->PrepIndicees(indices);*/
}

void NewObjectTDialog::OnSelectCylinder(TControl * tc, EventArgs ea)
{
	if (!tc || !arenaControl.get() || !targetModel.get())
		return;

	TDataArray<DirectX::XMFLOAT3> cubeVertices;
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 1.0));   // 0
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, 0.0));   // 1
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, -1.0, -1.0));  // 2
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, -1.0, -1.0));   // 3
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, -1.0));   // 4
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, -1.0, 1.0));    // 5
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 1.0));    // 6
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, -1.0, 0.0));    // 7

	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, 1.0));   // 8
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, 0.0));   // 9
	cubeVertices.push_back(DirectX::XMFLOAT3(-1.0, 1.0, -1.0));  // 10
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, 1.0, -1.0));   // 11
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, -1.0));   // 12
	cubeVertices.push_back(DirectX::XMFLOAT3(0.0, 1.0, 1.0));    // 13
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, 1.0));    // 14
	cubeVertices.push_back(DirectX::XMFLOAT3(1.0, 1.0, 0.0));    // 15

	 std::vector<UINT> v=  {
		0,1,4,
		1,2,5,
		2,3,6,
		3,4,7,
		4,5,0,
		5,6,1,
		6,7,2,
		7,0,3,

		8,9,12,
		9,10,13,
		10,11,14,
		11,12,15,
		12,13,8,
		13,14,9,
		14,15,10,
		15,8,11,

		0,1,8,
		8,9,0,
		1,2,9,
		9,10,1,
		2,3,10,
		10,11,2,
		3,4,11,
		11,12,3,
		4,5,12,
		12,13,4,
		5,6,13,
		13,14,5,
		6,7,14,
		14,15,6,
		7,8,15,
		15,8,7
	};
	 TDataArray<UINT> indices;
	 indices = v;
	 v.clear();
	TDataArray<float> floats;

	for (UINT c = 0; c < cubeVertices.Size(); c++)
	{
		floats.push_back(cubeVertices[c].x);
		floats.push_back(cubeVertices[c].y);
		floats.push_back(cubeVertices[c].z);
		//floats.push_back(1.0f);
	}

	targetModel->SetVertexData(floats, default_shader_Single_Color);
	targetModel->SetIndices(indices);
	targetModel->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	targetModel->setPipeColorBuffer(1.0f, 0.0f, 0.0f, 1.0f);

	/*
	if (targetModel->PrepModelWithSupplies(cubeVertices, DirectX::XMFLOAT4(1.0, 1.0, 1.0, 6.0)))
	MessageBox(windowHandle, L"Model was not prepped properly", L"Model Shader issue", MB_OK | MB_ICONHAND);
	targetModel->PrepIndicees(indices);*/
}

void NewObjectTDialog::OnSetToSize(TControl * tc, EventArgs ea)
{
	mcm = mc_size;
}

void NewObjectTDialog::OnSetToRotate(TControl * tc, EventArgs ea)
{
	mcm = mc_rotate;
}

void NewObjectTDialog::OnSetToTranslate(TControl * tc, EventArgs ea)
{
	mcm = mc_translate;
}

void NewObjectTDialog::OnFar(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:
	case mc_size:
		return;
	case mc_rotate:
		targetModel->Rotate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(-0.5f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	}
}

void NewObjectTDialog::OnUp(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:

		return;
	case mc_size:
		targetModel->Enlarge(0.1);
		return;
	case mc_rotate:
		targetModel->Rotate(0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(0.5f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
		break;
	}
}

void NewObjectTDialog::OnNear(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:
	case mc_size:
		return;
	case mc_rotate:
		targetModel->Rotate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(0.5f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	}
}

void NewObjectTDialog::OnLeft(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:
	case mc_size:
		return;
	case mc_rotate:
		targetModel->Rotate(-0.1, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(0.5f, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
		break;
	}
}

void NewObjectTDialog::OnDown(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:

		return;
	case mc_size:
		targetModel->Enlarge(-0.1);
		return;
	case mc_rotate:
		targetModel->Rotate(-0.1, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(-0.5f, DirectX::XMFLOAT3(0.0, 0.0, 1.0));
		break;
	}
}

void NewObjectTDialog::OnRight(TControl * tc, EventArgs ea)
{
	if (!targetModel.get())
		return;
	switch (mcm)
	{
	case mc_not_set:
	case mc_size:
		return;
	case mc_rotate:
		targetModel->Rotate(-0.5, DirectX::XMFLOAT3(0.0, 1.0, 0.0));
		break;
	case mc_translate:
		targetModel->Translate(1.0f, DirectX::XMFLOAT3(1.0, 0.0, 0.0));
		break;
	}
}

void NewObjectTDialog::OnUpdateNear(TControl * tc, EventArgs ea)
{
}

void NewObjectTDialog::OnUpdateFar(TControl * tc, EventArgs ea)
{
}

bool NewObjectTDialog::getValidConstruction()
{
	return constructionWorked;
}

void NewObjectTDialog::initializeControls()
{
	TrecPointer<TControl> dummy;
	// Get the Control for the Top Section
	try
	{
		dummy = dynamic_cast<TLayout*>(pointer.get())->GetLayoutChild(0, 0);
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nRoot Control needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch(...)
	{

	}

	// Now get the bottom gallary using the new control
	try
	{
		topGallary = dynamic_cast<TLayout*>(dummy.get());
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nTop Control in layout stack needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{

	}

	// Get the control for the bottom section
	try {
		dummy = dynamic_cast<TLayout*>(pointer.get())->GetLayoutChild(0, 1);
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nRoot Control needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{

	}

	// Now set up the Bottom Gallary
	try
	{
	bottomGallary = dynamic_cast<TLayout*>(dummy.get());
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nBottom Control in layout stack needed to be a Layout", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nBottom Control in layout stack is missing", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	dummy = bottomGallary->GetLayoutChild(1, 0);
	// Now set up the Arena
	try
	{
		arenaControl = dynamic_cast<TArena*>(dummy.get());
	}
	catch (std::bad_cast e)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nArena not established due to control type mismatch", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	catch (...)
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nArena not established due to NULL reference", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}
	

	

	engine;
	
	
	TrecPointer<ArenaEngine> tempEngine;
		tempEngine = arenaControl->getEngine();

	if (tempEngine.get() != engine.get())
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\n3D Engine mismatch", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}

	if (!engine.get())
	{
		MessageBox(windowHandle, L"Dialog was not initialized properly!\n3D Engine not acquired", L"TDialog Error", MB_OK | MB_ICONHAND);
		return;
	}



	int defaultEr = engine->InitializeDefaultShaders();

	if (defaultEr)
	{
		switch(defaultEr)
		{
		case 1:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFolders for the Shaders not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;

		case 2:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFile For Single Color Shader not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case -2:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nResources For Single Color Shader not properly aligned!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case 3:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFile For 3D Color Shader not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case -3:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nResources For 3D Color Shader not properly aligned!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case 4:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFile For 4D Color Shader not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case -4:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nResources For 4D Color Shader not properly aligned!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case 5:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFile For Single Texture Shader not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case -5:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nResources For Single Texture Shader not properly aligned!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case 6:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nFile For Double Texture Shader not Found!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		case -6:
			MessageBox(windowHandle, L"Dialog was not initialized properly!\nResources For Double Texture Shader not properly aligned!", L"TDialog Error", MB_OK | MB_ICONHAND);
			break;
		}
		return;
	}

	targetModel = new ArenaModel(*engine.get());
	if(!targetModel.get())
		MessageBox(windowHandle, L"Dialog was not initialized properly!\nModel Failed to allocate", L"TDialog Error", MB_OK | MB_ICONHAND);

	TDataArray<DirectX::XMFLOAT3> scaleVertices;
	scaleVertices.push_back(DirectX::XMFLOAT3(100.0, 0.0, 0.0));   // 0
	scaleVertices.push_back(DirectX::XMFLOAT3(-100.0, 0.0, 0.0));  // 2

	scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 100.0, 0.0));
	scaleVertices.push_back(DirectX::XMFLOAT3(0.0, -100.0, 0.0));

	scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, 100.0));
	scaleVertices.push_back(DirectX::XMFLOAT3(0.0, 0.0, -100.0));

	TDataArray<float> floats;

	for (UINT c = 0; c < scaleVertices.Size(); c++)
	{
		floats.push_back(scaleVertices[c].x);
		floats.push_back(scaleVertices[c].y);
		floats.push_back(scaleVertices[c].z);
		//floats.push_back(1.0f);
	}

	TDataArray<UINT> uints;
	for (UINT c = 0; c < scaleVertices.Size(); c++)
		uints.push_back(c);

	scale = new ArenaModel(*engine.get());
	scale->SetVertexData(floats, default_shader_Single_Color, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	scale->SetIndices(uints);
	scale->setColorBuffer(0.0f, 0.0f, 1.0f, 1.0f);
	return;
}
bool NewObjectTDialog::InitializeEngine()
{
	if(!windowHandle)
		return false;

	if (engine.get())
		return false;
	engine = ArenaEngine::GetArenaEngine(TString(L"Dialog"), windowHandle, instance);
	if (!engine.get())
		return false;
	IDXGISwapChain* swapper = engine->getSwapChain().get();
	if (!swapper)
		return false;
	IDXGISurface* surf = nullptr;
	HRESULT res = swapper->GetBuffer(0,IID_PPV_ARGS(&surf));
	if (FAILED(res))
	{
		//*error = -5;
		return false;
	}

	// Here, we diverge from the conventional DCRenderTarget to the Direct 2D Device and Context Device
	IDXGIDevice* dev = engine->getDeviceD_U().get();

	D2D1_CREATION_PROPERTIES cr;
	cr.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	cr.options = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
	cr.threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED;
	ID2D1Device* d2Dev = nullptr;
	res = fact->CreateDevice(dev, &d2Dev);
	device = d2Dev;
	if (FAILED(res))
	{
		//*error = -3;
		return false;
	}

	ID2D1DeviceContext* cd = nullptr;

	res = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &cd);
	contextDevice = cd;
	if (FAILED(res))
	{
		device = nullptr;
		return false;
	}

	regRenderTarget = contextDevice.get();

	D2D1_BITMAP_PROPERTIES1 bmp1;
	bmp1.colorContext = nullptr;
	bmp1.dpiX = bmp1.dpiY = 0;
	bmp1.pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE
	);
	bmp1.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_GDI_COMPATIBLE
		| D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

	ID2D1Bitmap1* rawBit = nullptr;
	res = contextDevice->CreateBitmapFromDxgiSurface(surf, bmp1, &rawBit);
	bit = rawBit;

	if (FAILED(res))
	{

		device = nullptr;
		contextDevice = nullptr;
		return false;
	}

	contextDevice->SetTarget(bit.get());
	ID2D1GdiInteropRenderTarget* gdiRender_raw = nullptr;
	res = contextDevice->QueryInterface(__uuidof(ID2D1GdiInteropRenderTarget), (void**)&gdiRender_raw);
	gdiRender = gdiRender_raw;
	bit = nullptr;
	/*
	float x = 0.0f, y = 0.0f;
	fact->GetDesktopDpi(&x, &y);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		x,
		y,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);

	res = fact->CreateDxgiSurfaceRenderTarget(surf, &props, reinterpret_cast<ID2D1RenderTarget**>(&renderTarget));


*/
	if (FAILED(res))
	{
		//*error = -3;
		
		//DXTrace(__FILEW__, __LINE__, res, nullptr, TRUE);
		return false;
	}
	return true;
}
void NewObjectTDialog::onDraw()
{
	if (!pointer.get())
		return;
	if (contextDevice.get())
	{
		contextDevice->BeginDraw();
		if (engine.get())
			engine->PrepareScene(D2D1::ColorF(D2D1::ColorF::White, 1.0f));
		if (firstDraw)
		{
			contextDevice->Clear(D2D1::ColorF(D2D1::ColorF::Aquamarine));
			firstDraw = false;
		}
		pointer->onDraw();

		HDC contDC = 0;
		if (gdiRender.get() && SUCCEEDED(gdiRender->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &contDC)))
		{
			HDC windDC = GetDC(windowHandle);
			if (windDC)
			{
				SelectObject(windDC, GetStockObject(DC_BRUSH));
				SetDCBrushColor(windDC, RGB(255, 255, 255));
			}
			if (engine.get())
				engine->FinalizeScene();
			//RECT winLoc;
			//if (GetWindowRect(windowHandle, &winLoc))
			//	InvertRect(contDC, &winLoc);
			if (arenaControl.get())
			{
				RECT d3dRect = arenaControl->getSnip();
				int w = d3dRect.right - d3dRect.left;
				int h = d3dRect.bottom - d3dRect.top;
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.AlphaFormat = 0;
				bf.SourceConstantAlpha = 0xff;
				if (AlphaBlend(contDC, d3dRect.left, d3dRect.top, w, h, windDC, d3dRect.left, d3dRect.top, w, h, bf))
					int er = GetLastError();
			}
			int er = 0;
			if (!BitBlt(windDC, 0, 0, width, height, contDC, 0, 0, SRCCOPY))
			{
				er = GetLastError();
			}

			gdiRender->ReleaseDC(nullptr);
		}
		HRESULT res = contextDevice->EndDraw();
	}
}
TrecPointer<ArenaModel> NewObjectTDialog::GetModel()
{
	return targetModel;
}
/*
bool NewObjectTDialog::Initialize(int *error)
{
	if (!fact || !FileOpened)
	{
		*error = -1;
		return false;
	}

	FileOpened = file.Open(filePath.GetBuffer(), CFile::modeRead);
	filePath.ReleaseBuffer();
	if (!FileOpened)
	{
		*error = -1;
		return false;
	}

	// Create the Window
	ZeroMemory(&windowFeatures, sizeof(windowFeatures));
	//windowFeatures.cbSize = sizeof(windowFeatures);
	windowFeatures.style = CS_HREDRAW | CS_VREDRAW;
	windowFeatures.lpfnWndProc = WindowProctor;
	windowFeatures.cbClsExtra = 0;
	windowFeatures.cbWndExtra = 0;
	windowFeatures.hInstance = AfxGetInstanceHandle();
	windowFeatures.hIcon = nullptr;//LoadIcon(NULL, IDI_APPLICATION);
	windowFeatures.hCursor = nullptr;//LoadCursor(NULL, IDC_ARROW);
	windowFeatures.hbrBackground = nullptr;//(HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	windowFeatures.lpszMenuName = nullptr;
	windowFeatures.lpszClassName = name.GetBuffer();

	//windowFeatures.hIconSm = nullptr;

	name.ReleaseBuffer();


	if (!at)
	{


		at = RegisterClass(&windowFeatures);

		if (!at)
		{
			*error = -2;
			DWORD err = GetLastError();
			return false;
		}
	}
	windowHandle = CreateWindow(name.GetBuffer(),
		name.GetBuffer(),
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		width,
		height,
		nullptr,
		nullptr,
		windowFeatures.hInstance,
		nullptr);
	name.ReleaseBuffer();
	if (!windowHandle)
	{
		*error = -2;
		DWORD err = GetLastError();
		return false;
	}

	// Now Prep Direct 2D and Direct Write for this window
	IDXGISurface* surf = nullptr;
	HRESULT res = swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&surf));
	if (FAILED(res))
	{
		*error = -5;
		return false;
	}
	res = fact->CreateDxgiSurfaceRenderTarget(surf, &(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		0, 0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT)), reinterpret_cast<ID2D1RenderTarget**>(&renderTarget));



	if (FAILED(res))
	{
		*error = -3;
		return false;
	}
	RECT area;
	GetClientRect(windowHandle, &area);
	renderTarget->BindDC(GetDC(windowHandle), &area);

	// Now Create THe Anaface that lays it out
	TString directory = file.GetFilePath();
	TString fileName = file.GetFileName();
	int ind = directory.Find(fileName, 0);
	if (ind > 0)
		directory.Delete(ind, fileName.GetLength());

	AnafaceParser* aParse = new AnafaceParser(renderTarget, windowHandle, &directory);
	aParse->setEventSystem(idMatch);

	archer = new CArchive(&file, CArchive::load);

	TML_Reader_* reader = new TML_Reader_(archer, aParse);
	int readingError;
	if (!reader->read(&readingError))
	{
		*error = readingError;
		return false;
	}
	else
	{
		TControlPointer* rootControl = aParse->getRootControl();
		if (rootControl)
		{
			pointer = *rootControl;

			pointer.rootType->onCreate(area);
		}
		else
		{
			*error = -4;
			return false;
		}
	}
	file.Close();
	isReady = true;
	return true;
}*/

void NewObjectTDialog::MessageHandler()
{
	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < EventCred.Size(); c++)
	{
		tc = EventCred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < T3D_DIALOG_METHOD_COUNT)
		{
			// call method
			if (_3DDialogMethods[e_id])
				(this->*_3DDialogMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}
