#pragma once
#include <TObject.h>
//#include <d3d.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "VideoGraphics.h"
#include <vector>

class Model;

// Create Macros for error
#define NO_ERROR         0
#define NO_DEVICE        1
#define NO_SWAP_CHAIN    2
#define NO_RENDER_TARGET 3

// Determines the Model type that influences the shaders to use and the data to send
typedef enum ModelType
{
	mod_type_Unset,
	mod_type_single_color,
	mod_type_vertex_color,
	mod_type_vertex_texel,

}ModelType;



class _VIDEO_GRAPHICS D3DEngine : public TObject
{
public:
	D3DEngine();
	~D3DEngine();

	static std::shared_ptr<D3DEngine> Get3DEngine(WCHAR* d, HWND w, HINSTANCE h);
	static D3D11_VIEWPORT* getViewPort(WCHAR* d, RECT& r);
	void Release();

	int initialize(HWND, HINSTANCE);
	bool isInitialized();

	void AddModel(Model*);

	void RenderScene(DirectX::XMMATRIX&, D3D11_VIEWPORT& viewPort);

	ID3D11VertexShader* getVertexShader(ModelType mt);
	ID3D11PixelShader* getPixelShader(ModelType mt);

	ID3D11DeviceContext* getDevice();
	IDXGISwapChain* getSwapChain();
	ID3D11RenderTargetView* getRederTarget();
	ID3D11Device* getDeviceD();
	ID3D11Buffer* getColorConstant();
protected:
	RECT Location;
	HWND window;
	HINSTANCE instance;
	bool isInit;

	long ModelCount;
	
	void PrepShader(ID3D11VertexShader** vs);
	void PrepShader(ID3D11PixelShader** ps);
	// Basic Direct 3D resources
	ID3D11Device* graphicsDevice;
	ID3D11DeviceContext* contextDevice;
	IDXGISwapChain* swapper;
	ID3D11RenderTargetView* renderTarget;
	D3D_DRIVER_TYPE driver;
	D3D_FEATURE_LEVEL version;

	std::vector<Model*> models;

	// Various shaders depending on model required
	ID3D11VertexShader* single_color_vertex_shader;
	ID3D11VertexShader*	vertex_color_vertex_shader;
	ID3D11VertexShader*	texel_color_vertex_shader;
	ID3D11PixelShader* single_color_pixel_shader;
	ID3D11PixelShader* vertex_color_pixel_shader;
	ID3D11PixelShader* texel_color_pixel_shader;

	// Various peices that enables communication with the 
	ID3D11Buffer* color;
};

typedef struct EngineID
{
	WCHAR* key;
	std::shared_ptr<D3DEngine> engine;
	int count;
}EngineID;