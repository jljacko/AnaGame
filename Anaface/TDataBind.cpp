#include "TDataBind.h"



TDataBind::TDataBind(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta): TLayoutEx(rt, ta)
{
}


TDataBind::~TDataBind()
{
}

void TDataBind::onDraw(TObject * obj)
{
	UINT r, c;

	UINT startElement = 0, startRow = 0, startCol = 0;
	TrecPointer<TControl> cont = children.ElementAt(0);
	if (!cont.get())
		return;

	switch (organization)
	{
	case VBuff:
	case VStack:
	case VMix:
		if (dataRaw)
		{
			for (r = startElement; r < dataRaw->Size(); r++)
			{
				cont->Resize(getRawSectionLocation(r, 0));
				cont->onDraw(dataRaw->GetObjectAt(r));
			}
		}
		else if (dataWrap)
		{
			for (r = startElement; r < dataWrap->Count(); r++)
			{
				cont->Resize(getRawSectionLocation(r, 0));
				cont->onDraw(dataWrap->GetObjectAt(r));
			}
		}
		break;
	case HBuff:
	case HStack:
	case HMix:
		if (dataRaw)
		{
			for (c = startElement; c < dataRaw->Size(); c++)
			{
				cont->Resize(getRawSectionLocation(0, c));
				cont->onDraw(dataRaw->GetObjectAt(c));
			}
		}
		else if (dataWrap)
		{
			for (c = startElement; c < dataWrap->Count(); c++)
			{
				cont->Resize(getRawSectionLocation(0, c));
				cont->onDraw(dataWrap->GetObjectAt(c));
			}
		}
		break;
	case grid:
		if (dataRaw)
		{
			if (byRow)
			{
				for (c = startCol; c < colunms && startElement < dataRaw->Size(); c++)
				{
					for (r = startRow; r < rows && startElement < dataRaw->Size(); r++)
					{
						cont->Resize(getRawSectionLocation(r, c));
						cont->onDraw(dataRaw->GetObjectAt(startElement++));
					}
				}
			}
			else
			{
				for (r = startRow; r < rows && startElement < dataRaw->Size(); r++)
				{
					for (c = startCol; c < colunms && startElement < dataRaw->Size(); c++)
					{
						cont->Resize(getRawSectionLocation(r, c));
						cont->onDraw(dataRaw->GetObjectAt(startElement++));
					}
				}
			}
		}
		else if (dataWrap)
		{
			if (byRow)
			{
				for (c = startCol; c < colunms && startElement < dataWrap->Count(); c++)
				{
					for (r = startRow; r < rows && startElement < dataWrap->Count(); r++)
					{
						cont->Resize(getRawSectionLocation(r, c));
						cont->onDraw(dataWrap->GetObjectAt(startElement++));
					}
				}
			}
			else
			{
				for (r = startRow; r < rows && startElement < dataWrap->Count(); r++)
				{
					for (c = startCol; c < colunms && startElement < dataWrap->Count(); c++)
					{
						cont->Resize(getRawSectionLocation(r, c));
						cont->onDraw(dataWrap->GetObjectAt(startElement++));
					}
				}
			}
		}

	}

}

UCHAR * TDataBind::GetAnaGameType()
{
	return nullptr;
}



void TDataBind::setData(TDataArrayBase* data)
{
	dataRaw = data;
}
void TDataBind::setData(TArrayBase* data)
{
	dataWrap = data;
}

bool TDataBind::onCreate(RECT r)
{
	byRow = false;
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|DataDirection"));
	if (valpoint.get())
	{
		if (!valpoint->Compare(L"Row") || !valpoint->Compare(L"Horizontal"))
			byRow = true;
	}
	return TLayoutEx::onCreate(r);
}
