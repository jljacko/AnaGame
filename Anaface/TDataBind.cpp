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
		for (r = startElement; r < data->Count(); r++)
		{
			cont->Resize(getRawSectionLocation(r, 0));
			cont->onDraw(data->ElementAt(r).get());
		}
		break;
	case HBuff:
	case HStack:
	case HMix:
		for (c = startElement; c < data->Count(); c++)
		{
			cont->Resize(getRawSectionLocation(0, c));
			cont->onDraw(data->ElementAt(c).get());
		}
		break;
	case grid:
		if (byRow)
		{
			for (c = startCol; c < colunms; c++)
			{
				for (r = startRow; r < rows; r++)
				{
					cont->Resize(getRawSectionLocation(r, c));
					cont->onDraw(data->ElementAt(startElement++).get());
				}
			}
		}
		else
		{
			for (r = startRow; r < rows; r++)
			{
				for (c = startCol; c < colunms; c++)
				{
					cont->Resize(getRawSectionLocation(r, c));
					cont->onDraw(data->ElementAt(startElement++).get());
				}
			}
		}

	}

}

UCHAR * TDataBind::GetAnaGameType()
{
	return nullptr;
}

void TDataBind::setData(TrecPointer<TArray<TObject>>& data)
{
	this->data = data;
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
