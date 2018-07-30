#include "TSpreadSheet.h"
#include "TTextField.h"


TSpreadSheet::TSpreadSheet(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta, HWND w): TLayoutEx(rt, ta)
{
	window = w;
	drawLines = true;
	stickToNums = false;
	organization = grid;
}


TSpreadSheet::~TSpreadSheet()
{
}

bool TSpreadSheet::onCreate(RECT l)
{
	// Clears all children, as the Spreadsheet is supposed to handle child controls internally
	children.Clear();

	lChildren.Clear();

	TLayoutEx::onCreate(l);

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|HeaderWidth"));
	int h_x = 0, h_y = 1;
	if (valpoint.get())
	{
		valpoint->ConvertToInt(&h_x);
	}

	valpoint = attributes.retrieveEntry(TString(L"|HeaderHeight"));
	if (valpoint.get())
	{
		valpoint->ConvertToInt(&h_y);
	}

	valpoint = attributes.retrieveEntry(TString(L"|HeaderCaption"));
	if (valpoint.get())
	{
		title = valpoint.get();
	}



	columnLines.RemoveAll();
	columnFlex.RemoveAll();
	rowLines.RemoveAll();
	rowFlex.RemoveAll();

	int rowCount = 1, columnCount = 1;
	valpoint = attributes.retrieveEntry(TString(L"|RowCount"));
	if (valpoint.get())
	{
		valpoint->ConvertToInt(&rowCount);
		if (!rowCount)
			rowCount++;
	}
	valpoint = attributes.retrieveEntry(TString(L"|ColumnCount"));
	if (valpoint.get())
	{
		valpoint->ConvertToInt(&columnCount);
		if (!columnCount)
			columnCount++;
	}

	UINT colSize = (l.right - l.left) / columnCount;
	UINT rowSize = (l.bottom - l.top) / rowCount;

	for (UINT Rust = 0; Rust < rowCount; Rust++)
		AddRow(rowSize);
	for (UINT Rust = 0; Rust < columnCount; Rust++)
		AddCol(colSize);

	// Now that we have Rows and Columns, set up the title
	if (title.GetLength() && h_x > 0)
	{
		containerControl* cc = new containerControl;
		cc->extend = true;
		cc->x = cc->y = 0;
		cc->x2 = h_x - 1;
		cc->y2 = h_y - 1;
		TrecPointer<containerControl> cc_tc;
		cc_tc = cc;
		lChildren.Add(cc_tc);
		TTextField* ttf = new TTextField(renderTarget, styles, window);
		ttf->SetText(title);
		cc->contain = ttf;
		children.Add(cc->contain);
		TrecPointer<TString> attr;
		attr = new TString(L"1,1,1,1");
		ttf->addAttribute(TString(L"|Margin"), attr);
		attr = new TString(L"1.0,1.0,1.0,1.0");
		ttf->addAttribute(TString(L"|ContentColor"), attr);
		ttf->onCreate(getRawSectionLocation(cc->y, cc->x));
	}
	valpoint = attributes.retrieveEntry(TString(L"|DrawLines"));
	if (valpoint.get() && !valpoint->Compare(L"False"))
		drawLines = false;

	valpoint = attributes.retrieveEntry(TString(L"|NumOnly"));
	if (valpoint.get() && !valpoint->Compare(L"True"))
		stickToNums = true;

	return false;
}

void TSpreadSheet::onDraw()
{
	if (drawLines && internalBrush.get())
	{
		int add = 0;
		for (int c = 0; c < rowLines.Size(); c++)
		{
			add += rowLines[c];
			renderTarget->DrawLine(D2D1::Point2F(location.left, location.top + add),
				D2D1::Point2F(location.right, location.top + add),
				internalBrush.get(), thickness);
		}
		add = 0;
		for (int c = 0; c < columnLines.Size(); c++)
		{
			add += columnLines[c];
			renderTarget->DrawLine(D2D1::Point2F(location.left + add, location.top),
				D2D1::Point2F(location.left + add, location.bottom),
				internalBrush.get(), thickness);
		}
	}
	TControl::onDraw();
}

UCHAR * TSpreadSheet::GetAnaGameType()
{
	return nullptr;
}

void TSpreadSheet::OnLButtonDown(UINT nFlags, CPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &location))
		return;

	CPoint relPoint{ point.x - location.left, point.y - location.top };
	UINT Rust, addUp = 0;
	for (Rust = 0; Rust < columnLines.Size(); Rust++)
	{
		addUp += columnLines[Rust];
		if (relPoint.x < addUp)
			break;
	}
	if (Rust == columnLines.Size())
		return;
	UINT x_loc = Rust;

	for (Rust = 0, addUp = 0; Rust < rowLines.Size(); Rust++)
	{
		addUp += rowLines[Rust];
		if (relPoint.y < addUp)
			break;
	}
	if (Rust == rowLines.Size())
		return;
	UINT y_loc = Rust;

	TrecPointer<TControl> tc = GetLayoutChild(x_loc, y_loc);
	if (tc.get() == nullptr)
	{
		containerControl* cc = new containerControl;
		cc->extend = false;
		cc->x = cc->y = 0;
		TrecPointer<containerControl> cc_tc;
		cc_tc = cc;
		lChildren.Add(cc_tc);
		TTextField* ttf = new TTextField(renderTarget, styles, window);
		//ttf->SetText(title);
		cc->contain = ttf;
		TrecPointer<TString> attr;
		attr = new TString(L"1,1,1,1");
		ttf->addAttribute(TString(L"|Margin"), attr);
		attr = new TString(L"1.0,1.0,1.0,1.0");
		ttf->addAttribute(TString(L"|ContentColor"), attr);
		
		if (stickToNums)
		{
			attr = new TString(L"True");
			ttf->addAttribute(TString(L"|IsNumberControl"), attr);
			attr = new TString(L"False");
			ttf->addAttribute(TString(L"|DrawNumberBoxes"), attr);
		}

		ttf->onCreate(getRawSectionLocation(cc->y, cc->x));
		tc = ttf;
	}

	tc->OnLButtonDown(nFlags, point, mOut, eventAr);
}
