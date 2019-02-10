#include "stdafx.h"
#include "TInterpretor.h"
#include"IntLanguage.h"

TInterpretor::TInterpretor()
{
	actionMode = im_run;
	returnValue = nullptr;
	parent = nullptr;
	resource = ir_none;
	language = nullptr;
}


TInterpretor::~TInterpretor()
{
}

TInterpretor::TInterpretor(TInterpretor* ti)
{
	resource = ir_none;
	actionMode = im_run;
	returnValue = nullptr;
	parent = ti;
	if (ti)
	{
		this->endComment = ti->endComment;
		this->globalVariables = ti->globalVariables;
		this->multiLineString = ti->multiLineString;
		this->singleComment = ti->singleComment;
		this->startComment = ti->startComment;
		this->string = ti->string;
		this->language = ti->language;
	}
	else
	{
		globalVariables = nullptr;
		this->language = nullptr;
	}
}

TInterpretor::TInterpretor(IntLanguage * lang)
{
	resource = ir_none;
	actionMode = im_run;
	returnValue = nullptr;
	parent = nullptr;
	globalVariables = nullptr;
	if (lang)
	{
		this->endComment = lang->endComment;
		
		this->multiLineString = lang->multiLineString;
		this->singleComment = lang->singleComment;
		this->startComment = lang->startComment;
		this->string = lang->string;
		this->language = lang;
	}
	else
	{
		
		this->language = nullptr;
	}
}

bool TInterpretor::SetFile(TFile & file)
{
	if (!file.IsOpen())
		return false;
	fileLoc = 0;

	TString sourceName = file.GetFileDirectory();
	TString fileName = L"_NO_COM_" + file.GetFileName();

	CFileException ex;

	if (!sourceFile.Open(sourceName + fileName, CFile::modeWrite | CFile::typeText | CFile::modeCreate, &ex))
	{
		WCHAR buff[200];
		ex.GetErrorMessage(buff, 200);
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

	while (readData = file.ReadString(inBuffer, 100))
	{
		UpdateDoubIndex(inBuffer, 0, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
		outbuffer = L"";
		for (UINT c = 0; c < inBuffer.GetLength(); c++)
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
					c += sinStr.stringQ.GetLength() - 1;
					sinStr = getNextSingleString(inBuffer, c + 1);
				}
				else if (c == mulStr.strInd)
				{
					sourceMode = cm_mulStr;
					outbuffer += mulStr.stringQ;
					c += mulStr.stringQ.GetLength() - 1;
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
					c = multiComE.strInd + multiComE.stringQ.GetLength() - 1;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				break;
			case cm_sinStr:
				if (c == sinStr.strInd)
				{
					outbuffer += sinStr.stringQ;
					c += sinStr.stringQ.GetLength() - 1;
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
					c += mulStr.stringQ.GetLength() - 1;
					sourceMode = cm_reg;
					UpdateDoubIndex(inBuffer, c + 1, sinCom, newL, multiCom, multiComE, sinStr, mulStr);
				}
				else
				{
					outbuffer += inBuffer[c];
				}
			} // End of Switch
		} // end of for


		sourceFile.WriteString(outbuffer);

		/*strIndex = 0;
		while (strIndex < readData)
		{
			nextSinStr = getSingleString(inBuffer, strIndex);
			nextSinCom = getSingleComment(inBuffer, strIndex);
			nextLine = getNextLine(inBuffer, strIndex);
			nextMulCom = getMultiComment(inBuffer, strIndex, true);
			endMulCom = getMultiComment(inBuffer, strIndex, false);
			nextMulStr = getMultiString(inBuffer, strIndex);

			UINT startStrIndex = strIndex;
			outbuffer.Empty();

			switch (sourceMode)
			{
			case cm_reg:
				if (nextSinCom == -1 && nextMulCom == -1)
				{
					// In this case, there is no comment and we can safely copy the entire String into the output.
					// We do, however, have to check for strings
					if(!setStringMode(sourceMode, readData, nextSinStr, nextMulStr))
						strIndex = readData;
					
				}
				else
				{	// We are dealing with comments at this point
					if (nextSinCom > -1 && (nextMulCom == -1 && nextMulCom > nextSinCom))
					{
						if (!setStringMode(sourceMode, readData, nextSinStr, nextMulStr))
						{
							strIndex = nextSinCom;
							sourceMode = cm_sinCom;
						}
						
						
					}
					else
					{
						if (!setStringMode(sourceMode, readData, nextSinStr, nextMulStr))
						{
							strIndex = nextMulCom;
							sourceMode = cm_mulCom;
						}
					}
				}
				outbuffer = inBuffer.SubString(startStrIndex, strIndex);
				break;
			case cm_sinCom:
				if (nextLine != -1)
				{
					strIndex = nextLine;
					sourceMode = cm_reg;
				}
				else
					strIndex = readData;
				break;
			case cm_sinStr:
				endString = getMulStringEnd(inBuffer, strIndex, nextMulStr.colInd);
				if (endString == -1 && (nextLine != -1 && nextLine < endString))
				{
					strIndex = nextLine;
				}
				else if (endString == -1 && nextLine == -1)
					strIndex = readData;
				else
					strIndex = endString + (multiLineString[nextMulStr.colInd].GetLength() - 1);
				outbuffer = inBuffer.SubString(startStrIndex, strIndex);
				break;
			case cm_mulCom:
				if (endMulCom != -1)
				{
					strIndex = endMulCom;
					sourceMode = cm_reg;
				}
				else
					strIndex = readData;
				break;
			case cm_mulStr:
				endString = getMulStringEnd(inBuffer, strIndex, nextMulStr.colInd);
				if (endString == -1)
					strIndex = readData;
				else
					strIndex = endString + (multiLineString[nextMulStr.colInd].GetLength() - 1);
				outbuffer = inBuffer.SubString(startStrIndex, strIndex);
			}

			sourceFile.WriteString(outbuffer);
		}*/

	}

	sourceFile.Seek(0, CFile::SeekPosition::begin);
}

bool TInterpretor::SetFile(TFile & file, ULONG seek)
{
	if(!file.IsOpen())
		return false;
	file.Seek(seek, CFile::SeekPosition::begin);
	return true;
}

void TInterpretor::setLanguage(IntLanguage * lang)
{
	if (lang)
		language = lang;
}

void TInterpretor::SetString(TString & strCode)
{
	sourceString = strCode;
	resource = ir_string;
}

void TInterpretor::SetGlobalVariables(VariableTree * vt)
{
	globalVariables = vt;
}

void TInterpretor::SendFlowMessage(InterpretorMessage im, intVariable* ret)
{
	returnValue = ret;
	actionMode = im;
}

intVariable * TInterpretor::GetVariable(TString & name)
{
	intVariable* ret = localVariables.getVariable(name);
	if (ret)
		return ret;
	if (!parent && globalVariables)
		ret = globalVariables->getVariable(name);
	if (ret)
		return ret;

	if (parent)
		return parent->GetVariable(name);

	return nullptr;
}

UINT TInterpretor::Run(TInterpretor * t)
{
	return 0;
}

UINT TInterpretor::Run(TInterpretor * t, VariableList & parameters)
{
	return 0;
}

UINT TInterpretor::Run(TInterpretor * t, TString & arguements)
{
	return 0;
}



void TInterpretor::UpdateDoubIndex(TString & buff, UINT index, DoubIndex & sinCom, DoubIndex & newLine, DoubIndex & mulCom, DoubIndex & mulComE, DoubIndex & sinStr, DoubIndex & mulStr)
{
	sinCom = getNextSingleComment(buff, index);
	newLine = getNextNewline(buff, index);
	mulCom = getNextMultiComment(buff, index);
	mulComE = getNextMultiCommentEnd(buff, index);
	sinStr = getNextSingleString(buff, index);
	mulStr = getNextMultiString(buff, index);
}

DoubIndex TInterpretor::getNextSingleComment(TString & buff, UINT index)
{
	DoubIndex ret{ -1, singleComment };

	ret.strInd = buff.Find(singleComment, index);

	return ret;
}

DoubIndex TInterpretor::getNextNewline(TString & buff, UINT index)
{
	DoubIndex ret{ -1, TString(L"\n") };

	ret.strInd = buff.Find(ret.stringQ, index);

	return ret;
}

DoubIndex TInterpretor::getNextMultiComment(TString & buff, UINT index)
{
	DoubIndex ret{ -1, startComment };

	ret.strInd = buff.Find(ret.stringQ, index);

	return ret;
}

DoubIndex TInterpretor::getNextMultiCommentEnd(TString & buff, UINT index)
{
	DoubIndex ret{ -1, endComment };

	ret.strInd = buff.Find(ret.stringQ, index);

	return ret;
}

DoubIndex TInterpretor::getNextSingleString(TString & buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < string.Size(); rust++)
	{
		int loc = buff.Find(string[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ = string[rust];
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

DoubIndex TInterpretor::getNextMultiString(TString & buff, UINT index)
{
	DoubIndex ret;
	ret.strInd = -1;

	for (UINT rust = 0; rust < multiLineString.Size(); rust++)
	{
		int loc = buff.Find(multiLineString[rust], index);
		if (loc != -1 && (ret.strInd == -1 || loc < ret.strInd))
		{
			ret.stringQ = multiLineString[rust];
			ret.strInd = loc;
			break;
		}
	}

	return ret;
}

/*
DoubIndex TInterpretor::getSingleString(TString & read, UINT in)
{
	DoubIndex ret = { -1,0 };

	for (UINT c = 0; c < string.Size(); c++)
	{
		int newRet = read.Find(string[c], in);
		if (ret.strInd == -1 || newRet < ret.strInd)
		{
			ret.strInd = newRet;
			ret.colInd = c;
		}
	}
	return ret;
}

DoubIndex TInterpretor::getMultiString(TString & read, UINT in)
{
	DoubIndex ret = { -1, 0 };

	for (UINT c = 0; c < multiLineString.Size(); c++)
	{
		int newRet = read.Find(multiLineString[c], in);
		if (ret.strInd == -1 || newRet < ret.strInd)
		{
			ret.strInd = newRet;
			ret.colInd = c;
		}
	}
	return ret;
}

int TInterpretor::getStringEnd(TString & read, UINT in, UINT strIn)
{
	if(strIn > string.Size())
		return -1;
	return read.Find(string[strIn], in);
}

int TInterpretor::getMulStringEnd(TString & read, UINT in, UINT dIn)
{
	if (dIn > multiLineString.Size())
		return -1;
	return read.Find(multiLineString[dIn], in);
}

int TInterpretor::getSingleComment(TString & read, UINT in)
{
	return read.Find(singleComment, in);
}

int TInterpretor::getMultiComment(TString & read, UINT in, bool start)
{
	if (start)
		return read.Find(startComment, in);
	else return read.Find(endComment, in);
}

int TInterpretor::getNextLine(TString & read, UINT in)
{
	return read.Find(L'\n', in);
}

bool TInterpretor::setStringMode(CodeMode & sourceMode, UINT & readData, DoubIndex& nextSinStr, DoubIndex& nextMulStr)
{
	if (nextSinStr.strInd != -1 && (nextMulStr.strInd == -1 || nextMulStr.strInd > nextSinStr.strInd))
	{
		// In this case, we have a Single line String to check for
		readData = nextSinStr.strInd;
		sourceMode = cm_sinStr;
		return true;
	}
	else if (nextMulStr.strInd != -1 && (nextSinStr.strInd == -1 || nextMulStr.strInd < nextSinStr.strInd))
	{
		readData = nextSinStr.strInd;
		sourceMode = cm_mulStr;
		return true;
	}
	return false;
}
*/