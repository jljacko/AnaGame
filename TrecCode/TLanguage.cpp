#include "pch.h"
#include "TLanguage.h"

TrecPointer<TLanguage> TLanguage::getLanguage(TString& langName)
{
	return TrecPointer<TLanguage>();
}

TrecPointer<TLanguage> TLanguage::getLanguage(TString& langName, TString& langVersion)
{
	return TrecPointer<TLanguage>();
}

UINT TLanguage::PreProcessFile(TrecPointer<TFile>& sourceFile)
{
	TString nextFile;
	if (!RunCommentFilter(sourceFile, nextFile))
		return 1;
	return 0;
}

TLanguage::TLanguage()
{
}

TLanguage::~TLanguage()
{
}

bool TLanguage::RunCommentFilter(TrecPointer<TFile> file, TString& newFileName)
{
	if (!file.Get() || !file->IsOpen())
		return false;

	TString sourceName = file->GetFileDirectory();
	newFileName = TString(L"_NO_COM_") + file->GetFileName();


	TrecPointer<TFile> sourceFile = TrecPointerKey::GetNewTrecPointer<TFile>();

	if (!sourceFile->Open(sourceName + newFileName, TFile::t_file_write | TFile::t_file_create_always))
	{
		return false;
	}
	TString inBuffer;

	TString outbuffer;

	CodeMode sourceMode = cm_reg;

	UINT readData = 0, strIndex = 0;
	int  nextSinCom = -1, nextLine = -1, nextMulCom = -1, endMulCom = -1;
	//DoubIndex nextSinStr = { -1, 0 }, nextMulStr = { -1,0 };
	int endString = -1;
	DoubIndex sinCom, newL, multiCom, multiComE, sinStr, mulStr;

	while (readData = file->ReadString(inBuffer, (UINT)100))
	{
		UpdateDoubIndex(inBuffer, 0, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
		outbuffer.Set(L"");
		for (UINT c = 0; c < inBuffer.GetSize(); c++)
		{

			switch (sourceMode)
			{
			case cm_reg:
				if (c == sinCom.strInd)
				{
					sourceMode = cm_sinCom;
				}
				else if (c == multiCom.strInd)
				{
					sourceMode = cm_mulCom;
				}
				else if (c == sinStr.strInd)
				{
					sourceMode = cm_sinStr;
					outbuffer += sinStr.stringQ;
					c += sinStr.stringQ.GetSize() - 1;
					sinStr = getNextSingleString(inBuffer, c + 1);
				}
				else if (c == mulStr.strInd)
				{
					sourceMode = cm_mulStr;
					outbuffer += mulStr.stringQ;
					c += mulStr.stringQ.GetSize() - 1;
					mulStr = getNextMultiString(inBuffer, c + 1);
				}
				else
					outbuffer += inBuffer[c];
				break;
			case cm_sinCom:
				if (newL.strInd == -1 || newL.strInd < c)
					newL = getNextNewline(inBuffer, c);
				if (newL.strInd != -1)
				{
					outbuffer += L'\n';
					c = newL.strInd;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				break;
			case cm_mulCom:
				newL = getNextNewline(inBuffer, c);
				while (newL.strInd != -1 && (multiComE.strInd == -1 || newL.strInd < multiComE.strInd))
				{
					outbuffer += L'\n';
					c = newL.strInd;
					newL = getNextNewline(inBuffer, c + 1);
				}
				if (multiComE.strInd != -1)
				{
					c = multiComE.strInd + multiComE.stringQ.GetSize() - 1;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				break;
			case cm_sinStr:
				if (c == sinStr.strInd)
				{
					outbuffer += sinStr.stringQ;
					c += sinStr.stringQ.GetSize() - 1;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				else
				{
					if (c == newL.strInd)
					{
						sinStr = getNextSingleString(inBuffer, c);
						sourceMode = cm_reg;
						UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
					}
					outbuffer += inBuffer[c];
				}

				break;
			case cm_mulStr:
				if (c == mulStr.strInd)
				{
					outbuffer += mulStr.stringQ;
					c += mulStr.stringQ.GetSize() - 1;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				else
				{
					outbuffer += inBuffer[c];
				}
			} // End of Switch
		} // end of for


		sourceFile->WriteString(outbuffer);



	}
	sourceFile->Close();

	return true;
}

void TLanguage::UpdateDoubIndex(TString& buff, UINT index, DoubIndex& sinCom, DoubIndex& newLine, DoubIndex& mulCom, DoubIndex& mulComE, DoubIndex& sinStr, DoubIndex& mulStr)
{
	sinCom = getNextSingleComment(buff, index);
	newLine = getNextNewline(buff, index);
	mulCom = getNextMultiComment(buff, index);
	mulComE = getNextMultiCommentEnd(buff, index);
	sinStr = getNextSingleString(buff, index);
	mulStr = getNextMultiString(buff, index);
}

DoubIndex TLanguage::getNextSingleComment(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < singleComment.Size(); rust++)
	{
		int loc = buff.Find(singleComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(singleComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextNewline(TString& buff, UINT index)
{
	DoubIndex ret{ -1, TString(L"\n") };

	ret.strInd = buff.Find(ret.stringQ, index);

	return ret;
}

DoubIndex TLanguage::getNextMultiComment(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < startComment.Size(); rust++)
	{
		int loc = buff.Find(startComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(startComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextMultiCommentEnd(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < endComment.Size(); rust++)
	{
		int loc = buff.Find(endComment[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(endComment[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextSingleString(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < string.Size(); rust++)
	{
		int loc = buff.Find(string[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(string[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TLanguage::getNextMultiString(TString& buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < multiLineString.Size(); rust++)
	{
		int loc = buff.Find(multiLineString[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ.Set(multiLineString[rust]);
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}
