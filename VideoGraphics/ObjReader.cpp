#include "stdafx.h"
#include "ObjReader.h"


ObjReader::ObjReader(TrecPointer<TFile> file, TrecPointer<ArenaEngine> engine) : ModelReader(file,engine)
{
}


ObjReader::~ObjReader()
{
}

UINT ObjReader::ReadFile()
{
	TString line;

	while (fileData->ReadString(line))
	{
		if (!line.GetLength())
			continue; // No meat to the string
		if (line[0] == L'#')
			continue; // This is a comment

		TrecPointer<TArray<TString>> linePieces = line.split(L" ");
		if (!linePieces.get() || !linePieces->Count())
			continue;

		for (UINT c = 0; c < linePieces->Count(); c++)
		{
			if (!linePieces->ElementAt(c).get())
				return 1;
		}

		if (!linePieces->ElementAt(0)->CompareNoCase(L"v"))
		{
			if (linePieces->Count() < 4)
				return 2;
			for (UINT c = 1; c < 4; c++)
			{
				float vertex = 0.0f;
				linePieces->ElementAt(c)->ConvertToFloat(&vertex);
				vertices.push_back(vertex);
			}
			if (linePieces->Count() > 3)
			{
				float vertex = 1.0f;
				linePieces->ElementAt(4)->ConvertToFloat(&vertex);
				vertices.push_back(vertex);
			}
			else
				vertices.push_back(1.0f);

		}
		else if (!linePieces->ElementAt(0)->CompareNoCase(L"vt"))
		{
			if (linePieces->Count() < 3)
				return 3;
			for (UINT c = 1; c < 3; c++)
			{
				float vertex = 0.0f;
				linePieces->ElementAt(c)->ConvertToFloat(&vertex);
				textures.push_back(vertex);
			}
			if (linePieces->Count() > 2)
			{
				float vertex = 1.0f;
				linePieces->ElementAt(3)->ConvertToFloat(&vertex);
				textures.push_back(vertex);
			}
			else
				vertices.push_back(0.0f);
		}
		else if (!linePieces->ElementAt(0)->CompareNoCase(L"vn"))
		{
			if (linePieces->Count() != 4)
				return 4;
			for (UINT c = 1; c < 4; c++)
			{
				float vertex = 0.0f;
				linePieces->ElementAt(c)->ConvertToFloat(&vertex);
				normals.push_back(vertex);
			}
		}
		else if (!linePieces->ElementAt(0)->CompareNoCase(L"f"))
		{
			if (linePieces->Count() == 4)
			{
				for (UINT c = 1; c < 4; c++)
				{
					int n = 0;
					linePieces->ElementAt(c)->ConvertToInt(&n);
					triangularIndex.push_back(n);
				}
			}
			else if (linePieces->Count() == 5)
			{
				for (UINT c = 1; c < 5; c++)
				{
					int n = 0;
					linePieces->ElementAt(c)->ConvertToInt(&n);
					squareIndex.push_back(n);
				}
			}
		}

	}
	return 0;
}

UCHAR ObjReaderType[] = { 4, 0b10000000, 7, 1 };

UCHAR * ObjReader::GetAnaGameType()
{
	return ObjReaderType;
}
