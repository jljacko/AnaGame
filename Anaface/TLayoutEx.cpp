
#include "TLayoutEx.h"


TLayoutEx::TLayoutEx(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> st):TLayout(rt,st)
{
}


TLayoutEx::~TLayoutEx()
{
}

int TLayoutEx::RemoveChildWithPrejudice(UINT x, UINT y)
{
	for (UINT c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
			lChildren.ElementAt(c)->contain.Delete();
	}
	return 0;
}

int TLayoutEx::RemoveColumn(UINT c)
{
	if(c >= columnLines.Size())
		return 1;

	int shift = columnLines[c];

	for (UINT C = c + 1; C < columnLines.Size(); C++)
	{
		columnLines[C - 1] = columnLines[C];
	}
	columnLines.RemoveAt(columnLines.Size() - 1);

	TDataArray<UINT> removeEl;

	for (UINT C = 0; C < lChildren.Count(); C++)
	{
		if (lChildren.ElementAt(C)->x == c)
		{
			lChildren.ElementAt(C)->contain.Delete();
			removeEl.push_back(C);
		}
		else if (lChildren.ElementAt(C)->x > c)
		{
			lChildren.ElementAt(C)->x--;
			lChildren.ElementAt(C)->contain->ShiftHorizontal(-shift);
		}
	}

	for (int C = 0; C < removeEl.Size(); C++)
	{
		lChildren.RemoveAt(C);
	}
}

int TLayoutEx::RemoveRow(UINT c)
{
	if (c >= rowLines.Size())
		return 1;

	int shift = rowLines[c];

	for (UINT C = c + 1; C < rowLines.Size(); C++)
	{
		rowLines[C - 1] = rowLines[C];
	}
	rowLines.RemoveAt(rowLines.Size() - 1);

	TDataArray<UINT> removeEl;

	for (UINT C = 0; C < lChildren.Count(); C++)
	{
		if (lChildren.ElementAt(C)->y == c)
		{
			lChildren.ElementAt(C)->contain.Delete();
			removeEl.push_back(C);
		}
		else if (lChildren.ElementAt(C)->y > c)
		{
			lChildren.ElementAt(C)->y--;
			lChildren.ElementAt(C)->contain->ShiftVertical(-shift);
		}
	}

	for (int C = 0; C < removeEl.Size(); C++)
	{
		lChildren.RemoveAt(C);
	}
}

UINT TLayoutEx::AddRow(UINT size)
{
	// rowLines.push_back(size);
	if (addRow(size,false))
		return rowLines.Size();
	return 0;
}

UINT TLayoutEx::AddCol(UINT size)
{
	if(addColunm(size, false))
		return columnLines.Size();
	return 0;
}

TrecPointer<TControl> TLayoutEx::RemoveChild(UINT x, UINT y)
{
	TrecPointer<TControl> ret;
	for (UINT c = 0; c < lChildren.Count(); c++)
	{
		if (lChildren.ElementAt(c)->x == x && lChildren.ElementAt(c)->y == y)
		{
			ret = lChildren.ElementAt(c)->contain;
			lChildren.RemoveAt(c);
		}
	}
	return ret;
}

bool TLayoutEx::onCreate(RECT r)
{
	return TLayout::onCreate(r);
}

void TLayoutEx::onDraw(TObject* obj)
{
	TLayout::onDraw(obj);
}

UCHAR * TLayoutEx::GetAnaGameType()
{
	return nullptr;
}
