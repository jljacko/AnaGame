#pragma once
#include <Parser_.h>

#include "ArenaEngine.h"
#include "ArenaModel.h"

/*
* struct BasicShaderDetails
* Purpose: Provides the bare miminum resources needed to produce a shader
*/
typedef struct BasicShaderDetails
{
	TrecPointer<TString> vertexFile;
	bool vertexFileSet;
	TrecPointer<TString> pixelFile;
	bool pixelFileSet;
	TrecPointer<TString> vertexFunction;
	bool vertexFunctionSet;
	TrecPointer<TString> pixelFunction;
	bool pixelFunctionSet;
}BasicShaderDetails;

/*
* struct ConstantBufferDescription
* Purpose: Provides information regarding a shaders constant buffers
*/
typedef struct ConstantBufferDescription
{
	ShaderPhase sp;
	int bufferSlot;
	bool ModelBuffer;
	int size;
	unsigned char purpose;
}ConstantBufferDescription;

/*
* class ShaderParser
* Creates a shader
*/
class ShaderParser : public Parser_
{
	friend class ArenaEngine;
public:
	ShaderParser(ArenaEngine& ae, TString directory);
	~ShaderParser();

	// for the initial object type
	virtual bool Obj(TString* v) override;
	// for the attribute name
	virtual bool Attribute(TrecPointer<TString> v, TString& e) override;
	// for the attribute value (here it may be good that TStrings are used)

	virtual bool submitType(TString v);
	virtual bool submitEdition(TString v);

	virtual bool goChild();
	virtual void goParent();

	virtual UCHAR* GetAnaGameType()override;

protected:
	bool AddShaderToProgram(TString& str);
	bool SetBasicShader();

	bool SetBufferPurpose(TString& t);
	bool SetInputSlot(TString& v);
	bool SetDataWidth(TString& v);
	bool SetDataCount(TString& v);

	bool SetTextureCount(TString& v);

	bool SetDefaultShader(DefaultShader ds);
	DefaultShader defaultShader;

	ShaderKey shaderID;
	bool shaderIDd;
	bool isDefaultShader;
	ArenaEngine* engine;
	BasicShaderDetails bsd;
	ShaderPhase phase;

	ConstantBufferDescription cbd;

	TrecPointer<TString> shaderFile;
	unsigned short desc;
	TDataArray<unsigned short> bufferDesc;

	TString fileLocation;
};

