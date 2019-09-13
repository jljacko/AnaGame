#include "TSpreadSheet.h"
#include "TTextField.h"


TSpreadSheet::TSpreadSheet(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta, HWND w): TLayoutEx(rt, ta)
{
	window = w;
	drawLines = true;
	stickToNums = false;
	organization = grid;
	hasTitle = false;
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
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(&h_x);
	}

	valpoint = attributes.retrieveEntry(TString(L"|HeaderHeight"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(&h_y);
	}

	valpoint = attributes.retrieveEntry(TString(L"|HeaderCaption"));
	if (valpoint.Get())
	{
		title = valpoint.Get();
	}



	columnLines.RemoveAll();
	columnFlex.RemoveAll();
	rowLines.RemoveAll();
	rowFlex.RemoveAll();

	int rowCount = 1, columnCount = 1;
	valpoint = attributes.retrieveEntry(TString(L"|RowCount"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(&rowCount);
		if (!rowCount)
			rowCount++;
	}
	valpoint = attributes.retrieveEntry(TString(L"|ColumnCount"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(&columnCount);
		if (!columnCount)
			columnCount++;
	}

	UINT colSize = (l.right - l.left) / columnCount;
	UINT rowSize = (l.bottom - l.top) / rowCount;
	for (UINT Rust = 0; Rust < columnCount; Rust++)
		AddCol(colSize);
	updateRow = true;
	for (UINT Rust = 0; Rust < rowCount; Rust++)
		AddRow(rowSize);


	// Now that we have Rows and Columns, set up the title
	if (title.GetSize() && h_x > 0)
	{
		
		containerControl* cc = nullptr;

		for (UINT Rust = 0; Rust < lChildren.Count(); Rust++)
		{
			containerControl* cc1 = lChildren.ElementAt(Rust).Get();
			if (cc1 && !cc1->x && !cc1->y)
			{
				cc = cc1;
				break;
			}
		}
		if (!cc)
		{
			TrecPointer<containerControl> cc_tc = TrecPointerKey::GetNewTrecPointer<containerControl>();
			lChildren.Add(cc_tc);
		}
		cc->extend = true;
		cc->x = cc->y = 0;
		cc->x2 = h_x - 1;
		cc->y2 = h_y - 1;

		TrecPointer<TControl> ttf = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(renderTarget, styles, window);
		TTextField* ttf_c = dynamic_cast<TTextField*>(ttf.Get());
		ttf_c->SetText(title);
		cc->contain = ttf;
		children.Add(cc->contain);
		TrecPointer<TString> attr;
		attr = TrecPointerKey::GetNewTrecPointer<TString>(L"1,1,1,1");
		ttf->addAttribute(TString(L"|Margin"), attr);
		attr = TrecPointerKey::GetNewTrecPointer<TString>(L"1.0,1.0,1.0,1.0");
		ttf->addAttribute(TString(L"|ContentColor"), attr);
		attr = TrecPointerKey::GetNewTrecPointer<TString>(L"Center");
		ttf->addAttribute(TString(L"|HorizontalAlignment"), attr);
		ttf->onCreate(getRawSectionLocation(cc->y, cc->x));
		hasTitle = true;
	}

	// Now to set up data if provided
	valpoint = attributes.retrieveEntry(TString(L"|SplitCharacters"));
	if (valpoint.Get() && valpoint->GetSize())
		splitTokens = valpoint.Get();
	else
		splitTokens = L";";

	valpoint = attributes.retrieveEntry(TString(L"|SheetData"));
	if (valpoint.Get())
	{
		TrecPointer<TDataArray<TString>> dataPieces = valpoint->split(splitTokens);
		if (!dataPieces.Get() || !dataPieces->Size())
			goto afterData;

		

		UINT lStart = 0;
		if (hasTitle)
			lStart = columnLines.Size();

		for (UINT Rust = 0; Rust < dataPieces->Size() && (lStart + Rust) < lChildren.Count(); Rust++)
		{
			TrecPointer<containerControl> tc_cc = lChildren.ElementAt(Rust + lStart);

			TString tc_ts = dataPieces->at(Rust);
			if (!tc_cc.Get() || !tc_ts.GetSize())
				continue;

			tc_cc->contain = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(renderTarget, styles, window);
			TTextField* ttf = dynamic_cast<TTextField*>(tc_cc->contain.Get());
			ttf->SetText(tc_ts);
			TrecPointer<TString> attr;
			attr = TrecPointerKey::GetNewTrecPointer<TString>(L"2,2,2,2");
			ttf->addAttribute(TString(L"|Margin"), attr);
			ttf->onCreate(getRawSectionLocation(tc_cc->y, tc_cc->x));
			children.Add(tc_cc->contain);
		}
	}

	afterData:

	valpoint = attributes.retrieveEntry(TString(L"|DrawLines"));
	if (valpoint.Get() && !valpoint->Compare(L"False"))
		drawLines = false;

	valpoint = attributes.retrieveEntry(TString(L"|NumOnly"));
	if (valpoint.Get() && !valpoint->Compare(L"True"))
		stickToNums = true;

	return false;
}

void TSpreadSheet::onDraw(TObject* obj)
{
	if (drawLines && internalBrush.Get())
	{
		int add = 0;
		for (int c = 0; c < rowLines.Size(); c++)
		{
			add += rowLines[c];
			renderTarget->DrawLine(D2D1::Point2F(location.left, location.top + add),
				D2D1::Point2F(location.right, location.top + add),
				internalBrush.Get(), thickness);
		}
		add = 0;
		for (int c = 0; c < columnLines.Size(); c++)
		{
			add += columnLines[c];
			renderTarget->DrawLine(D2D1::Point2F(location.left + add, location.top),
				D2D1::Point2F(location.left + add, location.bottom),
				internalBrush.Get(), thickness);
		}
	}
	TControl::onDraw(obj);
}

UCHAR * TSpreadSheet::GetAnaGameType()
{
	return nullptr;
}

TString TSpreadSheet::GetData()
{
	TString returnable;

	UINT start = 0;
	if (hasTitle)
		start = columnLines.Size();

	for (UINT Rust = start; Rust < lChildren.Count(); Rust++)
	{
		TTextField* ttf = dynamic_cast<TTextField*>(lChildren.ElementAt(Rust)->contain.Get());
		if (!ttf)
			continue;
		if (Rust > start)
			returnable += splitTokens[0];

		returnable += ttf->GetText();
	}

	return returnable;
}

TString TSpreadSheet::GetDataSplitTokens()
{
	return splitTokens;
}

void TSpreadSheet::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isContained(&point, &location))
		return;

	TPoint relPoint{ point.x - location.left, point.y - location.top };
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
	if (tc.Get() == nullptr)
	{
		TrecPointer<containerControl> cc = TrecPointerKey::GetNewTrecPointer<containerControl>();
		cc->extend = false;
		cc->x = cc->y = 0;
		TrecPointer<containerControl> cc_tc;
		cc_tc = cc;
		lChildren.Add(cc_tc);
		
		//ttf->SetText(title);
		cc->contain = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TTextField>(renderTarget, styles, window);
		TTextField* ttf = dynamic_cast<TTextField*>(cc->contain.Get());
		TrecPointer<TString> attr;
		attr = TrecPointerKey::GetNewTrecPointer<TString>(L"1,1,1,1");
		ttf->addAttribute(TString(L"|Margin"), attr);
		attr = TrecPointerKey::GetNewTrecPointer<TString>(L"1.0,1.0,1.0,1.0");
		ttf->addAttribute(TString(L"|ContentColor"), attr);
		
		if (stickToNums)
		{
			attr = TrecPointerKey::GetNewTrecPointer<TString>(L"True");
			ttf->addAttribute(TString(L"|IsNumberControl"), attr);
			attr = TrecPointerKey::GetNewTrecPointer<TString>(L"False");
			ttf->addAttribute(TString(L"|DrawNumberBoxes"), attr);
		}

		ttf->onCreate(getRawSectionLocation(cc->y, cc->x));
		tc = cc->contain;
	}

	tc->OnLButtonDown(nFlags, point, mOut, eventAr);
}
