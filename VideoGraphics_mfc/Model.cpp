#include "stdafx.h"
#include "Model.h"
#include "InlineMatrixOperators.h"
#include <DirectXTex.h>

//using namespace DirectX;

namespace
{
	ID3D11VertexShader* single_color_vertex_shader;
	ID3D11VertexShader*	vertex_color_vertex_shader;
	ID3D11VertexShader*	texel_color_vertex_shader;
	ID3D11PixelShader* single_color_pixel_shader;
	ID3D11PixelShader* vertex_color_pixel_shader;
	ID3D11PixelShader* texel_color_pixel_shader;
}


Model::Model(D3DEngine& e)
{
	modType = mod_type_Unset;
	set = false;
	parentModel = nullptr;
	colorMap = nullptr;
	sampleState = nullptr;
	PrepShaderNamespace();
	engine = &e;
	context = engine->getDevice();
	swapChain = engine->getSwapChain();
	renderTarget = engine->getRederTarget();
	device = engine->getDeviceD();
	indexBuffer = nullptr;

	engine->AddModel(this);
}

Model::Model(ModelType mt, D3DEngine& e)
{
	modType = mt;
	set = false;
	parentModel = nullptr;
	colorMap = nullptr;
	sampleState = nullptr;
	PrepShaderNamespace();
	engine = &e;
	context = engine->getDevice();
	swapChain = engine->getSwapChain();
	renderTarget = engine->getRederTarget();
	device = engine->getDeviceD();
	indexBuffer = nullptr;

	engine->AddModel(this);
}


Model::~Model()
{
	UnloadModel();
	for (int c = 0; c < childModels.size(); c++)
	{
		if(childModels[c])
			delete childModels[c];
		childModels[c] = nullptr;
	}
	childModels.clear();
}

HRESULT Model::LoadModel(CArchive & arch)
{



	set = true;
	return E_NOTIMPL;
}

bool Model::PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, std::vector<DirectX::XMFLOAT4>& c)
{
	// We are in MultiColorMode
	modType = mod_type_vertex_color;
	vertexShader = engine->getVertexShader(modType);
	pixelShader = engine->getPixelShader(modType);

	verticies = v;
	colors = c;

	if (!device)
		return false;

	if (v.size() != c.size()) // Make sure they have the same amount of data
		return false;

	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * 2;

	std::vector<DirectX::XMFLOAT4> vbuff;
	for (int C = 0; C < verticies.size() && C < colors.size();C++)
	{
		DirectX::XMFLOAT4 f4;
		f4.x = verticies[C].x;
		f4.y = verticies[C].y;
		f4.z = verticies[C].z;
		f4.w = 1.0f;
		vbuff.push_back(f4);
		vbuff.push_back(colors[C]);
	}

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = vbuff.data();
	
	HRESULT res = device->CreateBuffer(&vDesc, &rd, &vertexBuffer);
	return SUCCEEDED(res);
}


typedef struct vertexTexelCombo
{
	DirectX::XMFLOAT4 x4;
	DirectX::XMFLOAT2 x2;
}vertexTexelCombo;


bool Model::PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, std::vector<DirectX::XMFLOAT2>& t, CFile & f)
{
	// We are in MultiColorMode
	modType = mod_type_vertex_texel;
	vertexShader = engine->getVertexShader(modType);
	pixelShader = engine->getPixelShader(modType);

	verticies = v;
	texels = t;

	if (!device)
		return false;

	if (v.size() != t.size()) // Make sure they have the same amount of data
		return false;


	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.ByteWidth = sizeof(vertexTexelCombo);

	std::vector<vertexTexelCombo> vbuff;
	for (int C = 0; C < verticies.size() && C < texels.size();C++)
	{
		vertexTexelCombo vtc;
		vtc.x4.x = verticies[C].x;
		vtc.x4.y = verticies[C].y;
		vtc.x4.z = verticies[C].z;
		vtc.x4.w = 1.0f;
		vtc.x2 = texels[C];
		vbuff.push_back(vtc);

	}

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = vbuff.data();

	HRESULT res = device->CreateBuffer(&vDesc, &rd, &vertexBuffer);

	if (FAILED(res))
		return false;

	// Now onto the texture itself
	DirectX::TexMetadata imageData;
	DirectX::ScratchImage* scratchy = new DirectX::ScratchImage();

	res = LoadFromDDSFile(L"font.dds", DirectX::DDS_FLAGS_NONE, &imageData, *scratchy);

	if (FAILED(res))
	{
		if (scratchy)
			delete scratchy;
		scratchy = nullptr;
		return false;
	}

	res = CreateShaderResourceView(device, scratchy->GetImages(), scratchy->GetImageCount(), imageData, &colorMap);
	if (scratchy)
		delete scratchy;
	scratchy = nullptr;



	return SUCCEEDED(res);

}

bool Model::PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, DirectX::XMFLOAT4 c)
{
	// We are in SingleColorMode
	modType = mod_type_single_color;
	vertexShader = engine->getVertexShader(modType);
	pixelShader = engine->getPixelShader(modType);

	verticies = v;


	if (!device)
		return false;


	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);

	std::vector<DirectX::XMFLOAT4> vbuff;
	for (int C = 0; C < verticies.size() && C < colors.size();C++)
	{

		DirectX::XMFLOAT4 f4;
		f4.x = verticies[C].x;
		f4.y = verticies[C].y;
		f4.z = verticies[C].z;
		f4.w = 1.0f;
		vbuff.push_back(f4);

	}

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = vbuff.data();

	HRESULT res = device->CreateBuffer(&vDesc, &rd, &vertexBuffer);
	return SUCCEEDED(res);
}

void Model::PrepIndicees(std::vector<int>& i)
{
	indicees = i;
	if (indicees.size() && indicees.size() == 3 * verticies.size())
	{
		generateIndex();
	}
}

void Model::UnloadModel()
{
	set = false;
	ZeroMemory(&verticies, verticies.size() * sizeof(verticies[0]));
	verticies.empty();
	direction = location = { 0.0f,0.0f,0.0f };

}

int Model::Rotate(float radian, DirectX::XMFLOAT3 direction)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(
		direction.x * radian,
		direction.y * radian,
		direction.z * radian
	);
	DirectX::XMFLOAT4X4 matrix4;

	for (int c = 0; c < 4; c++)
	{
		for (int C = 0; C < 4; C++)
		{
			matrix4(c, C) = matrix.r[c].m128_f32[C];
		}
	}

	DirectX::XMFLOAT4 loc4{ direction.x, direction.y, direction.z, 1.0f };
	loc4 = matrix4 * loc4;

	direction.x = loc4.x / loc4.w;
	direction.y = loc4.y / loc4.w;
	direction.z = loc4.z / loc4.w;

	return 0;
}

int Model::Translate(float degree, DirectX::XMFLOAT3 direction)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(direction.x * degree, 
		direction.y * degree, direction.z * degree);

	DirectX::XMFLOAT4X4 matrix4;

	for (int c = 0; c < 4; c++)
	{
		for (int C = 0; C < 4; C++)
		{
			matrix4(c,C) = matrix.r[c].m128_f32[C];
		}
	}

	DirectX::XMFLOAT4 loc4{ location.x, location.y, location.z, 1.0f };

	loc4 = matrix4 * loc4;

	location.x = loc4.x / loc4.w;
	location.y = loc4.y / loc4.w;
	location.z = loc4.z / loc4.w;
	
	return 0;
}

int Model::Enlarge(float ratio)
{
	size = ratio;
	return 0;
}

void Model::Render(DirectX::XMMATRIX& projector)
{
	if (!context || !swapChain || !verticies.size())
		return;

	unsigned int stride = 0, offset = 0;
	context->VSSetShader(vertexShader,0,0);
	context->PSSetShader(pixelShader,0,0);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	if (verticies.size() == texels.size() && modType == mod_type_vertex_texel)
	{
		stride = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2);
		context->PSSetShaderResources(0, 1, &colorMap);
		context->PSSetSamplers(0, 1, &sampleState);
	}
	else if(modType == mod_type_vertex_color)
	{
		stride = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT4);
	}
	else if (modType == mod_type_single_color)
	{
		stride = sizeof(DirectX::XMFLOAT3);
		context->UpdateSubresource(engine->getColorConstant(), 0, 0, &singleColor, 0, 0);
		ID3D11Buffer* gc = engine->getColorConstant();
		context->PSSetConstantBuffers(0, 1, &gc);

	}
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(indicees.size(), 0, 0);
}

bool Model::generateIndex()
{
	if (!engine)
		return false;
	ID3D11Device* dev = engine->getDeviceD();
	if (!dev)
		return false;

	D3D11_BUFFER_DESC vDesc;
	ZeroMemory(&vDesc, sizeof(vDesc));
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vDesc.ByteWidth = sizeof(int);

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = indicees.data();


	HRESULT res = dev->CreateBuffer(&vDesc, &rd, &indexBuffer);

	return SUCCEEDED(res);
}

ID3D11VertexShader * getVertexShader(ModelType mt)
{
	HRESULT res;
	ID3DBlob* error = nullptr;
	switch (mt)
	{
	case mod_type_single_color:
		
		break;
	case mod_type_vertex_color:

		break;
	case mod_type_vertex_texel:

		break;
	default:
		return nullptr;
	}
}

ID3D11PixelShader * getPixelShader(ModelType mt)
{
	switch (mt)
	{
	case mod_type_single_color:

		break;
	case mod_type_vertex_color:

		break;
	case mod_type_vertex_texel:

		break;
	default:
		return nullptr;
	}
}

void PrepShaderNamespace()
{
	static bool Preped = false; // false only during first run
	if (Preped)
		return;

	single_color_pixel_shader = vertex_color_pixel_shader 
		= texel_color_pixel_shader = nullptr;
	single_color_vertex_shader = vertex_color_vertex_shader
		= texel_color_vertex_shader = nullptr;
	Preped = true;
}
