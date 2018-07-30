#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

#include <d3d11shader.h>

#include "D3DEngine.h"

// Since Visual Studio refuses to properly process DirectXMath.h
/*struct _VIDEO_GRAPHICS XMFLOAT3
{
	float x;
	float y;
	float z;

	XMFLOAT3() {}
	XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	explicit XMFLOAT3(_In_reads_(3) const float *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

	XMFLOAT3& operator= (const XMFLOAT3& Float3) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
};
*/



class _VIDEO_GRAPHICS Model
{
public:
	Model(D3DEngine&);
	Model(ModelType, D3DEngine&);
	~Model();

	// Settup and shutdown
	HRESULT LoadModel(CArchive& arch);
	bool PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, std::vector<DirectX::XMFLOAT4>& c);
	bool PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, std::vector<DirectX::XMFLOAT2>& t, CFile& f);
	bool PrepModelWithSupplies(std::vector<DirectX::XMFLOAT3>& v, DirectX::XMFLOAT4 c);
	void PrepIndicees(std::vector<int>& i);
	void UnloadModel();

	// Transformations
	int Rotate(float radian, DirectX::XMFLOAT3 direction);
	int Translate(float degree, DirectX::XMFLOAT3 direction);
	int Enlarge(float ratio);
	
	// Draw
	void Render(DirectX::XMMATRIX& projector);

protected:
	bool generateIndex();


	// Location information
	Model* parentModel;
	std::vector<Model*> childModels;
	std::vector<DirectX::XMFLOAT3> verticies;
	std::vector<DirectX::XMFLOAT4> colors;
	std::vector<DirectX::XMFLOAT2> texels;
	std::vector<int> indicees;
	DirectX::XMFLOAT3 location, direction;
	float size;
	bool set;

	// Color resources
	ID3D11ShaderResourceView* colorMap;
	ID3D11SamplerState* sampleState;
	DirectX::XMFLOAT4 singleColor;
	

	// Shader Resources
	ModelType modType;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	D3DEngine* engine;

	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTarget;
	ID3D11Device* device;

	// Other information
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
};

// Get Shaders according to model type

void PrepShaderNamespace();