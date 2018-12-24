#include "stdafx.h"
#include "TInterpretor.h"


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

	if (sourceFile.Open(sourceName + fileName, CFile::modeReadWrite | CFile::typeText | CFile::modeNoTruncate))
		return false;

	TString inBuffer;

	TString outbuffer;

	CodeMode sourceMode = cm_reg;

	UINT readData = 0, strIndex = 0;
	int  nextSinCom = -1, nextLine = -1, nextMulCom = -1, endMulCom = -1;
	DoubIndex nextSinStr = { -1, 0 }, nextMulStr = { -1,0 };
	int endString = -1;
	while (readData = file.ReadString(inBuffer, 100))
	{
		strIndex = 0;
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
		}

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
