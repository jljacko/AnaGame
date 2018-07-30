#pragma once
#include "ModelReader.h"
class ObjReader :public ModelReader
{
public:
	ObjReader(TrecPointer<TFile> file, TrecPointer<ArenaEngine> engine);
	~ObjReader();

	virtual UINT ReadFile()override;
	virtual UCHAR* GetAnaGameType()override;
private:
	TDataArray<float> vertices, normals, textures;
	TDataArray<UINT> triangularIndex, squareIndex;
};

