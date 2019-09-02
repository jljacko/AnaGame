
#include "ShaderParser.h"
/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/


/*
* Method: (ShaderParser) (Constructor)
* Purpose: Sets up the shader parser
* Parameters: ArenaEngine& ae - the Arena Engine to work with
*			TString directory - The directory of resources to look at
* Return: void
*/
ShaderParser::ShaderParser(ArenaEngine& ae, TString directory)
{
	isDefaultShader = false;
	engine = &ae;
	bsd.vertexFileSet = bsd.pixelFileSet = bsd.vertexFunctionSet = bsd.pixelFunctionSet = false;

	
	cbd.bufferSlot = 0;
	cbd.ModelBuffer = false;
	cbd.purpose = 0;
	cbd.size = 0;
	cbd.sp = static_cast<ShaderPhase>(0);
	fileLocation.Set(directory);
}

/*
* Method: ShaderParser) (Destructor)
* Purpose: Serves as the destructor for the object
* Parameters: void
* Return: void
*/
ShaderParser::~ShaderParser()
{
}

/*
* Method: ShaderParser - Obj
* Purpose: Rounds up the currently set attributes into either a vertex component or constant buffer and clears the attributes
* Parameters: TString * v - the Object type to set
* Return: bool - success result
*/
bool ShaderParser::Obj(TString * v)
{
	if(! v)
	return false;
	if (!v->Compare(L"Buff"))
	{
		bufferDesc.push_back(desc);	
		desc = 0;
		return true;
	}
	else if (!v->Compare(L"Const"))
	{
		TrecComPointer<ID3D11Buffer> buff;
		if (engine->GetConstantBuffer(cbd.size, buff))
			return false;
		bool newConst = false;
		if (shaderID._default)
			newConst = engine->SetNewConstantBuffer(buff, cbd.bufferSlot, cbd.ModelBuffer, cbd.sp, shaderID.card.dID, cbd.purpose);
		else
			newConst = engine->SetNewConstantBuffer(buff, cbd.bufferSlot, cbd.ModelBuffer, cbd.sp, shaderID.card.id, cbd.purpose);
		return newConst;
	}

	return false;
}

/*
* Method: ShaderParser - Attribute
* Purpose: Documents potential attributes of either vertex input or constant buffers to be declared
* Parameters: TrecPointer<TString> v - value of the shader attribute
*				TString& e - the attribute title
* Return: bool - whether the value submission was successful
*/
bool ShaderParser::Attribute(TrecPointer<TString> v, TString& e)
{
	// Quick Null check so we don't have to worry about it later down the line
	if (!v.Get())
		return false;

	if (!TString::Compare(e, L"|InputSlot"))
	{
		if (!v.Get())
			return false;
		return SetInputSlot(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferPurpose"))
	{
		if (!v.Get())
			return false;
		return SetBufferPurpose(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferWidth"))
	{
		if (!v.Get())
			return false;
		return SetDataWidth(*v.Get());
	}
	else if (!TString::Compare(e, L"|BufferSize"))
	{
		if (!v.Get())
		return false;
		return SetDataCount(*v.Get());
	}
	else if (!TString::Compare(e, L"|InputBufferCount"))
	{

	}
	else if (!TString::Compare(e, L"|PixelFile"))
	{
		TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
		sh_str->Append(L"\\");
		*sh_str.Get() + *v.Get();
		bsd.pixelFile = sh_str;
		bsd.pixelFileSet = true;
	}
	else if (!TString::Compare(e, L"|VertexFile"))
	{
		TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
		sh_str->Append(L"\\");
		*sh_str.Get() + *v.Get();
		bsd.vertexFile = sh_str;
		bsd.vertexFileSet = true;
	}
	else if (!TString::Compare(e, L"|PixelFunction"))
	{
		bsd.pixelFunction = v;
		bsd.pixelFunctionSet = true;
	}
	else if (!TString::Compare(e, L"|VertexFunction"))
	{
		bsd.vertexFunction = v;
		bsd.vertexFunctionSet = true;
	}

	else if (shaderIDd)
	{
		if (!TString::Compare(e, L"|DomainFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
			sh_str->Append(L"\\");
			*sh_str.Get() + *v.Get();
			shaderFile = sh_str;
			phase = sp_Domain;
		}
		else if (!TString::Compare(e, L"|DomainFunction"))
		{
			if (phase == sp_Domain)
				return AddShaderToProgram(*v.Get());
		}
		else if (!TString::Compare(e, L"|ComputeFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
			sh_str->Append(L"\\");
			*sh_str.Get() + *v.Get();
			shaderFile = sh_str;
			phase = sp_Compute;
		}
		else if (!TString::Compare(e, L"|ComputeFunction"))
		{
			if (phase == sp_Compute)
				return AddShaderToProgram(*v.Get());
		}
		if (!TString::Compare(e, L"|HullFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
			sh_str->Append(L"\\");
			*sh_str.Get() + *v.Get();
			shaderFile = sh_str;
			phase = sp_Hull;
		}
		else if (!TString::Compare(e, L"|HullFunction"))
		{
			if (phase == sp_Hull)
				return AddShaderToProgram(*v.Get());
		}
		if (!TString::Compare(e, L"|GeometryFile"))
		{
			TrecPointer<TString> sh_str = TrecPointerKey::GetNewTrecPointer<TString>(fileLocation);
			sh_str->Append(L"\\");
			*sh_str.Get() + *v.Get();
			shaderFile = sh_str;
			phase = sp_Geometry;
		}
		else if (!TString::Compare(e, L"|GeometryFunction"))
		{
			if (phase == sp_Geometry)
				return AddShaderToProgram(*v.Get());
		}
		else if (!TString::Compare(e, L"|TextureCount"))
		{
			return SetTextureCount(*v.Get());
		
		}
		else if (!TString::Compare(e, L"|ModelBuffer"))
		{
			if (!v->Compare(L"True"))
				cbd.ModelBuffer = true;
		}
		else if (!TString::Compare(e,L"|ConstantSize"))
		{
			int value = 0;
			if (!v->ConvertToInt(&value))
			{
				cbd.size = value;
			}
			else
				return false;
		}
		else if (!TString::Compare(e, L"|ConstantShaderPhase"))
		{
			if (!v->Compare(L"Pixel") || !v->Compare(L"Fragment"))
			{
				cbd.sp = sp_Pixel;
			}
			else if (!v->Compare(L"Hull"))
			{
				cbd.sp = sp_Hull;
			}
			else if (!v->Compare(L"Domain"))
			{
				cbd.sp = sp_Domain;
			}
			else if (!v->Compare(L"Compute"))
			{
				cbd.sp = sp_Compute;
			}
			else if (!v->Compare(L"Geometry"))
			{
				cbd.sp = sp_Geometry;
			}
			else
			{
				cbd.sp = sp_Vertex;
			}

		}
		else if (!TString::Compare(e, L"|ConstantSlot"))
		{
			int value = 0;
			if (!v->ConvertToInt(&value))
			{
				if (value >= 0 && value < 16)
				{
					cbd.bufferSlot = value;
					return true;
				}
			}
			return false;
		}
		else if (!TString::Compare(e, L"|ConstantPurpose"))
		{
			if (!v->Compare(L"Color"))
			{
				cbd.purpose = 1;
			}
			else if (!v->Compare(L"Camera"))
			{
				cbd.purpose = 2;
			}
			else if (!v->Compare(L"MVP"))
				cbd.purpose = 3;
			else if (!v->Compare(L"View"))
				cbd.purpose = 4;
			else if (!v->Compare(L"Model"))
				cbd.purpose = 5;
		}
	}
	
		return SetBasicShader();


	
}

/*
* Method: ShaderParser - submitType
* Purpose: Type of parser expected
* Parameters: TString v - type of parser the file was given
* Return: bool
*/
bool ShaderParser::submitType(TString v)
{
	return false;
}

/*
* Method: ShaderParser
* Purpose: Reports whether this parser may be compatible with the file provided version wise
* Parameters: TString v - the edition string
* Return: bool - false (see note)
* Note: Not applicable to this particular TML format
*/
bool ShaderParser::submitEdition(TString v)
{
	return false;
}

/*
* Method: ShaderParser - goChild
* Purpose: Whether to go child
* Parameters: void
* Return: bool - false (see note)
* Note: Not applicable to this particular TML format
*/
bool ShaderParser::goChild()
{
	return false;
}

/*
* Method: ShaderParser - goParent
* Purpose: Whether to go parent
* Parameters: void
* Return: void
* Note: Not applicable to this particular TML format
*/
void ShaderParser::goParent()
{
}


UCHAR ShaderParserType[] = { 3, 0b10000000, 4, 2 };

/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/
UCHAR * ShaderParser::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/
bool ShaderParser::AddShaderToProgram(TString & str)
{
	std::string cStr = str.GetRegString();

	int err = 0;
	if (shaderID._default)
	{
		switch (phase)
		{
		case sp_Domain:
			err = engine->AddDomainShader(shaderID.card.dID, *shaderFile.Get(), cStr.c_str());
			break;
		case sp_Compute:
			err = engine->AddComputeShader(shaderID.card.dID, *shaderFile.Get(), cStr.c_str());
			break;
		case sp_Hull:
			err = engine->AddHullShader(shaderID.card.dID, *shaderFile.Get(), cStr.c_str());
			break;
		case sp_Geometry:
			err = engine->AddGeometryShader(shaderID.card.dID, *shaderFile.Get(), cStr.c_str());
			break;
		}
	}
	else
	{
		switch (phase)
		{
		case sp_Domain:
			err = engine->AddDomainShader(shaderID.card.id, *shaderFile.Get(), cStr.c_str());
				break;
		case sp_Compute:
			err = engine->AddComputeShader(shaderID.card.id, *shaderFile.Get(), cStr.c_str());
			break;
		case sp_Hull:
			err = engine->AddHullShader(shaderID.card.id, *shaderFile.Get(), cStr.c_str());
			break;
		case sp_Geometry:
			err = engine->AddGeometryShader(shaderID.card.id, *shaderFile.Get(), cStr.c_str());
			break;
		}
	}
	return !err;
}

/*
* Method: ShaderParser
* Purpose:
* Parameters:
* Return:
*/
bool ShaderParser::SetBasicShader()
{
	if (bsd.vertexFileSet && bsd.pixelFileSet && bsd.vertexFunctionSet && bsd.pixelFunctionSet && !shaderIDd)
	{
		std::string cStr2 = bsd.vertexFunction->GetRegString();
		std::string cStr = bsd.pixelFunction->GetRegString();
		int err = 0;
		if (isDefaultShader)
		{
			if(!engine->SetNewBasicShader(*bsd.vertexFile.Get(), *bsd.pixelFile.Get(), cStr2.c_str(), cStr.c_str(), bufferDesc, defaultShader))
				return false;
		}
		else
			err = engine->SetNewBasicShader(*bsd.vertexFile.Get(), *bsd.pixelFile.Get(), cStr2.c_str(), cStr.c_str(), bufferDesc);

		if (err <= 0) // Succeeded
		{
			shaderIDd = true;
			if (!isDefaultShader)
			{
				shaderID._default = false;
				shaderID.card.id = err;
			}
		}
	}
	return true;
}

/*
* Method: ShaderParser
* Purpose: Sets the semantic purpose of a vertex buffer entry
* Parameters: TString & v - semantic buffer purpose in string form
* Return: bool - whether the string was a valid purpose
*/
bool ShaderParser::SetBufferPurpose(TString & t)
{
	if (!t.Compare(L"Position"))
	{
		desc = desc | 0b00000011;
	}
	else if (!t.Compare(L"Texture"))
	{
		desc = desc | 0b00000010;
	}
	else if (!t.Compare(L"Normal"))
	{
		desc = desc | 0b00000001;
	}
	else if(!t.Compare(L"Color"))
	{
		desc = desc | 0b00000000;
	}
	else if (!t.Compare(L"Binormal"))
	{
		desc = desc | static_cast<unsigned char>(4);
	}
	else if (!t.Compare(L"Blendindex"))
	{
		desc = desc | static_cast<unsigned char>(6);
	}
	else if (!t.Compare(L"Blendweight"))
	{
		desc = desc | static_cast<unsigned char>(5);
	}
	else if (!t.Compare(L"T Position"))
	{
		desc = desc | static_cast<unsigned char>(7);
	}
	else if (!t.Compare(L"Tangent"))
	{
		desc = desc | static_cast<unsigned char>(8);
	}
	else if (!t.Compare(L"Fog"))
	{
		desc = desc | static_cast<unsigned char>(9);
	}
	else if (!t.Compare(L"Tessfactor"))
	{
		desc = desc | static_cast<unsigned char>(10);
	}
	else return false;


	return true;
}

/*
* Method: ShaderParser - SetInputSlot
* Purpose: Determines the input slot of a constant buffer
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool ShaderParser::SetInputSlot(TString & v)
{
	int value = 0;
	if (!v.ConvertToInt(&value))
	{
		if (value < 0 || value > 15)
			return false;

		unsigned short cValue = static_cast<unsigned short>(value);
		desc = desc + (cValue << 8);
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetDataWidth
* Purpose: Sets the size of a field that a vertex buffer expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool ShaderParser::SetDataWidth(TString & v)
{
	int value = 0;
	if (!v.ConvertToInt(&value))
	{
		if (value < 0 || value > 4)
			return false;

		unsigned char cValue = 0;
		if(value != 4)  // If it is 4, leave it 0 and let it be corrected later
			cValue = static_cast<unsigned char>(value);// Avoids conflict with Input Slot bits
		desc = desc + (cValue << 4);
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetDataCount
* Purpose: Sets the number of a field that a vertex buffer expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool ShaderParser::SetDataCount(TString & v)
{
	int value = 0;
	if (!v.ConvertToInt(&value))
	{
		if (value < 0 || value > 4)
			return false;

		unsigned short cValue = static_cast<unsigned short>(value);
		cValue = cValue << 12;
		desc = desc + cValue;
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetTextureCount
* Purpose: Sets a texture count for the number of textures the shader expects
* Parameters: TString & v - number in string form
* Return: bool - success, whether the string was a number
*/
bool ShaderParser::SetTextureCount(TString & v)
{
	int value = 0;
	if (!v.ConvertToInt(&value))
	{
		if (shaderID._default)
			engine->SetTextureCount(shaderID.card.dID, static_cast<unsigned char>(value));
		else
			engine->SetTextureCount(shaderID.card.id, static_cast<unsigned char>(value));
		return true;
	}
	return false;
}

/*
* Method: ShaderParser - SetDefaultShader
* Purpose: Lets the parser know it is dealing with an AnaGame shader
* Parameters: DefaultShader ds - the AnaGame shader being created
* Return: bool - false, ignore this
*/
bool ShaderParser::SetDefaultShader(DefaultShader ds)
{
	isDefaultShader = true;
	defaultShader = ds;
	shaderID._default = true;
	shaderID.card.dID = ds;
	return false;
}
