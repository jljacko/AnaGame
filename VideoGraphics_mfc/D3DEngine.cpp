#include "stdafx.h"
#include <vector>
#include "D3DEngine.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
//#include <>
#include <wchar.h>
#include "Model.h"

std::vector<EngineID> engineList;


D3DEngine::D3DEngine()
{
	//Location = r;
	isInit = false;
	driver = D3D_DRIVER_TYPE_NULL;    // System will pick best configuration
	version = D3D_FEATURE_LEVEL_11_0; // Target Windows 7 and above
	graphicsDevice = NULL;
	contextDevice = NULL;
	swapper = NULL;
	renderTarget = NULL;
	ModelCount = 0;
	color = nullptr;
	single_color_pixel_shader = vertex_color_pixel_shader
		= texel_color_pixel_shader = nullptr;
	single_color_vertex_shader = vertex_color_vertex_shader
		= texel_color_vertex_shader = nullptr;
}


D3DEngine::~D3DEngine()
{
	if (graphicsDevice)
		graphicsDevice->Release();
	if (contextDevice)
		contextDevice->Release();
	if (swapper)
		swapper->Release();
	if (renderTarget)
		renderTarget->Release();
	graphicsDevice = NULL;
	contextDevice = NULL;
	swapper = NULL;
	renderTarget = NULL;

	if (color)
		color->Release();
	color = nullptr;

	if (texel_color_pixel_shader)
		texel_color_pixel_shader->Release();
	if (texel_color_vertex_shader)
		texel_color_vertex_shader->Release();
	if (vertex_color_pixel_shader)
		vertex_color_pixel_shader->Release();
	if (vertex_color_vertex_shader)
		vertex_color_vertex_shader->Release();
	if (single_color_pixel_shader)
		single_color_pixel_shader->Release();
	if (single_color_vertex_shader)
		single_color_vertex_shader->Release();

	single_color_vertex_shader = nullptr;
	vertex_color_vertex_shader = nullptr;
	texel_color_vertex_shader = nullptr;
	single_color_pixel_shader = nullptr;
	vertex_color_pixel_shader = nullptr;
	texel_color_pixel_shader = nullptr;
}

std::shared_ptr<D3DEngine> D3DEngine::Get3DEngine(WCHAR * d, HWND w, HINSTANCE i)
{
	if (!d)
		return std::shared_ptr<D3DEngine>(nullptr);
	for (int c = 0; c < engineList.size(); c++)
	{
		if (!StrCmpW(d, engineList[c].key))
		{
			engineList[c].count++;
			return engineList[c].engine;
		}
	}
	EngineID id;
	id.key = new WCHAR[wcslen(d) + 2];
	id.key[wcslen(d) + 1] = 0;
	wcsncpy_s(id.key, wcslen(id.key), d,wcslen(d));
	id.engine = std::make_shared<D3DEngine>();
	if (id.engine.get())
	{	
		engineList.push_back(id);
		id.engine->initialize(w, i);
		id.count = 1;
	}

	return id.engine;
}

D3D11_VIEWPORT * D3DEngine::getViewPort(WCHAR * d, RECT & r)
{
	
	for (int c = 0; c < engineList.size(); c++)
	{
		if (!StrCmpW(d, engineList[c].key))
		{
			if (!engineList[c].engine->getDevice())
				return nullptr;

			D3D11_VIEWPORT* vp = new D3D11_VIEWPORT;
			if (!vp)
				return vp;

			vp->Height = r.bottom - r.top;
			vp->Width = r.right - r.left;
			vp->MaxDepth = 1.0f;
			vp->MinDepth = 0.0f;
			vp->TopLeftX = static_cast<float>(r.left);
			vp->TopLeftY = static_cast<float>(r.top);

			engineList[c].engine->getDevice()->RSSetViewports(1, vp);
			return vp;
		}
			
	}
	return nullptr;
}

void D3DEngine::Release()
{
	for (std::vector<EngineID>::iterator c = engineList.begin(); c != engineList.end(); c++)
	{
		if (c->engine.get() == this)
		{
			(c->count)--;
			if (!(c->count))
			{
				std::shared_ptr<D3DEngine> en = c->engine;
				delete[] c->key;
				c->key = nullptr;
				engineList.erase(c);
				c->engine.reset();
			}
		}
	}

}

int D3DEngine::initialize(HWND w, HINSTANCE i)
{
	window = w;
	instance = i;
	
	GetClientRect(window, &Location);

	unsigned int width = Location.right - Location.left,
		height = Location.bottom-Location.top;

	D3D_DRIVER_TYPE dTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE//, D3D_DRIVER_TYPE_WARP
	};
	int tTypes = ARRAYSIZE(dTypes);
	D3D_FEATURE_LEVEL dLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};
	int tLevels = ARRAYSIZE(dLevels);


	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));
	// Initialize the swap cahin
	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Width = Location.right - Location.left;
	swapChainDescription.BufferDesc.Height = Location.bottom - Location.top;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 30;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = w;
	swapChainDescription.Windowed = true;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	unsigned int flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT results;
	
	results = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags,
		0, 0, D3D11_SDK_VERSION, &graphicsDevice, &version, &contextDevice);


	if (FAILED(results))
	{
		return NO_DEVICE;
	}

	IDXGIDevice* dxDev = nullptr;
	IDXGIAdapter* dxAdpt = nullptr;
	IDXGIFactory* dxFact = nullptr;
	results = (graphicsDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxDev));
	if (FAILED(results))
	{
		graphicsDevice->Release();
		contextDevice->Release();
		graphicsDevice = nullptr;
		contextDevice = nullptr;
		return NO_DEVICE;
	}

	results = dxDev->GetParent(__uuidof(IDXGIAdapter), (void**)&dxAdpt);
	if (FAILED(results))
	{
		graphicsDevice->Release();
		contextDevice->Release();
		dxDev->Release();
		graphicsDevice = nullptr;
		contextDevice = nullptr;
		return NO_DEVICE;
	}
	results = dxAdpt->GetParent(__uuidof(IDXGIFactory), (void**)&dxFact);


	if (FAILED(results))
	{

			graphicsDevice->Release();
			contextDevice->Release();
			dxDev->Release();
			dxAdpt->Release();
			graphicsDevice = nullptr;
			contextDevice = nullptr;
			return NO_DEVICE;
		
		
	}

	results = dxFact->CreateSwapChain(graphicsDevice, &swapChainDescription, &swapper);

	if (FAILED(results))
	{

		graphicsDevice->Release();
		contextDevice->Release();
		dxDev->Release();
		dxAdpt->Release();
		dxFact->Release();
		graphicsDevice = nullptr;
		contextDevice = nullptr;
		return NO_DEVICE;


	}

	ID3D11Texture2D* texture;

	results = swapper->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture);
	if (FAILED(results))
	{
		return NO_SWAP_CHAIN;
	}
	results = graphicsDevice->CreateRenderTargetView(texture, 0, &renderTarget);

	if (texture)
		texture->Release();

	if (FAILED(results))
	{
		return NO_RENDER_TARGET;
	}

	contextDevice->OMSetRenderTargets(1, &renderTarget, 0);

	// Create Swap Chain

	isInit = true;
	return NO_ERROR;
}

bool D3DEngine::isInitialized()
{
	return isInit;
}

void D3DEngine::AddModel(Model * m)
{
	if (m)
		models.push_back(m);
}

void D3DEngine::RenderScene(DirectX::XMMATRIX& m, D3D11_VIEWPORT& viewPort)
{
	if (contextDevice)
		contextDevice->RSSetViewports(1, &viewPort);
	for (int c = 0; c < models.size();c++)
	{
		if (models[c])
			models[c]->Render(m);
	}
}

ID3D11VertexShader * D3DEngine::getVertexShader(ModelType mt)
{
	switch (mt)
	{
	case mod_type_single_color:
		PrepShader(&single_color_vertex_shader);
		return single_color_vertex_shader;
	case mod_type_vertex_color:
		PrepShader(&vertex_color_vertex_shader);
		return vertex_color_vertex_shader;
	case mod_type_vertex_texel:
		PrepShader(&texel_color_vertex_shader);
		return texel_color_vertex_shader;
	default:
		return nullptr;
	}
	
}

ID3D11PixelShader * D3DEngine::getPixelShader(ModelType mt)
{
	switch (mt)
	{
	case mod_type_single_color:
		PrepShader(&single_color_pixel_shader);
		return single_color_pixel_shader;
	case mod_type_vertex_color:
		PrepShader(&vertex_color_pixel_shader);
		return vertex_color_pixel_shader;
	case mod_type_vertex_texel:
		PrepShader(&texel_color_pixel_shader);
		return texel_color_pixel_shader;
	default:
		return nullptr;
	}
}

ID3D11DeviceContext * D3DEngine::getDevice()
{
	
	return contextDevice;
}

IDXGISwapChain * D3DEngine::getSwapChain()
{
	return swapper;
}

ID3D11RenderTargetView * D3DEngine::getRederTarget()
{
	return renderTarget;
}

ID3D11Device * D3DEngine::getDeviceD()
{
	return graphicsDevice;
}

ID3D11Buffer * D3DEngine::getColorConstant()
{
	return color;
}

void D3DEngine::PrepShader(ID3D11VertexShader** vs)
{
	if (*vs) // It's been initialize, just return
		return;

	if (!graphicsDevice)
		return;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;
	HRESULT res;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	if (vs == &single_color_vertex_shader)
	{
		res = D3DCompileFromFile(L"sColor.fx", 0, 0, "VS_Main", "vs_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else if (vs == &vertex_color_vertex_shader)
	{
		res = D3DCompileFromFile(L"vColor.fx", 0, 0, "VS_Main", "vs_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else if (vs == &texel_color_vertex_shader)
	{
		res = D3DCompileFromFile(L"tColor.fx", 0, 0, "VS_Main", "vs_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else
		return;

	if (SUCCEEDED(res))
	{
		graphicsDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, vs);
	}
}

void D3DEngine::PrepShader(ID3D11PixelShader** ps)
{
	if (*ps) // It's been initialized, just return
		return;

	if (!graphicsDevice)
		return;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;
	HRESULT res;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	if (ps == &single_color_pixel_shader)
	{
		res = D3DCompileFromFile(L"sColor.fx", 0, 0, "PS_Main", "ps_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else if (ps == &vertex_color_pixel_shader)
	{
		res = D3DCompileFromFile(L"vColor.fx", 0, 0, "PS_Main", "ps_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else if (ps == &texel_color_pixel_shader)
	{
		res = D3DCompileFromFile(L"tColor.fx", 0, 0, "PS_Main", "ps_4_0", flags, 0, &buffer, &errorBuffer);
	}
	else
		return;

	if (SUCCEEDED(res))
	{
		graphicsDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, ps);
		if (ps == &single_color_pixel_shader)
		{
			D3D11_BUFFER_DESC constDesc;
			ZeroMemory(&constDesc, sizeof(constDesc));
			constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
			constDesc.Usage = D3D11_USAGE_DEFAULT;

			HRESULT d3dResult = graphicsDevice->CreateBuffer(&constDesc, 0, &color);

			
		}
	}

}
