
#include "AnafaceUI.h"

/*
* Method: (AnaFaceUI) (Constructor)
* Purpose: Sets up the AnafaceUI
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - the render target to use
*				TrecPointer <TArray<styleTable>> ta - the styles for Anaface
*				HWND win - the window handle to use
* Returns void
*/
AnafaceUI::AnafaceUI(TrecComPointer<ID2D1RenderTarget>rt, TrecPointer <TArray<styleTable>> ta, HWND win):TControl(rt,ta,false)
{
	winHandle = win;
	tabHeight = unknownTab = 0;
	tabs = nullptr;
	proceed = true;
}

/*
* Method: (AnafaceUI) (Destructor)
* Purpose: Cleans up the Anaface UI 
* Parameters: void 
* Returns: void
*/
AnafaceUI::~AnafaceUI()
{
	//TrecPointer <auiHold> holdTemp;
	//for (int c = 0; c < children.Count(); c++)
	//{
	//	holdTemp = children.ElementAt(c);
	//	holdTemp->children.Clear();
	//	holdTemp->source.Clear();
	//}
	//TControl::~TControl();
}

/*
* Method: AnafaceUI - switchView
* Purpose: Switches the view towards the specified control
* Parameters: UINT x - the index of the control to use
* Returns: bool - whether the operation is successful
*/
bool AnafaceUI::switchView(UINT x)
{
	if (children.Count() > x)
	{
		try
		{
			currentControl = children.ElementAt(x);
			return true;
		}
		catch (...)
		{
			currentControl.Nullify();
		}
	}
	return false;
}

void AnafaceUI::SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget> rt)
{
	TControl::SetNewRenderTarget(rt);
	if (tabs)
		tabs->SetNewRenderTarget(rt);
	
}

/*
int AnafaceUI::loadFromTML(CArchive * ar)
{
	TrecPointer <auiHold> temp;
	TrecPointer <AnafaceUI> uiTemp;
	TrecPointer <CString> strTemp;
	CFile tempFile;
	CArchive* tempArchive = NULL;
	int error = 0;
	for (int c = 0; c < children.Count(); c++)
	{
		temp = children.ElementAt(c);
		for (int d = 0; d < temp->children.Count(); d++)
		{
			uiTemp = temp->children.ElementAt(d);
			if (!uiTemp)
				return 1;
			strTemp = temp->source.ElementAt(d);
			if (!strTemp)
			{
				return 2;
			}
			if (!tempFile.Open(*strTemp, CFile::modeRead))
				return 3;
			tempArchive = new CArchive(&tempFile, CArchive::load, 12000);
			error = uiTemp->loadFromTML(tempArchive); // get an error message

			// Clode File Resources
			tempArchive->Close();
			delete tempArchive; // delete any unecessary memory
			tempArchive = NULL; // set NULL by convention
			tempFile.Close(); // Close the File so it could be used again
			if (error) // if this is 0, no errors. Otherwise, needs to be address
			{
				return 10 + error; // let the double (or unlikely triple) digits decide how deep down a given anaface was
				// use single digits to specify the error
			}
		}
	}
	TControl::loadFromTML(ar);
	return 0;
}*/


void AnafaceUI::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
}

/*
* Method: AnafaceUI - OnLButtonDown
* Purpose: Allows the AnafaceUI to catch when a tab is 
* Parameters: UINT nFlags - flags provided by MFC
*			CPoint point - the point of the click
*			messageOutput * mOut - the result of the click (was it in the control?)
*			TDataArray<EventID_Cred>& eventAr - events and their handlers documented by the control
* Returns: void
*/
void AnafaceUI::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;
			
			messageOutput tempOut = negative;
			tcon->OnLButtonDown(nFlags, point, &tempOut, eventAr, clickedControl);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					resetArgs();
					args.eventType = R_Message_Type::On_sel_change;
					args.point = point;
					args.methodID = -1;
					args.isClick = true;
					args.isLeftClick = true;
					args.control = this;
					args.arrayLabel = c;
					eventAr.push_back(EventID_Cred{ On_Click,this });
					return;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
}

void AnafaceUI::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;

			messageOutput tempOut = negative;
			tcon->OnRButtonDown(nFlags, point, &tempOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnRButtonDown(nFlags, point, mOut, eventAr);
}

void AnafaceUI::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;

			messageOutput tempOut = negative;
			tcon->OnMouseMove(nFlags, point, &tempOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnMouseMove(nFlags, point, mOut, eventAr);
}

void AnafaceUI::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;

			messageOutput tempOut = negative;
			tcon->OnLButtonDblClk(nFlags, point, &tempOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnLButtonDblClk(nFlags, point, mOut, eventAr);
}

void AnafaceUI::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;

			messageOutput tempOut = negative;
			tcon->OnLButtonUp(nFlags, point, &tempOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnLButtonUp(nFlags, point, mOut, eventAr);
}

bool AnafaceUI::OnChar(bool fromChar, UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs)
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).Get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.Get())
				continue;

			messageOutput tempOut = negative;
			tcon->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).Get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return true;
				}
			}
		}
	}

	if (currentControl.Get() && proceed)
		currentControl->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr);
}


/*
* Method: AnafaceUI - onCreate
* Purpose: Sets up the switching control with AnafaceUI specific attributes
* Parameters: RECT container - the location the control will be stored
* Returns: bool
*/
bool AnafaceUI::onCreate(D2D1_RECT_F container, TrecPointer<TWindowEngine> d3d)
{
	windowEngine = d3d;
	TControl::onCreate(container,d3d);
	D2D1_RECT_F r;
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|TabHeight"));
	if (valpoint.Get())
	{
		tabs_base = TrecPointerKey::GetNewSelfTrecPointerAlt<TControl, TLayoutEx>(renderTarget, styles);
		tabs = dynamic_cast<TLayoutEx*>(tabs_base.Get());
		tabs->setLayout(HStack);
		if (valpoint->ConvertToInt(&tabHeight))
			tabHeight = 30;

		r = D2D1_RECT_F{ location.left, location.top, location.right, location.top + tabHeight };
		for (UINT C = 0; C < children.Count(); C++)
		{
			TControl* tcon = children.ElementAt(C).Get();
			if (!tcon)
				continue;
			
			if (!tcon)
				continue;
			tabs->addColunm(60, true);
			TrecPointer<TControl> newTab = TrecPointerKey::GetNewSelfTrecPointer<TControl>(renderTarget, styles);
			tabs->addChild(newTab, tabs->getColumnNumber() - 1, 0);
		}

		if (styles.Get())
		{
			valpoint = attributes.retrieveEntry(TString(L"|TabStyle"));
			if (valpoint.Get())
			{
				styleTable* st;
				for (UINT c = 0; c < styles->Count(); c++)
				{
					if (!styles->ElementAt(c).Get())
						continue;
					st = styles->ElementAt(c).Get();
					if (st->style == valpoint.Get())
					{
						for (UINT C = 0; C < children.Count(); C++)
						{
							TrecPointer<TControl> newTab = tabs->GetLayoutChild(C,0);
							if(!newTab.Get())
								continue;
							for (UINT rust = 0; rust < st->names.count(); rust++)
							{
								tEntry<TString>* ent = st->names.GetEntryAt(rust).Get();
								if (!ent)
									continue;
								newTab->addAttribute(ent->key, ent->object);
							}
						}
					}
				}
			} // End of Valpoint if for "TabStyle"

		} // End of Tab Style
		if (children.Count() && children.ElementAt(0).Get())
			currentControl = children.ElementAt(0);

	}// End of valpoint if statement for "TabHeight" and code for creating tabs

	container.top += tabHeight;

	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc = children.ElementAt(c).Get();
		if (tc)
			tc->onCreate(container,d3d);
	}

	if (tabs)
	{
		tabs->onCreate(r,d3d);

		for (UINT c = 0; c < children.Count(); c++)
		{
			TControl* tcon = children.ElementAt(c).Get();
			if (!tcon)
				continue;
			
			TrecPointer<TControl> tc2 = tabs->GetLayoutChild(c, 0);
			if (!tc2.Get())
				continue;
			TString id = tcon->GetID();
			TrecPointer<TText> text = tc2->getText(1);
			if (!text.Get())
				continue;
			text->setCaption(id);
		}

		
	}

	valpoint = attributes.retrieveEntry(TString(L"|BeginningChildIndex"));
	int ind = -1;
	if (valpoint.Get() && !valpoint->ConvertToInt(&ind))
	{
		if (ind > -1 && ind < children.Count())
			currentControl = children.ElementAt(ind);
	}

	valpoint = attributes.retrieveEntry(TString(L"|ExternalApp"));
	if (valpoint.Get() && valpoint->CompareNoCase(TString(L"true")))
	{
		proceed = false;
	}


	return false;
}


/*
* Method: AnafaceUI - addControl
* Purpose: Adds a new control to the list of child controls
* Parameters: TrecPointer<TControl> control - the control to add
* Returns: int - the index of the control (-1 if failed)
*/
int AnafaceUI::addControl(TrecPointer<TControl> control, TString tabName)
{
	if (control.Get())
	{
		for (UINT c = 0; c < children.Count(); c++)
		{
			if (control.Get() == children.ElementAt(c).Get())
			{
				currentControl = children.ElementAt(c);
				return c;
			}
		}
		children.Add(control);
		currentControl = control;

		AddNewTab(tabName);


		return children.Count() - 1;
	}
	return -1;
}

/*
* Method: AnafaceUI - addControl
* Purpose: Adds a new control that needs to be read into memory
* Parameters: CArchive * arch - the file to read from
* Returns: int - 0 if successful
* Note: INCOMPLETE - not properly implimented, DEPRECIATED - CArchive should be replaced with TFile
*/
int AnafaceUI::addControl(TFile * arch)
{
	if (!arch)
		return -2;


	if (!arch)
		return -3;
	TString fileName = arch->GetFileName();

	return 0;
}

/*
* Method: AnafaceUI - setDontAddControl
* Purpose: Adds a control without adding it to the
* Parameters: TrecPointer<TControl> control - the control to draw
* Returns: void
*/
void AnafaceUI::setDontAddControl(TrecPointer<TControl> control)
{
	if (control.Get())
	{
		//TrecPointer<TContainer> tc(new TContainer());
		//tc->setTControl(control);
		control->onCreate(location,windowEngine);
		currentControl = control;
	}
}

/*
* Method: AnafaceUI - onDraw
* Purpose: Draws the control and any tabs present
* Parameters: void
* Returns: void
*/
void AnafaceUI::onDraw(TObject* obj)
{
	//TControl::onDraw();
	if (!isActive)
		return;
	if (mState == mouseLClick)
	{
		if (content3.Get())
			content3->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		if (border3.Get())
			border3->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text3.Get())
			text3->onDraw(location, obj);
		else if (text1.Get())
			text1->onDraw(location, obj);
	}
	else if (mState == mouseHover)
	{
		if (content2.Get())
			content2->onDraw(location);
		else if (content1.Get())
			content1->onDraw(location);
		if (border2.Get())
			border2->onDraw(location);
		else if (border1.Get())
			border1->onDraw(location);
		if (text2.Get())
			text2->onDraw(location, obj);
		else if (text1.Get())
			text1->onDraw(location, obj);
	}
	else
	{
		if (content1.Get())
			content1->onDraw(location);
		if (border1.Get())
			border1->onDraw(location);
		if (text1.Get())
			text1->onDraw(location, obj);
	}
	/*
	if (vScroll)
		vScroll->updateDraw();
	if (hScroll)
		hScroll->updateDraw();
	*/
	if (tabs)
		tabs->onDraw(obj);
	if (currentControl.Get() && proceed)
		currentControl->onDraw(obj);
}

/*
* Method: AnafaceUI - GetChildAt
* Purpose: Retrieves the child control at the given location
* Parameters: UINT c - the index of the control to look for
* Returns: TrecPointer<TControl> - the control at the given index
*/
TrecPointer<TControl> AnafaceUI::GetChildAt(UINT c)
{
	if (c < children.Count())
		return children.ElementAt(c);
	return TrecPointer<TControl>();
}

/*
* Method: AnafaceUI - GetCurrentChild
* Purpose: Retrieves the currently active body control
* Parameters: void
* Returns: TrecPointer<TControl> - the current control active
*/
TrecPointer<TControl> AnafaceUI::GetCurrentChild()
{
	return currentControl;
}

/*
* Method: AnafaceUI - GetAnaGameType
* Purpose: Enables system to report whether the object is in fact an AnafaceUI
* Parameters: void
* Returns: UCHAR* - the AnaGame representation of the AnafaceUI
*/
UCHAR * AnafaceUI::GetAnaGameType()
{
	return nullptr;
}

void AnafaceUI::AddNewTab(TString t)
{
	if (tabs)
	{
		TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|TabStyle"));
		TrecPointer<TControl> tc(TrecPointerKey::GetNewSelfTrecPointer<TControl>(renderTarget, styles));
		if (valpoint.Get())
		{
			styleTable* st;
			for (UINT c = 0; c < styles->Count(); c++)
			{
				if (!styles->ElementAt(c).Get())
					continue;
				st = styles->ElementAt(c).Get();

				if (st->style == valpoint.Get())
				{

					for (UINT rust = 0; rust < st->names.count(); rust++)
					{

						tEntry<TString>* ent = st->names.GetEntryAt(rust).Get();
						if (!ent)
							continue;
						tc->addAttribute(ent->key, ent->object);

					}

				}
			}

		}
		if (!t.Compare(L""))
		{
			t.Format(L"Unknown (%d)", unknownTab++);
		}

		tc->addAttribute(TString(L"|Caption"), TrecPointerKey::GetNewTrecPointer<TString>(t));
		UINT indexAdd = tabs->AddCol(40);
		if (indexAdd)
		{
			tabs->addChild(tc, indexAdd - 1, 0);
			D2D1_RECT_F tabLoc = tabs->getRawSectionLocation(0, indexAdd - 1);
			tc->onCreate(tabLoc,windowEngine);
		}
	}
}

/*
* Method: AnafaceUI - GetControlArea
* Purpose: Retrieves the location of the main body control (tabs might reduce the actual size)
* Parameters: void
* Returns: RECT - the location of the main body area
*/
D2D1_RECT_F AnafaceUI::GetControlArea()
{
	D2D1_RECT_F returnable = location;
	returnable.top += tabHeight;
	return returnable;
}

void AnafaceUI::Resize(D2D1_RECT_F r)
{
	location = r;
	r.top += tabHeight;

	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		if (children.ElementAt(Rust).Get())
			children.ElementAt(Rust)->Resize(r);
	}

	if (tabs)
	{
		D2D1_RECT_F tabLoc = location;
		tabLoc.bottom = tabLoc.top + tabHeight;
		tabs->Resize(tabLoc);
	}
	updateComponentLocation();
}
