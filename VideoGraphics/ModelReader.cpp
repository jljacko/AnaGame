#include "stdafx.h"
#include "ModelReader.h"


ModelReader::ModelReader(TrecPointer<TFile> file, TrecPointer<ArenaEngine> engine)
{
	if (!file.get())
		throw L"Error! Null file provided!";
	if (!file->IsOpen())
		throw L"Error! File is closed!";
	if (!engine.get())
		throw L"Error! Null engine provided";

	fileData = file;
	targetEngine = engine;
}


ModelReader::~ModelReader()
{
}

UINT ModelReader::ReadFile()
{
	return 0;
}

TrecPointer<ArenaModel> ModelReader::GetArenaModel(UINT c)
{
	if (c < models.Count())
		return models.ElementAt(c);
	return TrecPointer<ArenaModel>();
}

UINT ModelReader::GetArenaModelCount()
{
	return models.Count();
}

UCHAR ModelReaderType[] = { 3, 0b10000000, 7 };

UCHAR * ModelReader::GetAnaGameType()
{
	return ModelReaderType;
}
