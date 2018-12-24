#include "stdafx.h"
#include "ArenaEngine.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "ArenaModel.h"
#include "ShaderParser.h"
#include <TML_Reader_.h>
#include <DirectoryInterface.h>

TDataArray<ArenaEngine*> engineList;

const CHAR* sem_V = "POSITION";
const CHAR* sem_BN = "BINORMAL";
const CHAR* sem_BW = "BLENDWEIGHT";
const CHAR* sem_BI = "BLENDINDICES";
const CHAR* sem_T = "TEXCOORD";
const CHAR* sem_N = "NORMAL";
const CHAR* sem_C = "COLOR";
const CHAR* sem_PT = "POSITIONT";
const CHAR* sem_TA = "TANGENT";
const CHAR* sem_F = "FOG";
const CHAR* sem_TS = "TESSFACTOR";

/*
* Method: ArenaEngine
* Purpose: 
* Parameters:
* Return:  
*/

static ArenaEngine* TheEngine = nullptr;
static int EngineReferenceCount = 0;
static int InitializationError = 0;


void ArenaEngine::UnloadAnaGameShaders()
{
	for (UINT c = 0; c < ANAGAME_DAFAULT_SHADER_COUNT; c++)
	{
		defaultShaders[c].vs = nullptr;
		defaultShaders[c].ps = nullptr;
		defaultShaders[c].cs = nullptr;
		defaultShaders[c].ds = nullptr;
		defaultShaders[c].gs = nullptr;
		defaultShaders[c].hs = nullptr;

		if (defaultShaders[c].elements)
			delete[] defaultShaders[c].elements;
		defaultShaders[c].elements = nullptr;

		defaultShaders[c].layout = nullptr;

	}
}

ArenaEngine::ArenaEngine(ArenaEngine& ae)
{
	defaultShadersSet = false;
	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;

	graphicsDevice = ae.getDeviceD();
	contextDevice = ae.getDevice();
	swapper = ae.getSwapChain();
	renderTarget = ae.getRederTarget();
	dxDev = ae.getDeviceD_U();
	viewBuffer = DirectX::XMMatrixIdentity();
	//InitializeDefaultShaders();
}

/*
* Method: ArenaEngine (Constructor)
* Purpose: Sets up a clean Arena Engine
* Parameters: void
* Return: void
*/
ArenaEngine::ArenaEngine()
{
	defaultShadersSet = false;
	/*
	isInit = false;*/
	driver = D3D_DRIVER_TYPE_NULL;    // System will pick best configuration
	version = D3D_FEATURE_LEVEL_11_0; // Target Windows 7 and above
	graphicsDevice = NULL;
	contextDevice = NULL;
	swapper = NULL;
	renderTarget = NULL;
	Initialized = false;

	// Zero the Memory



	// Prepare Rasterization States
	
	ZeroMemory(&rasterizer, sizeof(rasterizer));
	rasterizer.FillMode = D3D11_FILL_SOLID;
	rasterizer.CullMode = D3D11_CULL_BACK;
	//rasterizer.DepthClipEnable = TRUE;

	/*solidNoCull = nullptr;
	solidCullFront = nullptr;
	solidCullBack = nullptr;
	wiredNoCull = nullptr;
	wiredCullFront = nullptr;
	wiredCullBack = nullptr;*/

	isDefaultInit = false;

	graphicsDevice = nullptr;
	contextDevice = nullptr;
	swapper = nullptr;
	renderTarget = nullptr;
	dxDev = nullptr;
	viewBuffer = DirectX::XMMatrixIdentity();
	//InitializeDefaultShaders();
}

/*
* Method: ArenaEngine (Destructor)
* Purpose: Unloads the Engine from the engine list and nullifies all resourses
* Parameters: void
* Return: void
*/
ArenaEngine::~ArenaEngine()
{

	Unload();
	for (UINT c = 0; c < engineList.Size(); c++)
	{
		if (engineList[c] == this)
		{
			engineList.RemoveAt(c);
			break;
		}
	}

	if (!engineList.Size())
	{
		UnloadAnaGameShaders();
		defaultShadersSet = false;
	}
}

/*
* Method: ArenaEngine - InitializeDefaultShaders
* Purpose: Sets up the built-in AnaGame shaders if not done already
* Parameters: void
* Return: int - 0 if successful, error code otherwise
*/
int ArenaEngine::InitializeDefaultShaders()
{
	if (defaultShadersSet)
		return 0;
	WCHAR* directory = nullptr;
	HRESULT res = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &directory);

	if (!directory || FAILED(res))
		return 1;

	TString tDirectory = GetDirectoryWithSlash(cd_Executable);

	tDirectory += L"\\Resources\\DefaultShaders";

	CFile* file = nullptr;
	CArchive* arch = nullptr;
	TML_Reader_* read = nullptr;

	TString tempDir;


	// Prepare to Read the Single Color Shader

	tempDir = tDirectory;
	tempDir += L"\\SingleColor";
	ShaderParser* parse = new ShaderParser(*this, &tempDir);
	tempDir += L"\\SingleColor.tml";


	try {
		file = new CFile(tempDir.GetBuffer(), CFile::modeRead);
		arch = new CArchive(file, CArchive::load);
		TML_Reader_* read = new TML_Reader_(arch,parse);
		int error = 0;
		parse->SetDefaultShader(default_shader_Single_Color);
		if (!read->read(&error))
		{
			arch->Close();
			file->Close();
			delete arch;
			delete file;
			delete read;
			delete parse;
			return -2;
		}
	}
	catch (CInvalidArgException& e)
	{

		return 2;
	}

	arch->Close();
	file->Close();
	delete arch;
	delete file;
	delete read;
	delete parse;
	arch = nullptr;
	file = nullptr;
	read = nullptr;
	parse = nullptr;

	// Prepare to read the 3D Color Vertex Shader


	tempDir = tDirectory;
	tempDir += L"\\3DColor";
	parse = new ShaderParser(*this, &tempDir);
	tempDir += L"\\3DColor.tml";


	try {
		file = new CFile(tempDir.GetBuffer(), CFile::modeRead);
		arch = new CArchive(file, CArchive::load);
		TML_Reader_* read = new TML_Reader_(arch, parse);
		int error = 0;
		parse->SetDefaultShader(default_shader_3D_Color);
		if (!read->read(&error))
		{
			arch->Close();
			file->Close();
			delete arch;
			delete file;
			delete read;
			delete parse;
			return -3;
		}
	}
	catch (CInvalidArgException& e)
	{

		return 3;
	}
	arch->Close();
	file->Close();
	delete arch;
	delete file;
	delete read;
	delete parse;
	arch = nullptr;
	file = nullptr;
	read = nullptr;
	parse = nullptr;

	// Prepare to Read the 4D Color Shader


	tempDir = tDirectory;
	tempDir += L"\\4DColor";
	parse = new ShaderParser(*this, &tempDir);
	tempDir += L"\\4DColor.tml";


	try {
		file = new CFile(tempDir.GetBuffer(), CFile::modeRead);
		arch = new CArchive(file, CArchive::load);
		TML_Reader_* read = new TML_Reader_(arch, parse);
		int error = 0;
		parse->SetDefaultShader(default_Shader_4D_Color);
		if (!read->read(&error))
		{
			arch->Close();
			file->Close();
			delete arch;
			delete file;
			delete read;
			delete parse;
			return -4;
		}
	}
	catch (CInvalidArgException& e)
	{

		return 4;
	}
	arch->Close();
	file->Close();
	delete arch;
	delete file;
	delete read;
	delete parse;
	arch = nullptr;
	file = nullptr;
	read = nullptr;
	parse = nullptr;

	// Prepare to read the one Texture Shader

	tempDir = tDirectory;
	tempDir += L"\\Texture1";
	parse = new ShaderParser(*this, &tempDir);
	tempDir += L"\\Texture1.tml";


	try {
		file = new CFile(tempDir.GetBuffer(), CFile::modeRead);
		arch = new CArchive(file, CArchive::load);
		TML_Reader_* read = new TML_Reader_(arch, parse);
		int error = 0;
		parse->SetDefaultShader(default_Shader_Texture);
		if (!read->read(&error))
		{
			arch->Close();
			file->Close();
			delete arch;
			delete file;
			delete read;
			delete parse;
			return -5;
		}
	}
	catch (CInvalidArgException& e)
	{

		return 5;
	}
	arch->Close();
	file->Close();
	delete arch;
	delete file;
	delete read;
	delete parse;
	arch = nullptr;
	file = nullptr;
	read = nullptr;
	parse = nullptr;

	// Prepare the Double Texture Shader
	tempDir = tDirectory;
	tempDir += L"\\Texture2";
	parse = new ShaderParser(*this, &tempDir);
	tempDir += L"\\Texture2.tml";


	try {
		file = new CFile(tempDir.GetBuffer(), CFile::modeRead);
		arch = new CArchive(file, CArchive::load);
		TML_Reader_* read = new TML_Reader_(arch, parse);
		int error = 0;
		parse->SetDefaultShader(default_Shader_Texture2);
		if (!read->read(&error))
		{
			arch->Close();
			file->Close();
			delete arch;
			delete file;
			delete read;
			delete parse;
			return -6;
		}
	}
	catch (CInvalidArgException& e)
	{

		return 6;
	}
	arch->Close();
	file->Close();
	delete arch;
	delete file;
	delete read;
	delete parse;
	arch = nullptr;
	file = nullptr;
	read = nullptr;
	parse = nullptr;

	isDefaultInit = true;
	defaultShadersSet = true;
	return 0;
}

/*
* Method: ArenaEngine - IsDefaultShadersReady
* Purpose: Reports whether the Default Shaders are prepared for use
* Parameters: void
* Return: bool - whether the shaders are initialized
*/
bool ArenaEngine::IsDefaultShadersReady()
{
	return defaultShadersSet;
}

/*
* Method: ArenaEngine - SetNewBasicShader
* Purpose: Prepares a shader's core components, use if pixel and vertex file are in seperate files
* Parameters: TString& vs - the path of the file with vertex shader to read
*				TString& ps - the path of the file with pixel shader to read
*				CHAR *vf - function name of the vertex shader
*				CHAR *sf - function name of the pixel shader
*				TDataArray<unsigned short>& bufferDesc - list of buffer information
* Return: int - location of the shader in the list if successful, 0 or negative otherwise
*/
int ArenaEngine::SetNewBasicShader(TString & vs, TString & ps, CHAR * vf, CHAR * sf, TDataArray<unsigned short>& bufferDesc)
{
	ShaderProgram newShader;
	ResetShaderProgram(newShader);

	if (!vf || !sf || !graphicsDevice.get())
		return -1;

	int test = SetNewBasicShader(vs, vf, bufferDesc, true, newShader);
	
	if (test)
		return test;
	test = SetNewBasicShader(ps, sf, bufferDesc, false, newShader);

	if (test)
	{
		//newShader.vs->Release();
		newShader.vs = nullptr;
		return test - 3;
	}



	shaderList.push_back(newShader);

	return shaderList.Size();
}

/*
* Method: ArenaEngine - SetNewBasicShader
* Purpose: Prepares a shader's core components, use if pixel and vertex shaders are in the same file
* Parameters: TString& s - the path of the file to read
*				CHAR *vf - function name of the vertex shader
*				CHAR *sf - function name of the pixel shader
*				TDataArray<unsigned short>& bufferDesc - list of buffer information
* Return: int
*/
int ArenaEngine::SetNewBasicShader(TString & s, CHAR * vf, CHAR * sf, TDataArray<unsigned short>& bufferDesc)
{
	if (!vf || !sf)
		return -1;

	ShaderProgram newShader;
	ResetShaderProgram(newShader);
	int test = SetNewBasicShader(s, vf, bufferDesc, true, newShader);

	if (test)
		return test;
	test = SetNewBasicShader(s, sf, bufferDesc, false, newShader);

	if (test)
	{
		//newShader.vs->Release();
		newShader.vs = nullptr;
		return test - 3;
	}

	shaderList.push_back(newShader);

	return shaderList.Size() - 1;
}

/*
* Method: ArenaEngine - AddComputeShader
* Purpose: Adds a Compute shader to the indexed Shader Module
* Parameters: int shaderID - the index of the shader to target
*				TString& cs - file holding the compute shader
*				CHAR* cf - function name of the compute shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddComputeShader(int shaderID, TString & cs, CHAR * cf)
{
	if (!cf)
		return 1;

	if (shaderID < 0 || shaderID >= shaderList.Size())
		return 2;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(cs.GetBuffer(), 0, 0, cf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11ComputeShader* css = nullptr;
	res = graphicsDevice->CreateComputeShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &css);
	shaderList[shaderID].cs = css;
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;


}

/*
* Method: ArenaEngine - AddDomainShader
* Purpose: Adds a Domain shader to the indexed Shader Module
* Parameters: int shaderID - the index of the shader to target
*				TString& ds - file holding the domain shader
*				CHAR* df - function name of the domain shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddDomainShader(int shaderID, TString & ds, CHAR * df)
{
	if (!df)
		return 1;

	if (shaderID < 0 || shaderID >= shaderList.Size())
		return 2;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(ds.GetBuffer(), 0, 0, df, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11DomainShader* d = nullptr;
	res = graphicsDevice->CreateDomainShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &d);
	shaderList[shaderID].ds = d;
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - AddGeometryShader
* Purpose: Adds a geometry shader to the indexed Shader Module
* Parameters: int shaderID - the index of the shader to target
*				TString& gs - file holding the geometry shader
*				CHAR* gf - function name of the geometry shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddGeometryShader(int shaderID, TString & gs, CHAR * gf)
{
	if (!gf)
		return 1;

	if (shaderID < 0 || shaderID >= shaderList.Size())
		return 2;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(gs.GetBuffer(), 0, 0, gf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11GeometryShader* g = nullptr;
	res = graphicsDevice->CreateGeometryShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &g);
	shaderList[shaderID].gs = g;
	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - AddHullShader
* Purpose: Adds a hull shader to the indexed Shader Module
* Parameters: int shaderID - the index of the shader to target
*				TString& hs - file holding the hull shader
*				CHAR* hf - function name of the hull shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddHullShader(int shaderID, TString & hs, CHAR * hf)
{
	if (!hf)
		return 1;

	if (shaderID < 0 || shaderID >= shaderList.Size())
		return 2;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(hs.GetBuffer(), 0, 0, hf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11HullShader* h = nullptr;
	res = graphicsDevice->CreateHullShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &h);
	shaderList[shaderID].hs = h;

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - setShader
* Purpose: Sets an external shader to Direct 3D to use for rendering
* Parameters: int shaderID - the index of the shader to use
* Return: bool - success
*/
bool ArenaEngine::setShader(int shaderID)
{
	if (!contextDevice.get())
		return false;
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return false;



	ShaderProgram* sp = &(shaderList[shaderID]);

	if (!sp->layout.get())
		return false;

	contextDevice->IASetInputLayout(sp->layout.get());

	contextDevice->CSSetShader(sp->cs.get(), 0, 0);
	contextDevice->DSSetShader(sp->ds.get(), 0, 0);
	contextDevice->GSSetShader(sp->gs.get(), 0, 0);
	contextDevice->HSSetShader(sp->hs.get(), 0, 0);
	contextDevice->PSSetShader(sp->ps.get(), 0, 0);
	contextDevice->VSSetShader(sp->vs.get(), 0, 0);

	if (sp->cameraLoc >= 0 && sp->cameraLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->cameraLoc].buff.get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->cameraLoc].buff);
		}
		if (sp->constantBuffers[sp->cameraLoc].buff.get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->cameraLoc].buff.get(), 0, 0, &currentCameraProj, 0, 0);

	}

	if (sp->viewLoc >= 0 && sp->viewLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->viewLoc].buff.get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->viewLoc].buff);
		}
		if (sp->constantBuffers[sp->viewLoc].buff.get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->viewLoc].buff.get(), 0, 0, &camera, 0, 0);
	}

	for (int c = 0; c < sp->constantBuffers.Size(); c++)
	{
		ConstantBufferMark cbm = sp->constantBuffers[c];
		unsigned char slot = cbm.label >> 4;
		unsigned char shaderType = cbm.label & 0b00000111;
		ID3D11Buffer* buff = nullptr;
		switch (shaderType)
		{
		case 1: // PixelShader
			UpdateConstantBuffer(&buff, sp_Pixel, slot);
			break;
		case 2: // GeometryShader
			UpdateConstantBuffer(&buff, sp_Geometry, slot);
			break;
		case 3: // HULL Shader
			UpdateConstantBuffer(&buff, sp_Hull, slot);
			break;
		case 4: // Compute Shader
			UpdateConstantBuffer(&buff, sp_Compute, slot);
			break;
		case 5: // Domain Shader
			UpdateConstantBuffer(&buff, sp_Domain, slot);
			break;
		default: // Vertex Shader
			UpdateConstantBuffer(&buff, sp_Vertex, slot);
		}
		cbm.buff = buff;
	}
	NeedsMVPCalculation = sp->mvp_cpu;
	


	return true;
	
}

/*
* Method: ArenaEngine - setShader
* Purpose: Sets a built-in shader to Direct 3D to use for rendering
* Parameters: DefaultShader id - the built-in shader to use
* Return: bool - success
*/
bool ArenaEngine::setShader(DefaultShader id)
{
	ShaderProgram* sp = &defaultShaders[static_cast<int>(id)];

	if (!sp->layout.get())
		return false;

	contextDevice->IASetInputLayout(sp->layout.get());

	contextDevice->CSSetShader(sp->cs.get(), 0, 0);
	contextDevice->DSSetShader(sp->ds.get(), 0, 0);
	contextDevice->GSSetShader(sp->gs.get(), 0, 0);
	contextDevice->HSSetShader(sp->hs.get(), 0, 0);
	contextDevice->PSSetShader(sp->ps.get(), 0, 0);
	contextDevice->VSSetShader(sp->vs.get(), 0, 0);

	if (sp->cameraLoc >= 0 && sp->cameraLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->cameraLoc].buff.get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->cameraLoc].buff);
		}
		if (sp->constantBuffers[sp->cameraLoc].buff.get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->cameraLoc].buff.get(), 0, 0, &currentCameraProj, 0, 0);

	}

	if (sp->viewLoc >= 0 && sp->viewLoc < sp->constantBuffers.Size())
	{
		if (!sp->constantBuffers[sp->viewLoc].buff.get())
		{
			GetConstantBuffer(sizeof(DirectX::XMMATRIX), sp->constantBuffers[sp->viewLoc].buff);
		}
		if (sp->constantBuffers[sp->viewLoc].buff.get())
			contextDevice->UpdateSubresource(sp->constantBuffers[sp->viewLoc].buff.get(), 0, 0, &camera, 0, 0);
	}

	

	for (UINT c = 0; c < sp->constantBuffers.Size(); c++)
	{
		ConstantBufferMark cbm = sp->constantBuffers[c];
		unsigned char slot = cbm.label >> 4;
		unsigned char shaderType = cbm.label & 0b00000111;
		ID3D11Buffer* buff = cbm.buff.get();
		switch (shaderType)
		{
		case 1: // PixelShader
			UpdateConstantBuffer(&buff, sp_Pixel, slot);
			break;
		case 2: // GeometryShader
			UpdateConstantBuffer(&buff, sp_Geometry, slot);
			break;
		case 3: // HULL Shader
			UpdateConstantBuffer(&buff, sp_Hull, slot);
			break;
		case 4: // Compute Shader
			UpdateConstantBuffer(&buff, sp_Compute, slot);
			break;
		case 5: // Domain Shader
			UpdateConstantBuffer(&buff, sp_Domain, slot);
			break;
		default: // Vertex Shader
			UpdateConstantBuffer(&buff, sp_Vertex, slot);
		}
		cbm.buff = buff;
	}
	NeedsMVPCalculation = sp->mvp_cpu;
	return true;

}

/*
* Method: ArenaEngine - getLayoutDesc
* Purpose: Retrieves a Layout Description array from an external shader module
* Parameters: int shaderID - the index of the shader to use
* Return: ShaderLayoutDesc - the array of the shader's layout
*/
ShaderLayoutDesc ArenaEngine::getLayoutDesc(int shaderID)
{
	ShaderLayoutDesc ret;
	
	ret.elementCount = 0;
	ret.elements = nullptr;
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return ret;

	ret.elementCount = shaderList[shaderID].elementCount;
	ret.elements = shaderList[shaderID].elements;
	return ret;
}

/*
* Method: ArenaEngine - getLayoutDesc
* Purpose: Retrieves a Layout Description array from a built-in shader module
* Parameters: DefaultShader id - the built-in shader to use
* Return: ShaderLayoutDesc - the array of the shader's layout
*/
ShaderLayoutDesc ArenaEngine::getLayoutDesc(DefaultShader ds)
{
	ShaderLayoutDesc ret;
	
	ret.elementCount = defaultShaders[static_cast<int>(ds)].elementCount;
	ret.elements = defaultShaders[static_cast<int>(ds)].elements;
	return ShaderLayoutDesc();
}

/*
* Method: ArenaEngine - getBufferSize
* Purpose: Retrieves the buffer of a single round of Vertex shader input
* Parameters: int shaderID - the index of the shader to use
* Return: int -  the buffer size
*/
int ArenaEngine::getBufferSize(int shaderID)
{
	if(shaderID < 0 || shaderID >= shaderList.Size())
		return 0;

	return shaderList[shaderID].bufferSize;
}

/*
* Method: ArenaEngine - getBufferSize
* Purpose: Retrieves the buffer of a single round of Vertex shader input
* Parameters: DefaultShader ds - the default shader to use
* Return: int -  the buffer size
*/
int ArenaEngine::getBufferSize(DefaultShader ds)
{
	return defaultShaders[static_cast<int>(ds)].bufferSize;
}

/*
* Method: ArenaEngine - UpdateConstantBuffer
* Purpose: Updates an external shader's constant buffer
* Parameters: int shaderID - the index of the shader to use
*			int bufferLoc - index of the constant buffer in the shader
*			ID3D11Buffer* buff - the new constant buffer
* Return: bool - true if successful, false otherwise
*/
bool ArenaEngine::UpdateConstantBuffer(int shaderID, int bufferLoc, ID3D11Buffer * buff)
{
	if(shaderID < 0 || shaderID >= shaderList.Size() || !buff)
		return false;
	ShaderProgram sp = shaderList[shaderID];
	if (bufferLoc < 0 || bufferLoc >= sp.constantBuffers.Size())
		return false;

	sp.constantBuffers[bufferLoc].buff = buff;
	return true;
}

/*
* Method: ArenaEngine - UpdateConstantBuffer
* Purpose: Updates a built-in Shader's constant buffer
* Parameters: DefaultShader ds - the default shader to use
*			int bufferLoc - index of the constant buffer in the shader
*			ID3D11Buffer* buff - the new constant buffer
* Return: bool - true if successful, false otherwise
*/
bool ArenaEngine::UpdateConstantBuffer(DefaultShader shader, int bufferLoc, ID3D11Buffer * buff)
{
	ShaderProgram sp = shaderList[static_cast<int>(shader)];
	if (bufferLoc < 0 || bufferLoc >= sp.constantBuffers.Size())
		return false;

	sp.constantBuffers[bufferLoc].buff = buff;
	return true;
}

/*
* Method: ArenaEngine - GetLayoutError
* Purpose: Retrieves a Layout error generated during Shader construcion in an external shader
* Parameters: int shaderID - the external shader to look at
*				HRESULT& res - the result of the construction
* Return: bool - whether the parameters were valid
*/
bool ArenaEngine::GetLayoutError(int shaderID, HRESULT& res)
{
	if (shaderID < 0 && shaderID >= shaderList.Size())
		return false;

	res = shaderList[shaderID].layoutError;
	return true;
}

/*
* Method: ArenaEngine - GetLayoutError
* Purpose: Retrieves a Layout error generated during Shader construcion in an external shader
* Parameters: DefaultShader ds - the AnaGame shader to look at
*				HRESULT& res - the result of the construction
* Return: bool - true
*/
bool ArenaEngine::GetLayoutError(DefaultShader ds, HRESULT & res)
{
	res = defaultShaders[static_cast<int>(ds)].layoutError;
	return true;
}

/*
* Method: ArenaEngine - CompareElementLayout
* Purpose: Returns whather an Input Element Layout is the same as that of the specified External shader
* Parameters: int shaderID - the external shder to look at
*				D3D11_INPUT_ELEMENT_DESC * des - the first element layout object
*				int desSize - the size of the provided array
* Return: bool - whether the input elements are the same
*/
bool ArenaEngine::CompareElementLayout(int shaderID, D3D11_INPUT_ELEMENT_DESC * des, int desSize)
{
	if(shaderID < 0 && shaderID >= shaderList.Size())
		return false;

	ShaderProgram sp = shaderList[shaderID];
	if (sp.elementCount != desSize)
		return false;
	for (int c = 0; c < desSize; c++)
	{
		try
		{
			if (des[c].AlignedByteOffset != sp.elements[c].AlignedByteOffset)
				return false;
			if (des[c].Format != sp.elements[c].Format)
				return false;
			if (des[c].InputSlot != sp.elements[c].InputSlot)
				return false;
			if (des[c].InputSlotClass != sp.elements[c].InputSlotClass)
				return false;
			if (des[c].InstanceDataStepRate != sp.elements[c].InstanceDataStepRate)
				return false;
			if (des[c].SemanticIndex != sp.elements[c].SemanticIndex)
				return false;
			if (strcmp(des[c].SemanticName, des[c].SemanticName))
				return false;

		}
		catch (char* e)
		{
			
			return false;
		}
		catch(...)
		{
			return false;
		}
	}
	return true;
}

/*
* Method: ArenaEngine - CompareElementLayout
* Purpose: Returns whather an Input Element Layout is the same as that of the specified AnaGame shader
* Parameters: DefaultShader shader - the AnaGame shder to look at
*				D3D11_INPUT_ELEMENT_DESC * des - the first element layout object
*				int desSize - the size of the provided array
* Return: bool - whether the input elements are the same
*/
bool ArenaEngine::CompareElementLayout(DefaultShader shader, D3D11_INPUT_ELEMENT_DESC * des, int desSize)
{
	ShaderProgram sp = shaderList[static_cast<int>(shader)];
	if (sp.elementCount != desSize)
		return false;
	for (int c = 0; c < desSize; c++)
	{
		try
		{
			if (des[c].AlignedByteOffset != sp.elements[c].AlignedByteOffset)
				return false;
			if (des[c].Format != sp.elements[c].Format)
				return false;
			if (des[c].InputSlot != sp.elements[c].InputSlot)
				return false;
			if (des[c].InputSlotClass != sp.elements[c].InputSlotClass)
				return false;
			if (des[c].InstanceDataStepRate != sp.elements[c].InstanceDataStepRate)
				return false;
			if (des[c].SemanticIndex != sp.elements[c].SemanticIndex)
				return false;
			if (strcmp(des[c].SemanticName, des[c].SemanticName))
				return false;

		}
		catch (char* e)
		{

			return false;
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}

/*
* Method: ArenaEngine - SetNewConstantBuffer
* Purpose: Sets up a Constant buffer 
* Parameters: TrecComPointer<ID3D11Buffer>& buff - the Constant Buffer List
*				int slot - Slot in the Constant buffer memory in the GPU
*				bool isModelBuffer - is this buffer for models?
*				ShaderPhase sp - the shader phase using the buffer
*				int shaderID - the external shader involved
*				unsigned char purp - the purpose of the constant buffer
* Return: bool -  whether all the parameters are valid
*/
bool ArenaEngine::SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, int shaderID, unsigned char purp)
{
	if(!buff.get() || shaderID < 0 || shaderID >= shaderList.Size() || slot < 0 || slot > 15)
		return false;
	ShaderProgram* spr = &(shaderList[shaderID]);

	// Cannot have the same slot
	for (UINT c = 0; c < spr->constantBuffers.Size(); c++)
	{
		if (spr->constantBuffers[c].label >> 4 == slot)
			return false;
	}

	ConstantBufferMark cbm;
	cbm.buff = nullptr;
	cbm.label = 0;

	switch (sp)
	{
	case sp_Vertex:
		cbm.label = 0;
		break;
	case sp_Pixel:
		cbm.label = 1;
		break;
	case sp_Geometry:
		cbm.label = 2;
		break;
	case sp_Hull:
		cbm.label = 3;
		break;
	case sp_Compute:
		cbm.label = 4;
		break;
	case sp_Domain:
		cbm.label = 5;
		break;
	}

	if (isModelBuffer)
		cbm.label = cbm.label | 0b00001000;

	cbm.label += slot << 4;
	cbm.buff = buff;
	spr->constantBuffers.push_back(cbm);

	
	switch (purp)
	{
	case 1: // Color
		spr->colorLoc = spr->constantBuffers.Size() - 1;
		break;
	case 2:// Camera
		spr->cameraLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If Camera was in the shader memory, it would have to calculate
		break;
	case 3:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = true; // if this was the MVP, then the shader expects the CPU to calculate it
		break;
	case 4:
		spr->viewLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If view was in the shader memory, it would have to calculate
		if (cbm.buff.get())
			contextDevice->UpdateSubresource(cbm.buff.get(), 0, 0, &viewBuffer,0,0);
		break;
	case 5:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // In this case, the model just has to update its own matrix, the shader will calculate
	}


	return true;
}

/*
* Method: ArenaEngine - SetNewConstantBuffer
* Purpose: Sets up a Constant buffer
* Parameters: TrecComPointer<ID3D11Buffer>& buff - the Constant Buffer List
*				int slot - Slot in the Constant buffer memory in the GPU
*				bool isModelBuffer - is this buffer for models?
*				ShaderPhase sp - the shader phase using the buffer
*				DefaultShader ds - the AnaGame shader involved
*				unsigned char purp - the purpose of the constant buffer
* Return: bool -  whether all the parameters are valid
*/
bool ArenaEngine::SetNewConstantBuffer(TrecComPointer<ID3D11Buffer>& buff, int slot, bool isModelBuffer, ShaderPhase sp, DefaultShader ds, unsigned char purp)
{
	ShaderProgram* spr = &(defaultShaders[static_cast<int>(ds)]);

	// Cannot have the same slot
	for (UINT c = 0; c < spr->constantBuffers.Size(); c++)
	{
		if (spr->constantBuffers[c].label >> 4 == slot)
			return false;
	}

	ConstantBufferMark cbm;
	cbm.buff = nullptr;
	cbm.label = 0;
	switch (sp)
	{
	case sp_Vertex:
		cbm.label = 0;
		break;
	case sp_Pixel:
		cbm.label = 1;
		break;
	case sp_Geometry:
		cbm.label = 2;
		break;
	case sp_Hull:
		cbm.label = 3;
		break;
	case sp_Compute:
		cbm.label = 4;
		break;
	case sp_Domain:
		cbm.label = 5;
		break;
	}

	if (isModelBuffer)
		cbm.label = cbm.label & 0b00001000;

	cbm.label += slot << 4;
	cbm.buff = buff;
	spr->constantBuffers.push_back(cbm);


	switch (purp)
	{
	case 1: // Color
		spr->colorLoc = spr->constantBuffers.Size() - 1;
		break;
	case 2:// Camera
		spr->cameraLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If Camera was in the shader memory, it would have to calculate
		break;
	case 3:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = true; // if this was the MVP, then the shader expects the CPU to calculate it
		break;
	case 4:
		spr->viewLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // If view was in the shader memory, it would have to calculate
		if (cbm.buff.get())
			contextDevice->UpdateSubresource(cbm.buff.get(), 0, 0, &viewBuffer, 0, 0);
		break;
	case 5:
		spr->modelLoc = spr->constantBuffers.Size() - 1;
		spr->mvp_cpu = false; // In this case, the model just has to update its own matrix, the shader will calculate
	}

	

	return true;
}

/*
* Method: ArenaEngine - SetCameraBuffer
* Purpose: Sets the location of the Camera buffer in the shader
* Parameters: int shaderID - the index of the shader to use
*			int cameraLocation - the index of the camera to set to
* Return: bool - true if successful, otherwise false
*/
bool ArenaEngine::SetCameraBuffer(int shaderID, int cameraLocation)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return false;

	ShaderProgram sp = shaderList[shaderID];
	if (sp.cameraLoc >= 0)
		return false;

	if (cameraLocation < 0 || cameraLocation >= sp.constantBuffers.Size())
		return false;
	sp.cameraLoc = cameraLocation;
	return true;
}

/*
* Method: ArenaEngine - SetCameraBuffer
* Purpose: Sets the location of the Camera buffer in the shader
* Parameters: DefaultShader ds - the default shader to use
*			int cameraLocation - the index of the camera to set to
* Return: bool - true if successful, otherwise false
*/
bool ArenaEngine::SetCameraBuffer(DefaultShader ds, int cameraLocation)
{
	ShaderProgram sp = defaultShaders[static_cast<int>(ds)];
	if (sp.cameraLoc >= 0)
		return false;

	if (cameraLocation < 0 || cameraLocation >= sp.constantBuffers.Size())
		return false;
	sp.cameraLoc = cameraLocation;
	return true;
}

/*
* Method: ArenaEngine - GetTextureCount
* Purpose: Retrieves the number of textures the shader expects
* Parameters: int shaderID - the external shader to look at
* Return: signed char - the number of texture buffers
*/
signed char ArenaEngine::GetTextureCount(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return -1;
	ShaderProgram sp = shaderList[shaderID];
	return sp.TextureCount;
}

/*
* Method: ArenaEngine - GetTextureCount
* Purpose: Retrieves the number of textures the shader expects
* Parameters: DefaultShader shader - the AnaGame shader to look at
* Return: signed char - the number of texture buffers
*/
signed char ArenaEngine::GetTextureCount(DefaultShader shader)
{
	ShaderProgram sp = shaderList[static_cast<int>(shader)];
	return sp.TextureCount;
}

/*
* Method: ArenaEngine - getColorBufferLocation
* Purpose: Retrieves the Index of the Color Buffer in the external shader
* Parameters: DefaultShader shader - the external shader to look at
* Return: signed char - the index of the color buffer (-1 if not present)
*/
signed char ArenaEngine::getColorBufferLocation(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return -2;

	return shaderList[shaderID].colorLoc;
}

/*
* Method: ArenaEngine - getColorBufferLocation
* Purpose: Retrieves the Index of the Color Buffer in the AnaGame shader
* Parameters: DefaultShader shader - the AnaGame shader to look at
* Return: signed char - the index of the color buffer (-1 if not present)
*/
signed char ArenaEngine::getColorBufferLocation(DefaultShader shader)
{
	return defaultShaders[static_cast<int>(shader)].colorLoc;
}

/*
* Method: ArenaEngine - ModelSetFillMode
* Purpose: Sets the Fill mode of this engine
* Parameters: D3D11_FILL_MODE fill - the fill mode to set the engine to
* Return: void
*/
void ArenaEngine::ModelSetFillMode(D3D11_FILL_MODE fill)
{
	if (!contextDevice.get())
		return;
	rasterizer.FillMode = fill;
	ID3D11RasterizerState* state = nullptr;
	graphicsDevice->CreateRasterizerState(&rasterizer, &state);
	contextDevice->RSSetState(state);
	rasterizerState = state;
	
	if (fill == D3D11_FILL_SOLID)
	{
		isFill = true;
	}
	else
	{
		isFill = false;
	}

}

/*
* Method: ArenaEngine - ModelSetCullMode
* Purpose: Sets the CULL mode of this engine
* Parameters: D3D11_CULL_MODE cull - the CULL mode to set to
* Return: void
*/
void ArenaEngine::ModelSetCullMode(D3D11_CULL_MODE cull)
{
	if (!contextDevice.get())
		return;
	rasterizer.CullMode = cull;
	ID3D11RasterizerState* state = nullptr;
	graphicsDevice->CreateRasterizerState(&rasterizer, &state);
	contextDevice->RSSetState(state);
	rasterizerState = state;
	cullMode = static_cast<unsigned char>(cull);

}

/*
* Method: ArenaEngine - SetDefaultRasterization
* Purpose: Sets the Rasterization mode to the default settings
* Parameters: void
* Return: void
*/
void ArenaEngine::SetDefaultRasterization()
{
	if (contextDevice.get())
	{
		rasterizer.CullMode = D3D11_CULL_BACK;
		rasterizer.FillMode = D3D11_FILL_SOLID;
		ID3D11RasterizerState* state = nullptr;
		graphicsDevice->CreateRasterizerState(&rasterizer, &state);
		rasterizerState = state;
		contextDevice->RSSetState(state);
		isFill = true;
		cullMode = CULL_BACK + 1;
	}
}

/*
* Method: ArenaEngine - GetConstantBuffer
* Purpose: Creates a Constant Buffer
* Parameters: int size - the size being requested
*				TrecComPointer<ID3D11Buffer>& buff - reference to hold the buffer
* Return: int - 0 if successful, error code otherwise
*/
int ArenaEngine::GetConstantBuffer(int size, TrecComPointer<ID3D11Buffer>& buff)
{
	if (!size)
		return 1;
	if (!contextDevice.get())
		return 2;
	D3D11_BUFFER_DESC conBuf;
	
	conBuf.CPUAccessFlags = 0;
	conBuf.MiscFlags = 0;
	conBuf.StructureByteStride = 0;
	conBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	conBuf.ByteWidth = size;
	conBuf.Usage = D3D11_USAGE_DEFAULT;
	buff = nullptr;
	ID3D11Buffer* b = nullptr;
	HRESULT res = graphicsDevice->CreateBuffer(&conBuf, 0, &b);
	buff = b;
	return !SUCCEEDED(res);
}

/*
* Method: ArenaEngine - UpdateConstantBuffer
* Purpose: Updates the constant buffer at the DirectX level
* Parameters: ID3D11Buffer  ** buff - the Buffer to update
*				ShaderPhase sp - the Shader phase involved
*				int slot - the slot that the buffer takes up
* Return: void
*/
void ArenaEngine::UpdateConstantBuffer( ID3D11Buffer  ** buff, ShaderPhase sp, int slot)
{
	if ((!buff || !*buff) || !contextDevice.get())
		return;
	switch (sp)
	{
	case sp_Compute:
		contextDevice->CSSetConstantBuffers(slot,1,buff);
		break;
	case sp_Domain:
		contextDevice->DSSetConstantBuffers(slot, 1, buff);
		break;
	case sp_Geometry:
		contextDevice->GSSetConstantBuffers(slot, 1, buff);
		break;
	case sp_Hull:
		contextDevice->HSSetConstantBuffers(slot, 1, buff);
		break;
	case sp_Pixel:
		contextDevice->PSSetConstantBuffers(slot, 1, buff);
		break;
	case sp_Vertex:
		contextDevice->VSSetConstantBuffers(slot, 1, buff);
		break;
	}
}

/*
* Method: ArenaEngine - ReplaceConstantBuffer
* Purpose: Replaces a Constant Buffer with new information in an external shader
* Parameters: int shaderID - the external shader to update
*				unsigned char slot - the constant buffer to update
*				ID3D11Buffer * buff - the new buffer to update the Constant buffer with
* Return: int - 0 if successful, error otherwise
*/
int ArenaEngine::ReplaceConstantBuffer(int shaderID, unsigned char slot, ID3D11Buffer * buff)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return 1;

	if (slot > shaderList[shaderID].constantBuffers.Size())
		return 2;

	if (!buff)
		return 3;



	shaderList[shaderID].constantBuffers[slot].buff = buff;

	return 0;
}

/*
* Method: ArenaEngine - ReplaceConstantBuffer
* Purpose: Replaces a Constant Buffer with new information in an AnaGame shader
* Parameters: DefaultShader shaderID - the AnaGame shader to update
*				unsigned char slot - the constant buffer to update
*				ID3D11Buffer * buff - the new buffer to update the Constant buffer with
* Return: int - 0 if successful, error otherwise
*/
int ArenaEngine::ReplaceConstantBuffer(DefaultShader shaderID, unsigned char slot, ID3D11Buffer * buff)
{
	if (slot > defaultShaders[static_cast<int>(shaderID)].constantBuffers.Size())
		return 2;

	if (!buff)
		return 3;


	defaultShaders[static_cast<int>(shaderID)].constantBuffers[slot].buff = buff;

	return 0;
}

/*
* Method: ArenaEngine - GetArenaEngine (static)
* Purpose: Retrieves the Arena Engine with the provided Name, Window, and Instance
* Parameters: WCHAR * d - the Name of the Engine
*				HWND w - The window to use
*				HINSTANCE i - the Application instance
* Return: TrecPointer<ArenaEngine> - the Arena Engine with the requiremnts (or null)
*/
TrecPointer<ArenaEngine> ArenaEngine::GetArenaEngine(TString& d, HWND w, HINSTANCE i)
{

	for (UINT c = 0; c < engineList.Size(); c++)
	{
		if (engineList[c] && !StrCmpW(d, engineList[c]->name))
		{
			//engineList[c].count++;
			return TrecPointer<ArenaEngine>(engineList[c]);
		}
	}
	ArenaEngine* ae = nullptr;

	for (UINT c = 0; c < engineList.Size(); c++)
	{
		if (engineList[c] && w == engineList[c]->window)
		{
			ae = engineList[c];
			break;
		}
	}
	ArenaEngine* engine = nullptr;
	if (ae)
		engine = new ArenaEngine(*ae);
	else
		engine = new ArenaEngine();
	if (engine)
	{
		engine->name = d;
		engineList.push_back(engine);
		if (ae)
		{
			engine->SetDefaultRasterization();
			engine->isInit = true;
		}
		else
			engine->initialize(w, i);
		//id.count = 1;
	}

	return TrecPointer<ArenaEngine>(engine);
}

/*
* Method: ArenaEngine - getViewPort
* Purpose: Ahould retrieve the View port
* Parameters: WCHAR * d - NA
*				RECT& r - NA
* Return: D3D11_VIEWPORT * - null
* NOTE: DEPRECIATED - Viewports are managed by the TArena Class in the Anaface library
*/
D3D11_VIEWPORT * ArenaEngine::getViewPort(WCHAR * d, RECT & r)
{
	return nullptr;
}

/*
* Method: ArenaEngine - Release
* Purpose: Removes this engine from the List of Arena Engines
* Parameters: void
* Return: void
*/
void ArenaEngine::Release()
{
	
}

/*
* Method: ArenaEngine - initialize
* Purpose: Sets up the Direct 3D Resources used to Render scenes
* Parameters: HWND w - the Window handle the engine is to use
*				HINSTANCE i - the windows instance handle to use
* Return: int - 0 if successful, error code otherwise
*/
UCHAR ArenaEngine::initialize(HWND w, HINSTANCE i)
{
	if (Initialized)
	{
		if (window == w && instance == i)
			return 0;
		Initialized = false;
		Unload();
	}


	window = w;
	instance = i;

	GetClientRect(window, &Location);

	unsigned int width = Location.right - Location.left,
		height = Location.bottom - Location.top;

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
	

	
	// Initialize the swap cahin
	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Width = Location.right - Location.left;
	swapChainDescription.BufferDesc.Height = Location.bottom - Location.top;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 30;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = w;
	swapChainDescription.Windowed = true;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	unsigned int flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG ;
#endif

	HRESULT results;

	ID3D11Device* d = nullptr;
	ID3D11DeviceContext* cd = nullptr;
	results = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags,
		0, 0, D3D11_SDK_VERSION, &(d), &version, &(cd));
	graphicsDevice = d;
	contextDevice = cd;
	

	if (FAILED(results))
	{
		return NO_DEVICE;
	}

	dxDev = nullptr;
	IDXGIDevice* raw_dxDev = nullptr;
	IDXGIAdapter* dxAdpt = nullptr;
	IDXGIFactory* dxFact = nullptr;
	results = (graphicsDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&raw_dxDev));
	if (FAILED(results))
	{

		graphicsDevice = nullptr;
		contextDevice = nullptr;
		return NO_DEVICE;
	}
	dxDev = raw_dxDev;
	results = dxDev->GetParent(__uuidof(IDXGIAdapter), (void**)&dxAdpt);
	if (FAILED(results))
	{

		graphicsDevice = nullptr;
		contextDevice = nullptr;
		dxDev = nullptr;
		return NO_DEVICE;
	}
	results = dxAdpt->GetParent(__uuidof(IDXGIFactory), (void**)&dxFact);


	if (FAILED(results))
	{


		dxAdpt->Release();
		graphicsDevice = nullptr;
		contextDevice = nullptr;
		dxDev = nullptr;
		return NO_DEVICE;
	}


	IDXGISwapChain* sc = nullptr;
	results = dxFact->CreateSwapChain(graphicsDevice.get(), &swapChainDescription, &sc);
	swapper = sc;

	if (FAILED(results))
	{


		dxAdpt->Release();
		dxFact->Release();
		dxDev = nullptr;
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

	D3D11_RENDER_TARGET_VIEW_DESC rendDesc;
	ZeroMemory(&rendDesc, sizeof(rendDesc));
	//rendDesc.
	rendDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rendDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rendDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView* rt = nullptr;
	results = graphicsDevice->CreateRenderTargetView(texture, &rendDesc, &rt);
	renderTarget = rt;

	if (texture)
		texture->Release();

	if (FAILED(results))
	{
		return NO_RENDER_TARGET;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	stencilDesc.Texture2D.MipSlice = 0;
	ID3D11DepthStencilView* depthStencil = nullptr;
	results = graphicsDevice->CreateDepthStencilView(texture, &stencilDesc, &depthStencil);

	ID3D11RenderTargetView* const renderer = renderTarget.get();
	if(SUCCEEDED(results))
		contextDevice->OMSetRenderTargets(1, &renderer, depthStencil);
	else
		contextDevice->OMSetRenderTargets(1, &renderer, 0);

	D3D11_BUFFER_DESC conBuf;
	ZeroMemory(&conBuf, sizeof(conBuf));
	conBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	conBuf.ByteWidth = sizeof(DirectX::XMMATRIX);
	conBuf.Usage = D3D11_USAGE_DEFAULT;


	ID3D11RasterizerState* rs = nullptr;

	// Create Rasterization Objects
	/*graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	solidCullBack = rs;
	rasterizer.CullMode = D3D11_CULL_NONE;

	graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	solidNoCull = rs;
	rasterizer.CullMode = D3D11_CULL_FRONT;

	graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	solidCullFront = rs;
	rasterizer.FillMode = D3D11_FILL_WIREFRAME;

	graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	wiredCullFront = rs;
	rasterizer.CullMode = D3D11_CULL_NONE;

	graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	wiredNoCull = rs;
	rasterizer.CullMode = D3D11_CULL_BACK;
	graphicsDevice->CreateRasterizerState(&rasterizer, &rs);
	wiredCullBack = rs;

	isFill = true;
	cullMode = CULL_BACK;

	isInit = true;*/
	SetDefaultRasterization();
	InitializeDefaultShaders();
	return NO_ERROR;
}

/*
* Method: ArenaEngine - isInitialized
* Purpose: Reports whether the Engine has been properly initialized
* Parameters: void
* Return: bool - whether initialization was successful
*/
bool ArenaEngine::isInitialized()
{
	return isInit;
}

/*
* Method: ArenaEngine - getDevice
* Purpose: Retrieves the Context Device used for drawing
* Parameters: void
* Return: TrecComPointer<ID3D11DeviceContext> - Pointer to the Device Context
*/
TrecComPointer<ID3D11DeviceContext> ArenaEngine::getDevice()
{
	return contextDevice;
}

/*
* Method: ArenaEngine - getSwapChain
* Purpose: Retrieves the Swap Chain used by the Engine
* Parameters: void
* Return: TrecComPointer<IDXGISwapChain> - Pointer to the Swap Chain
*/
TrecComPointer<IDXGISwapChain> ArenaEngine::getSwapChain()
{
	return swapper;
}

/*
* Method: ArenaEngine - getRederTarget
* Purpose: Retrieves the 3D Render Target for the engine
* Parameters: void
* Return: TrecComPointer<ID3D11RenderTargetView> - Pointer to the 3D Render Target  
*/
TrecComPointer<ID3D11RenderTargetView> ArenaEngine::getRederTarget()
{
	return renderTarget;
}

/*
* Method: ArenaEngine - getDeviceD
* Purpose: Retrieves the Direct 3D device used by the engine
* Parameters: void
* Return: TrecComPointer<ID3D11Device> - Pointer to the engine's Direct3D Device  
*/
TrecComPointer<ID3D11Device> ArenaEngine::getDeviceD()
{
	return graphicsDevice;
}

/*
* Method: ArenaEngine - getDeviceD_U
* Purpose: Retrieves the DirectX (mixer) device the engine uses
* Parameters: void
* Return: TrecComPointer<IDXGIDevice> - Pointer to the engine's DirectX Device
*/
TrecComPointer<IDXGIDevice> ArenaEngine::getDeviceD_U()
{
	return dxDev;
}

/*
* Method: ArenaEngine - DoMVP
* Purpose: Reports whether the Engine is set to use a shader that requires the MVP calculation to be done in software
* Parameters: void
* Return: bool - whether software needs to do the MVP calculation
*/
bool ArenaEngine::DoMVP()
{
	return NeedsMVPCalculation;
}

/*
* Method: ArenaEngine - PrepareScene
* Purpose: Sets up a Round of Drawing 
* Parameters: D2D1::ColorF color - The color to clear the render target to
* Return: void
*/
void ArenaEngine::PrepareScene(D2D1::ColorF color)
{	
	if (!contextDevice.get() || !renderTarget.get())
		return;
	float clearColor[4] = { color.r, color.g ,color.b,color.a };

	contextDevice->ClearRenderTargetView(renderTarget.get(), clearColor);
}

/*
* Method: ArenaEngine - RenderScene
* Purpose: Renders a Camera View to a location on the screen
* Parameters: DirectX::XMMATRIX& proj - Reference to the Projection Matrix for MPV calculation
*				DirectX::XMMATRIX& cam - Reference to the View Matrix for MPV calculation
*				D3D11_VIEWPORT& viewPort - Location of Screen or Window to render to
* Return: void
*/
void ArenaEngine::RenderScene(DirectX::XMMATRIX& proj, DirectX::XMMATRIX& cam, D3D11_VIEWPORT& viewPort)
{
	if (!contextDevice.get())
		return;

	contextDevice->RSSetViewports(1, &viewPort);

	//contextDevice->ClearDepthStencilView(depthStensil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//currentCamera = m;
	currentCameraProj =  proj;
	camera = cam;

	for (UINT c = 0; c < models.Size(); c++)
	{
		if (models[c])
			models[c]->Render(proj,cam, this);
	}

}

/*
* Method: ArenaEngine - FinalizeScene
* Purpose: Completes a round of Rendering by presenting it to the Swap Chain
* Parameters: void
* Return: void
*/
void ArenaEngine::FinalizeScene()
{
	if (swapper.get())
		swapper->Present(0, 0);
}

/*
* Method: ArenaEngine - GetModelLocation
* Purpose: Retrieves the index of the model buffer in the shader module
* Parameters: DefaultShader ds - the external shader to look at
* Return: signed char - the index of the model buffer in the shader
*/
signed char ArenaEngine::GetModelLocation(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return -2;
	return shaderList[shaderID].modelLoc;
}

/*
* Method: ArenaEngine - GetModelLocation
* Purpose: Retrieves the index of the model buffer in the shader module
* Parameters: DefaultShader ds - the AnaGame shader to look at
* Return: signed char - the index of the model buffer in the shader
*/
signed char ArenaEngine::GetModelLocation(DefaultShader ds)
{
	return defaultShaders[static_cast<unsigned int>(ds)].modelLoc;
}

/*
* Method: ArenaEngine - GetViewLocation
* Purpose: Retrieves the index of the View buffer in the shader module
* Parameters: DefaultShader ds - the external shader to look at
* Return: signed char - the index of the view buffer in the shader
*/
signed char ArenaEngine::GetViewLocation(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return -2;
	return shaderList[shaderID].viewLoc;
}

/*
* Method: ArenaEngine - GetViewLocation
* Purpose: Retrieves the index of the View buffer in the shader module
* Parameters: DefaultShader ds - the AnaGame shader to look at
* Return: signed char - the index of the view buffer in the shader
*/
signed char ArenaEngine::GetViewLocation(DefaultShader ds)
{
	return defaultShaders[static_cast<unsigned int>(ds)].viewLoc;
}

/*
* Method: ArenaEngine - GetCameraLocation
* Purpose: Retrieves the index of the View buffer in the shader module
* Parameters: DefaultShader ds - the external shader to look at
* Return: signed char - the index of the view buffer in the shader
*/
signed char ArenaEngine::GetCameraLocation(int shaderID)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return -2;
	return 0;
}

/*
* Method: ArenaEngine - GetCameraLocation
* Purpose: Retrieves the index of the Camera buffer in the shader module
* Parameters: DefaultShader ds - the AnaGame shader to look at 
* Return: signed char - the index of the camera buffer in the shader
*/
signed char ArenaEngine::GetCameraLocation(DefaultShader ds)
{
	return defaultShaders[static_cast<unsigned int>(ds)].cameraLoc;
}


UCHAR ArenaEngineType[] = {2, 0b10000000, 5};

/*
* Method: ArenaEngine - GetAnaGameType
* Purpose: Reports to the Virtual Machine what type this object is
* Parameters: void
* Return: UCHAR* - AnaGame representation of the ArenaEngine type 
*/
UCHAR * ArenaEngine::GetAnaGameType()
{
	return nullptr;
}

UCHAR ArenaEngine::OnWindowResize()
{
	/*Unload();
	Initialized = false;
	return initialize(window, instance);*/
	// Initialize a new swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDescription;

	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Width = Location.right - Location.left;
	swapChainDescription.BufferDesc.Height = Location.bottom - Location.top;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 30;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = window;
	swapChainDescription.Windowed = true;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	unsigned int flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	IDXGIAdapter* dxAdpt = nullptr;
	IDXGIFactory* dxFact = nullptr;

	HRESULT results = dxDev->GetParent(__uuidof(IDXGIAdapter), (void**)&dxAdpt);
	if (FAILED(results))
	{

		//graphicsDevice = nullptr;
		//contextDevice = nullptr;
		//dxDev = nullptr;
		return NO_DEVICE;
	}
	results = dxAdpt->GetParent(__uuidof(IDXGIFactory), (void**)&dxFact);


	if (FAILED(results))
	{


		dxAdpt->Release();
		//graphicsDevice = nullptr;
		//contextDevice = nullptr;
		//dxDev = nullptr;
		return NO_DEVICE;
	}

	IDXGISwapChain* sc = nullptr;
	 results = dxFact->CreateSwapChain(graphicsDevice.get(), &swapChainDescription, &sc);
	swapper = sc;

	if (FAILED(results))
	{
		dxAdpt->Release();
		dxFact->Release();
		//dxDev = nullptr;
		//graphicsDevice = nullptr;
		//contextDevice = nullptr;
		return NO_DEVICE;
	}

	ID3D11Texture2D* texture;

	results = swapper->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture);
	if (FAILED(results))
	{
		return NO_SWAP_CHAIN;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rendDesc;
	ZeroMemory(&rendDesc, sizeof(rendDesc));
	//rendDesc.
	rendDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	rendDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rendDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView* rt = nullptr;
	results = graphicsDevice->CreateRenderTargetView(texture, &rendDesc, &rt);
	renderTarget = rt;

	if (texture)
		texture->Release();

	if (FAILED(results))
	{
		return NO_RENDER_TARGET;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	stencilDesc.Texture2D.MipSlice = 0;
	ID3D11DepthStencilView* depthStencil = nullptr;
	results = graphicsDevice->CreateDepthStencilView(texture, &stencilDesc, &depthStencil);

	ID3D11RenderTargetView* const renderer = renderTarget.get();
	if (SUCCEEDED(results))
		contextDevice->OMSetRenderTargets(1, &renderer, depthStencil);
	else
		contextDevice->OMSetRenderTargets(1, &renderer, 0);

	D3D11_BUFFER_DESC conBuf;
	ZeroMemory(&conBuf, sizeof(conBuf));
	conBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	conBuf.ByteWidth = sizeof(DirectX::XMMATRIX);
	conBuf.Usage = D3D11_USAGE_DEFAULT;


	ID3D11RasterizerState* rs = nullptr;

	SetDefaultRasterization();

	return NO_ERROR;
}

TDataArray<ArenaModel*>* ArenaEngine::getModelList()
{
	return &models;
}

/*
* Method: ArenaEngine - Unload
* Purpose: nullifies the resources of the engine
* Parameters: void
* Return: void
*/
void ArenaEngine::Unload()
{

	graphicsDevice = nullptr;
	contextDevice = nullptr;
	swapper = nullptr;
	renderTarget = nullptr;

	dxDev = nullptr;
	ShaderProgram sp;

}

/*
* Method: ArenaEngine - SetNewBasicShader
* Purpose: Sets up either a pixel shader or vertex shader, preparing an input description for vertex buffer
* Parameters: TString& s - the path of the file to read
*				CHAR *f - function name of the shader
*				TDataArray<unsigned short>& bufferDesc - list of buffer information
*				bool isVertex - true if compiling Vertex shader, flase for pixel shader
*				ShaderProgram& sp - the shader module holding these shaders
* Return: int - 0 if successful, error otherwise
*/
int ArenaEngine::SetNewBasicShader(TString & s, CHAR * f, TDataArray<unsigned short>& bufferDesc, bool isVertex, ShaderProgram& sp)
{
	if (!f)
		return -1;

	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res;
		
		if(isVertex)
			res = D3DCompileFromFile(s.GetBuffer(), 0, 0, f, "vs_4_0", flags, 0, &buffer, &errorBuffer);
		else
			res = D3DCompileFromFile(s.GetBuffer(), 0, 0, f, "ps_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		if (errorBuffer)
		{
			OutputDebugStringA(reinterpret_cast<char*>(errorBuffer->GetBufferPointer()));
			errorBuffer->Release();
			errorBuffer = nullptr;
		}
		if (buffer)
		{
			buffer->Release();
			buffer = nullptr;
		}

		return -2;
	}

	if (isVertex)
	{
		ID3D11VertexShader* vs = nullptr;
		ID3D11InputLayout* il = nullptr;

		res = graphicsDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &vs);
		sp.vs = vs;

		// Now handle the input Layout
		TDataArray<D3D11_INPUT_ELEMENT_DESC>* desc = getInputDescription(bufferDesc, sp.bufferSize);
		D3D11_INPUT_ELEMENT_DESC* dec = desc->data();
		sp.layoutError = graphicsDevice->CreateInputLayout(dec, desc->Size(), buffer->GetBufferPointer(), buffer->GetBufferSize(), &il);
		sp.layout = il;
		sp.elements = new D3D11_INPUT_ELEMENT_DESC[(sizeof(D3D11_INPUT_ELEMENT_DESC) * desc->Size())];
		sp.elementCount = desc->Size();
		for (UINT c = 0; c < desc->Size(); c++)
		{
			sp.elements[c] = desc->at(c);
		}
		delete desc;
		desc = nullptr;
	}
	else
	{
		ID3D11PixelShader* ps = nullptr;
		res = graphicsDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &ps);
		sp.ps = ps;
	}

	if(buffer)
	buffer->Release();
	buffer = nullptr;
	if(errorBuffer)
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return -3;
}



/*
* Method: ArenaEngine - getInputDescription
* Purpose: Converts An AnaGame Input Layout list to a Direct3D Input Layout List
* Parameters:TDataArray<unsigned short>& bufferDesc - the Input Laout in AnaGame format
*			int& buffSize - the total size of the input buffer
* Return: TDataArray<D3D11_INPUT_ELEMENT_DESC>* - the Direct3D Compatible version of the input layout
*/
TDataArray<D3D11_INPUT_ELEMENT_DESC>* ArenaEngine::getInputDescription(TDataArray<unsigned short>& bufferDesc, int& buffSize)
{
	TDataArray<D3D11_INPUT_ELEMENT_DESC> *returnable = new TDataArray<D3D11_INPUT_ELEMENT_DESC>();

	unsigned char colorCount = 0, positionCount = 0, texcoordCount = 0, normalCount = 0,
		binormalCount = 0, blendweightCount = 0, blendindexCount = 0, tPosCount = 0,
		tangentCount = 0, fogCount = 0, tessCount = 0;
	unsigned char offsetCount = 0;

	for (UINT c = 0; c < bufferDesc.Size(); c++)
	{
		D3D11_INPUT_ELEMENT_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		
		// Width of the data
		unsigned char dataWidth = bufferDesc[c] & 0b0000000011110000;
		dataWidth = dataWidth >> 4;

		// Count of the data
		unsigned short dataCount = bufferDesc[c] & 0b1111000000000000;
		dataCount = dataCount >> 12;
		
		// First, mark offset count so 0 value is included in first element
		desc.AlignedByteOffset = offsetCount;

		// If these values are 0, assume default of 4
		if (!dataCount)
			dataCount = 4;
		if (!dataWidth)
			dataWidth = 4;

		assignDescriptionFormat(desc, dataWidth, dataCount);
		offsetCount += dataWidth * dataCount;

		switch (bufferDesc[c] & 0b00001111)
		{
		case 0: // Color
			desc.SemanticName = sem_C;
			desc.SemanticIndex = colorCount++;

			break;
		case 1: // normal
			desc.SemanticName = sem_N;
			desc.SemanticIndex = normalCount++;
			break;
		case 2: // TextCoord
			desc.SemanticName = sem_T;
			desc.SemanticIndex = texcoordCount++;
			break;
		case 3: // position
			desc.SemanticName = sem_V;
			desc.SemanticIndex = positionCount++;
			break;
		case 4: // binormal
			desc.SemanticName = sem_BN;
			desc.SemanticIndex = binormalCount++;
			break;
		case 5: // blendweight
			desc.SemanticName = sem_BW;
			desc.SemanticIndex = blendweightCount++;
			break;
		case 6: // blendindex
			desc.SemanticName = sem_BI;
			desc.SemanticIndex = blendindexCount++;
			break;
		case 7: // position T
			desc.SemanticName = sem_PT;
			desc.SemanticIndex = tPosCount++;
			break;
		case 8: // Tangent
			desc.SemanticName = sem_T;
			desc.SemanticIndex = tangentCount++;
			break;
		case 9: // Fog
			desc.SemanticName = sem_F;
			desc.SemanticIndex = fogCount++;
			break;
		case 10: // Tessfactor
			desc.SemanticName = sem_TS;
			desc.SemanticIndex = tessCount++;
			break;
		}

		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InputSlot = (bufferDesc[c] & 0b0000111100000000) >> 8;

		returnable->push_back(desc);
	}
	buffSize = offsetCount;
	return returnable;
}

/*
* Method: ArenaEngine - assignDescriptionFormat
* Purpose: Aids in the Development of a D3D Input Description b deducing what type of input is being added
* Parameters: D3D11_INPUT_ELEMENT_DESC & desc - The description to update
*				unsigned char valueSize - the size of the data types
*				unsigned short valueCount - the count of data per entry
* Return: void
*/
void ArenaEngine::assignDescriptionFormat(D3D11_INPUT_ELEMENT_DESC & desc, unsigned char valueSize, unsigned short valueCount)
{
	switch (valueSize)
	{
	case 1: // 8 bits
		switch(valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R8_TYPELESS;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R8G8_TYPELESS;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_UNKNOWN;      // Not Supported by Direct 3d
			break;
		default:
			desc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
			break;
		}
		return;
	case 2: // 16 bits
		switch (valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R16_FLOAT;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R16G16_FLOAT;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_UNKNOWN;
			break;
		default:
			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		}
		return;
	default: // 32 bits
		switch (valueCount)
		{
		case 1:
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case 2:
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case 3:
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		default:
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		return;
	}
}

/*
* Method: ArenaEngine - AddModel
* Purpose: Adds a Model to the collection of models to draw
* Parameters: ArenaModel& ae - the Model to add
* Return: void
*/
void ArenaEngine::AddModel(ArenaModel& ae)
{
	models.push_back(&ae);
}

/*
* Method: ArenaEngine - RemoveModel
* Purpose: Extracts an Arena Model from the Engine
* Parameters: ArenaModel* ae - pointer tot he Model to extract
* Return: void
*/
void ArenaEngine::RemoveModel(ArenaModel * ae)
{
	for (UINT c = 0; c < models.Size(); c++)
	{
		if (models[c] == ae)
		{
			models.RemoveAt(c);
			return;
		}
	}
}

void ArenaEngine::SetResources(TrecComPointer<ID3D11DeviceContext>& c, TrecComPointer<IDXGISwapChain>& swapChain, TrecComPointer<ID3D11RenderTargetView>& renderTarget, TrecComPointer<ID3D11Device>& device)
{
	c = this->contextDevice;
	swapChain = this->swapper;
	renderTarget = this->renderTarget;
	device = this->graphicsDevice;
}

/*
* Method: ArenaEngine - SetNewBasicShader
* Purpose: Sets up a Default Shader
* Parameters: TString& vs - the path of the file with vertex shader to read
*				TString& ps - the path of the file with pixel shader to read
*				CHAR *vf - function name of the vertex shader
*				CHAR *sf - function name of the pixel shader
*				TDataArray<unsigned short>& bufferDesc - list of buffer information
*				DefaultShader ds - the shader identity
* Return: bool - true if successful, false otherwise
*/
bool ArenaEngine::SetNewBasicShader(TString & vs, TString & ps, CHAR * vf, CHAR * sf, TDataArray<unsigned short>& bufferDesc, DefaultShader ds)
{
	ShaderProgram newShader;
	ResetShaderProgram(newShader);
	ShaderProgram* targetShader = &defaultShaders[static_cast<int>(ds)];

	ResetShaderProgram(newShader);

	targetShader->cameraLoc = -1;
	targetShader->colorLoc = -1;
	targetShader->modelLoc = -1;
	targetShader->viewLoc = -1;

	if (!vf || !sf || !graphicsDevice.get())
		return false;

	int test = SetNewBasicShader(vs, vf, bufferDesc, true, newShader);

	if (test)
		return false;
	test = SetNewBasicShader(ps, sf, bufferDesc, false, newShader);

	if (test)
	{
		//newShader.vs->Release();
		newShader.vs = nullptr;
		return false;
	}



	(defaultShaders[static_cast<int>(ds)]) = newShader;

	return true;
}

/*
* Method: ArenaEngine - GetMostRecentBasicShader
* Purpose: Turns the parameter reference to the most recent AnaGame shader used
* Parameters: DefaultShader& ds - reference to the shader to hold
* Return: bool - whether the most recent shader worked
*/
bool ArenaEngine::GetMostRecentBasicShader(DefaultShader & ds)
{
	ds = mostRecentDefault;
	return mostRecentWorked;
}

/*
* Method: ArenaEngine - AddComputeShader
* Purpose: Adds a compute shader to the indexed Shader Module
* Parameters: DefaultShader s - the default shader to target
*				TString& cs - file holding the compute shader
*				CHAR* cf - function name of the compute shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddComputeShader(DefaultShader ds, TString & cs, CHAR * cf)
{
	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(cs.GetBuffer(), 0, 0, cf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11ComputeShader* c = nullptr;
	res = graphicsDevice->CreateComputeShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &c);
	defaultShaders[static_cast<int>(ds)].cs = c;

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - AddDomainShader
* Purpose: Adds a domain shader to the indexed Shader Module
* Parameters: DefaultShader s - the default shader to target
*				TString& ds - file holding the domain shader
*				CHAR* df - function name of the domain shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddDomainShader(DefaultShader s, TString & ds, CHAR * df)
{
	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(ds.GetBuffer(), 0, 0, df, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11DomainShader* d = nullptr;
	res = graphicsDevice->CreateDomainShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &d);
	shaderList[static_cast<int>(s)].ds = d;

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - AddGeometryShader
* Purpose: Adds a geometry shader to the indexed Shader Module
* Parameters: DefaultShader s - the default shader to target
*				TString& gs - file holding the geometry shader
*				CHAR* gf - function name of the geometry shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddGeometryShader(DefaultShader ds, TString & gs, CHAR * gf)
{
	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(gs.GetBuffer(), 0, 0, gf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11GeometryShader* g = nullptr;
	res = graphicsDevice->CreateGeometryShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &g);
	defaultShaders[static_cast<int>(ds)].gs = g;

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - AddHullShader
* Purpose: Adds a hull shader to the indexed Shader Module
* Parameters: DefaultShader s - the default shader to target
*				TString& hs - file holding the hull shader
*				CHAR* hf - function name of the hull shader
* Return: int - 0 if successful, otherwise error code
*/
int ArenaEngine::AddHullShader(DefaultShader ds, TString & hs, CHAR * hf)
{
	ID3DBlob* buffer = nullptr, *errorBuffer = nullptr;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT res = D3DCompileFromFile(hs.GetBuffer(), 0, 0, hf, "vs_4_0", flags, 0, &buffer, &errorBuffer);

	if (!SUCCEEDED(res))
	{
		return 3;
	}
	ID3D11HullShader* h = nullptr;
	res = graphicsDevice->CreateHullShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, &h);
	defaultShaders[static_cast<int>(ds)].hs = h;

	buffer->Release();
	buffer = nullptr;
	errorBuffer->Release();
	errorBuffer = nullptr;

	if (SUCCEEDED(res))
		return 0;
	else
		return 4;
}

/*
* Method: ArenaEngine - SetTextureCount
* Purpose: Sets the Count of an external shader module
* Parameters: int shaderID - the index o the Shader module to target
*				unsigned char c - the texture count to set to
* Return: void
*/
void ArenaEngine::SetTextureCount(int shaderID, unsigned char c)
{
	if (shaderID < 0 || shaderID >= shaderList.Size())
		return;
	ShaderProgram sp = shaderList[shaderID];
	sp.TextureCount = c;
}

/*
* Method: ArenaEngine - SetTextureCount
* Purpose: Sets the Count of an external shader module
* Parameters: DefaultShader shaderID - the Built-in Shader module to target
*				unsigned char c - the texture count to set to
* Return: void
*/
void ArenaEngine::SetTextureCount(DefaultShader shaderID, unsigned char c)
{
	ShaderProgram sp = defaultShaders[static_cast<int>(shaderID)];
	sp.TextureCount = c;
}

/*
* Method: ArenaEngine - ResetShaderProgram
* Purpose: sets a Shader Module to it's Nullified state
* Parameters: ShaderProgram & sp - the shader module to reset
* Return: void
*/
void ArenaEngine::ResetShaderProgram(ShaderProgram & sp)
{
	sp.bufferSize = 0;
	sp.cs = nullptr;
	sp.ds = nullptr;
	sp.elementCount = 0;
	sp.elements = 0;
	sp.gs = nullptr;
	sp.hs = nullptr;
	sp.layout = nullptr;
	sp.layoutError = 0;
	sp.mvp_cpu = false;
	sp.ps = nullptr;
	sp.TextureCount = 0;
	sp.vs = nullptr;
	sp.cameraLoc = -1;
	sp.colorLoc = -1;
	sp.modelLoc = -1;
	sp.viewLoc = -1;
}

