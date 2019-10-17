#pragma once
#define D3D_DEBUG_INFO
#include <TObject.h>
#include <TString.h>
#include <TDataArray.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "VideoGraphics.h"
#include <TrecReference.h>
#include <vector>

class ArenaModel;

// Create Macros for error
#define NO_ERROR         0
#define NO_DEVICE        1
#define NO_SWAP_CHAIN    2
#define NO_RENDER_TARGET 3

// Macros for keeping track of non-boolean rasterization
#define CULL_MODE unsigned char
#define CULL_NONE  0
#define CULL_BACK  2
#define CULL_FRONT 1

// Default Shader Count, since AnaGame will provide it's own shaders
// the Actual number being unknown as of right now
#define ANAGAME_DAFAULT_SHADER_COUNT 5

/*
* enum ShaderPhase
* Used to keep track of what shader type is being discussed
*/
typedef enum ShaderPhase
{
	sp_Vertex,
	sp_Compute,
	sp_Hull,
	sp_Domain,
	sp_Geometry,
	sp_Pixel
}ShaderPhase;

/*
* enum DefaultShader
* Mark for a built-in AnaGame Shader
*/
typedef enum _VIDEO_GRAPHICS DefaultShader
{
	default_shader_Single_Color = 0,
	default_shader_3D_Color = 1,
	default_Shader_4D_Color = 2,
	default_Shader_Texture = 3,
	default_Shader_Texture2 = 4,
}DefaultShader;

/*
* struct ConstantBufferMark
* Holds a shader's constant buffer as well as a label signaling what that buffer is 
*/
typedef struct ConstantBufferMark
{
	unsigned char label;
	TrecComPointer<ID3D11Buffer> buff;
}ConstantBufferMark;

/*
* struct ShaderProgram
* Holds all of the resources that constitute a shader as well as indexes to imprtant 
*	resources within the shader
*/
typedef struct ShaderProgram
{
	TrecComPointer<ID3D11VertexShader> vs;
	TrecComPointer<ID3D11ComputeShader> cs;
	TrecComPointer<ID3D11DomainShader> ds;
	TrecComPointer<ID3D11GeometryShader> gs;
	TrecComPointer<ID3D11HullShader> hs;
	TrecComPointer<ID3D11PixelShader> ps;
	D3D11_INPUT_ELEMENT_DESC* elements;
	unsigned char elementCount;
	int bufferSize;
	TrecComPointer<ID3D11InputLayout> layout;
	HRESULT layoutError;
	TDataArray<ConstantBufferMark> constantBuffers;
	signed char cameraLoc;
	signed char colorLoc;
	signed char modelLoc;
	signed char viewLoc;
	signed char TextureCount;
	bool mvp_cpu;
}ShaderProgram;

/*
* struct DefaultShaderProgram
* Holds a Default shader
*/
typedef struct DefaultShaderProgram
{
	ShaderProgram sp;
	DefaultShader type;
}DefaultShaderProgram;

/*
* struct ShaderLayoutDesc
* Acts as a primitive array for the input element descriptors
*/
typedef struct ShaderLayoutDesc
{
	D3D11_INPUT_ELEMENT_DESC* elements;
	unsigned char elementCount;
}ShaderLayoutDesc;


/*
* Class ArenaEngine
* Holds core resources for a 3D set-up
*/
class _VIDEO_GRAPHICS ArenaEngine :	public TObject
{
	friend class ShaderParser;
	friend class ArenaModel;
	friend class TrecPointerKey;
public:
	ArenaEngine();
	~ArenaEngine();

	int InitializeDefaultShaders();
	bool IsDefaultShadersReady();

	// Shader Creater Methods
	int SetNewBasicShader(TString& vs, TString& ps, const CHAR* vf, const CHAR* sf, TDataArray<unsigned short>& bufferDesc);
	int SetNewBasicShader(TString& s, const CHAR* vf, const CHAR* sf, TDataArray<unsigned short>& bufferDesc);

	int AddComputeShader(int shaderID, TString& cs, const CHAR* cf);
	int AddDomainShader(int shaderID, TString& ds, const CHAR* df);
	int AddGeometryShader(int shaderID, TString& gs, const CHAR* gf);
	int AddHullShader(int shaderID, TString& hs, const CHAR* hf);

	// Shader Modification Methods
	bool setShader(int shaderID);
	bool setShader(DefaultShader id);
	ShaderLayoutDesc getLayoutDesc(int shaderID);
	ShaderLayoutDesc getLayoutDesc(DefaultShader ds);
	int getBufferSize(int shaderID);
	int getBufferSize(DefaultShader ds);
	//bool UpdateConstantBuffer(int shaderID, int BufferLoc, ID3D11Buffer* buff);
	//bool UpdateConstantBuffer(DefaultShader shader, int BufferLoc, ID3D11Buffer* buff);

	bool GetLayoutError(int shaderID, HRESULT& res);
	bool GetLayoutError(DefaultShader ds, HRESULT& res);
	bool CompareElementLayout(int shaderID, D3D11_INPUT_ELEMENT_DESC* des, int desSize);
	bool CompareElementLayout(DefaultShader shader, D3D11_INPUT_ELEMENT_DESC* des, int desSize);

	bool SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, int shaderID, unsigned char purp);
	bool SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, DefaultShader ds, unsigned char purp);
	bool SetCameraBuffer(int shaderID,int cameraLocation);
	bool SetCameraBuffer(DefaultShader ds, int cameraLocation);

	signed char GetTextureCount(int shaderID);
	signed char GetTextureCount(DefaultShader shader);

	signed char getColorBufferLocation(int shaderID);
	signed char getColorBufferLocation(DefaultShader shader);

	// Handle Rasterization Conditions
	void ModelSetFillMode(D3D11_FILL_MODE fill);
	void ModelSetCullMode(D3D11_CULL_MODE cull);
	void SetDefaultRasterization();

	int GetConstantBuffer(int size, TrecComPointer<ID3D11Buffer>& buff);
	void UpdateConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, ShaderPhase sp, int slot);

	int ReplaceConstantBuffer(int shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);
	int ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, TrecComPointer<ID3D11Buffer> buff);


	// Basic Methods, imported from D3DEngine
	static TrecPointer<ArenaEngine> GetArenaEngine(TString& d, HWND w, HINSTANCE h);
	static D3D11_VIEWPORT* getViewPort(WCHAR* d, RECT& r);
	void Release();

	UCHAR initialize(HWND, HINSTANCE);
	bool isInitialized();

	void SetResources(TrecComPointer<ID3D11DeviceContext>& c,
		TrecComPointer<IDXGISwapChain>& swapChain,
		TrecComPointer<ID3D11RenderTargetView>& renderTarget,
		TrecComPointer<ID3D11Device>& device);

	// Direct3D Get Methods
	TrecComPointer<ID3D11DeviceContext> getDevice();
	TrecComPointer<IDXGISwapChain> getSwapChain();
	TrecComPointer<ID3D11RenderTargetView> getRederTarget();
	TrecComPointer<ID3D11Device> getDeviceD();
	TrecComPointer<IDXGIDevice> getDeviceD_U();

	// TO-Do: Add Model Method
	bool DoMVP();

	
	

	// Rndering scene
	void PrepareScene(D2D1::ColorF color);
	void RenderScene(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam, D3D11_VIEWPORT& viewPort);
	void FinalizeScene();

	signed char GetModelLocation(int shaderID);
	signed char GetModelLocation(DefaultShader ds);
	signed char GetViewLocation(int shaderID);
	signed char GetViewLocation(DefaultShader ds);
	signed char GetCameraLocation(int shaderID);
	signed char GetCameraLocation(DefaultShader ds);

	virtual UCHAR* GetAnaGameType()override;

	UCHAR OnWindowResize();

	TDataArray<ArenaModel*>* getModelList();
	
private:
	void Unload();
	bool defaultShadersSet;
	void UnloadAnaGameShaders();

	ShaderProgram defaultShaders[ANAGAME_DAFAULT_SHADER_COUNT];
	ArenaEngine(ArenaEngine& ae);

	// View Buffer
	DirectX::XMMATRIX viewBuffer;

	// Basic Shader Information
	int SetNewBasicShader(TString& s, const CHAR* f, TDataArray<unsigned short>& bufferDesc, bool isVertex, ShaderProgram& sp);
	TDataArray<D3D11_INPUT_ELEMENT_DESC>* getInputDescription(TDataArray<unsigned short>& bufferDesc, int& buffSize);
	void assignDescriptionFormat(D3D11_INPUT_ELEMENT_DESC& desc, unsigned char valueSize, unsigned short valueCount);
	TDataArray<ShaderProgram> shaderList;

	// Model Methods
	void AddModel(ArenaModel& ae);
	void RemoveModel(ArenaModel* ae);
	// Functions and data for setting up Default Shaders
	bool SetNewBasicShader(TString& vs, TString& ps, const CHAR* vf, const CHAR* sf, TDataArray<unsigned short>& bufferDesc, DefaultShader ds);
	bool GetMostRecentBasicShader(DefaultShader& ds);

	DefaultShader mostRecentDefault;
	bool mostRecentWorked;

	// Basic Direct 3D resources
	TrecComPointer<ID3D11Device> graphicsDevice;
	TrecComPointer<IDXGIDevice> dxDev;
	TrecComPointer<IDXGISurface> surface;
	TrecComPointer<ID3D11DeviceContext> contextDevice;
	TrecComPointer<IDXGISwapChain> swapper;
	TrecComPointer<ID3D11RenderTargetView> renderTarget;
	D3D_DRIVER_TYPE driver;
	D3D_FEATURE_LEVEL version;

	// Basic Object Resources
	RECT Location;
	HWND window;
	HINSTANCE instance;
	bool isInit;



	// TO-Do: Add Model collection
	TDataArray<ArenaModel*> models;
	DirectX::XMMATRIX currentCameraProj;
	DirectX::XMMATRIX camera;
	bool isDefaultInit;

	// Default Shaders
	// ShaderProgram defaultShaders[ANAGAME_DAFAULT_SHADER_COUNT];
	D3D11_RASTERIZER_DESC rasterizer;
	/*TrecComPointer<ID3D11RasterizerState> solidNoCull;
	TrecComPointer<ID3D11RasterizerState> solidCullFront;
	TrecComPointer<ID3D11RasterizerState> solidCullBack;
	TrecComPointer<ID3D11RasterizerState> wiredNoCull;
	TrecComPointer<ID3D11RasterizerState> wiredCullFront;
	TrecComPointer<ID3D11RasterizerState> wiredCullBack;*/

	TrecComPointer<ID3D11RasterizerState> rasterizerState;
	bool isFill; // Solid or wire-frame?
	CULL_MODE cullMode;



	int AddComputeShader(DefaultShader ds, TString& cs, const CHAR* cf);
	int AddDomainShader(DefaultShader s, TString& ds, const CHAR* df);
	int AddGeometryShader(DefaultShader ds, TString& gs, const CHAR* gf);
	int AddHullShader(DefaultShader ds, TString& hs, const CHAR* hf);

	void SetTextureCount(int shaderID, unsigned char c);
	void SetTextureCount(DefaultShader shaderID, unsigned char c);

	void ResetShaderProgram(ShaderProgram& sp);

	bool NeedsMVPCalculation;
	bool Initialized;

	TString name;
};

