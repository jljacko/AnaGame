#include "TDataBind.h"



TDataBind::TDataBind(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta): TControl(rt, ta)
{
	isStack = true;
	widthHeight = 90;
	dataRaw = nullptr;
	dataWrap = nullptr;
}


TDataBind::~TDataBind()
{
}

void TDataBind::onDraw(TObject * obj)
{
	UINT r, c;

	UINT startElement = 0, startRow = 0, startCol = 0;
	TrecPointer<TControl> cont = children.ElementAt(0);
	if (!cont.Get())
		return;
	RECT original = cont->getLocation();


	if (dataRaw)
	{
		for (UINT Rust = 0; Rust < dataRaw->Size(); Rust++)
		{
			RECT curLoc = cont->getLocation();
			if (isStack)
			{
				if (curLoc.bottom > location.bottom)
					break;
			}
			else
			{
				if (curLoc.right > location.right)
					break;
			}

			cont->onDraw(dataRaw->GetObjectAt(Rust));

			if (isStack)
			{
				curLoc.bottom += widthHeight;
				curLoc.top += widthHeight;
			}
			else
			{
				curLoc.left += widthHeight;
				curLoc.right += widthHeight;
			}
			cont->setLocation(curLoc);
		}
	}
	else if (dataWrap)
	{
		for (UINT Rust = 0; Rust < dataWrap->Count(); Rust++)
		{
			RECT curLoc = cont->getLocation();
			if (isStack)
			{
				if (curLoc.bottom > location.bottom)
					break;
			}
			else
			{
				if (curLoc.right > location.right)
					break;
			}

			messageState curState = cont->mState;


			if (isContained(&mouseMovePoint, &curLoc))
				cont->mState = mouseHover;

			cont->onDraw(dataWrap->GetObjectAt(Rust));

			cont->mState = curState;

			if (isStack)
			{
				curLoc.bottom += widthHeight;
				curLoc.top += widthHeight;
			}
			else
			{
				curLoc.left += widthHeight;
				curLoc.right += widthHeight;
			}
			cont->setLocation(curLoc);
		}
	}
	cont->setLocation(original);
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

bool TDataBind::onCreate(RECT r, TrecPointer<TWindowEngine> d3d)
{
	RECT loc = r;
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|ColumnWidth"));
	if (valpoint.Get())
	{
		int value = 0;
		if (!valpoint->ConvertToInt(&value))
		{
			widthHeight = value;
			isStack = false;

			if (r.right - r.left > widthHeight)
			{
				r.right = r.left + widthHeight;
			}

		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|RowHeight"));
	if (valpoint.Get())
	{
		r = loc;
		int value = 0;
		if (!valpoint->ConvertToInt(&value))
		{
			widthHeight = value;
			isStack = true;

			if (r.bottom - r.top > widthHeight)
			{
				r.bottom = r.top + widthHeight;
			}
		}
	}

	bool ret = TControl::onCreate(loc, d3d);
	
	if (children.Count() && children.ElementAt(0).Get())
	{
		children.ElementAt(0)->onCreate(r, d3d);
	}

	return ret;
}

/*void TDataBind::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons)
{
	TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedButtons);
	if (mState == mouseLClick)
	{
		UINT items = 0;
		if (dataRaw)
			items = dataRaw->Size();
		else if (dataWrap)
			items = dataWrap->Count();

		TrecPointer<TControl> cont = children.ElementAt(0);
		if (!cont.Get())
			return;
		RECT original = cont->getLocation();

		for (UINT Rust = 0; Rust < items && isContained(&point, &location); Rust++)
		{
			if (isContained(&point, &original))
			{
				resetArgs();
				args.arrayLabel = Rust;
				args.isClick = true;
				args.isLeftClick = true;
				args.eventType = On_sel_change;
				args.control = this;
				args.methodID = getEventID(On_sel_change);
				args.point = point;
				eventAr.push_back({ On_sel_change, this });
				break;


			}
			if (isStack)
			{
				original.bottom += widthHeight;
				original.top += widthHeight;
			}
			else
			{
				original.left += widthHeight;
				original.right += widthHeight;
			}
		}


	}
}*/

void TDataBind::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	bool wasClicked = mState == mouseLClick;
	TControl::OnLButtonUp(nFlags, point, mOut, eventAr);
	if (wasClicked && isContained(&point, &location))
	{
		UINT items = 0;
		if (dataRaw)
			items = dataRaw->Size();
		else if (dataWrap)
			items = dataWrap->Count();

		TrecPointer<TControl> cont = children.ElementAt(0);
		if (!cont.Get())
			return;
		RECT original = cont->getLocation();

		for (UINT Rust = 0; Rust < items && isContained(&point, &location); Rust++)
		{
			if (isContained(&point, &original))
			{
				resetArgs();
				args.arrayLabel = Rust;
				args.isClick = true;
				args.isLeftClick = true;
				args.eventType = On_sel_change;
				args.control = this;
				args.methodID = getEventID(On_sel_change);
				args.point = point;
				eventAr.push_back({ On_sel_change, this });
				break;


			}
			if (isStack)
			{
				original.bottom += widthHeight;
				original.top += widthHeight;
			}
			else
			{
				original.left += widthHeight;
				original.right += widthHeight;
			}
		}


	}
}

void TDataBind::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	TControl::OnMouseMove(nFlags, point, mOut, eventAr);
	if (mState == mouseHover)
		mouseMovePoint = point;
}
