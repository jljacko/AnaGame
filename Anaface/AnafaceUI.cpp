#include "stdafx.h"
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
			currentControl = nullptr;
		}
	}
	return false;
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


/*
* Method: AnafaceUI - OnLButtonDown
* Purpose: Allows the AnafaceUI to catch when a tab is 
* Parameters: UINT nFlags - flags provided by MFC
*			CPoint point - the point of the click
*			messageOutput * mOut - the result of the click (was it in the control?)
*			TDataArray<EventID_Cred>& eventAr - events and their handlers documented by the control
* Returns: void
*/
void AnafaceUI::OnLButtonDown(UINT nFlags, CPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (tabs.get())
	{
		for (UINT c = 0; c < tabs->lChildren.Count(); c++)
		{
			if (!tabs->lChildren.ElementAt(c).get())
				continue;
			TrecPointer<TControl> tcon = tabs->lChildren.ElementAt(c)->contain;
			if (!tcon.get())
				continue;
			
			messageOutput tempOut = negative;
			tcon->OnLButtonDown(nFlags, point, &tempOut, eventAr);
			if (tempOut != negative && tempOut != negativeUpdate)
			{
				if (children.Count() > c && children.ElementAt(c).get())
				{
					currentControl = children.ElementAt(c);
					*mOut = messageOutput::positiveOverride;
					return;
				}
			}
		}
	}

	if (currentControl.get())
		currentControl->OnLButtonDown(nFlags, point, mOut, eventAr);
}


/*
* Method: AnafaceUI - onCreate
* Purpose: Sets up the switching control with AnafaceUI specific attributes
* Parameters: RECT container - the location the control will be stored
* Returns: bool
*/
bool AnafaceUI::onCreate(RECT container)
{
	TControl::onCreate(container);
	RECT r;
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|TabHeight"));
	if (valpoint.get())
	{
		tabs = new TLayoutEx(renderTarget, styles);
		tabs->setLayout(HStack);
		if (valpoint->ConvertToInt(&tabHeight))
			tabHeight = 30;

		 r = CRect(location.left, location.top, location.right, location.top + tabHeight);
		for (UINT C = 0; C < children.Count(); C++)
		{
			TControl* tcon = children.ElementAt(C).get();
			if (!tcon)
				continue;
			
			if (!tcon)
				continue;
			tabs->addColunm(60, true);
			TrecPointer<TControl> newTab = new TControl(renderTarget, styles);
			tabs->addChild(newTab, tabs->getColumnNumber() - 1, 0);
		}

		if (styles.get())
		{
			valpoint = attributes.retrieveEntry(TString(L"|TabStyle"));
			if (valpoint.get())
			{
				styleTable* st;
				for (UINT c = 0; c < styles->Count(); c++)
				{
					if (!styles->ElementAt(c).get())
						continue;
					st = styles->ElementAt(c).get();
					if (st->style == valpoint.get())
					{
						for (UINT C = 0; C < children.Count(); C++)
						{
							TrecPointer<TControl> newTab = tabs->GetLayoutChild(C,0);
							if(!newTab.get())
								continue;
							for (UINT rust = 0; rust < st->names.count(); rust++)
							{
								tEntry<TString>* ent = st->names.GetEntryAt(rust).get();
								if (!ent)
									continue;
								newTab->addAttribute(ent->key, ent->object);
							}
						}
					}
				}
			} // End of Valpoint if for "TabStyle"

		} // End of Tab Style
		if (children.Count() && children.ElementAt(0).get())
			currentControl = children.ElementAt(0);

	}// End of valpoint if statement for "TabHeight" and code for creating tabs

	container.top += tabHeight;

	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc = children.ElementAt(c).get();
		if (tc)
			tc->onCreate(container);
	}

	if (tabs.get())
	{
		tabs->onCreate(r);

		for (UINT c = 0; c < children.Count(); c++)
		{
			TControl* tcon = children.ElementAt(c).get();
			if (!tcon)
				continue;
			
			TrecPointer<TControl> tc2 = tabs->GetLayoutChild(c, 0);
			if (!tc2.get())
				continue;
			TString id = tcon->GetID();
			TrecPointer<TText> text = tc2->getText(1);
			if (!text.get())
				continue;
			text->setCaption(id);
		}

		
	}

	valpoint = attributes.retrieveEntry(TString(L"|BeginningChildIndex"));
	int ind = -1;
	if (valpoint.get() && !valpoint->ConvertToInt(&ind))
	{
		if (ind > -1 && ind < children.Count())
			currentControl = children.ElementAt(ind);
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
	if (control.get())
	{
		for (UINT c = 0; c < children.Count(); c++)
		{
			if (control.get() == children.ElementAt(c).get())
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
int AnafaceUI::addControl(CArchive * arch)
{
	if (!arch)
		return -2;

	CFile* file = arch->GetFile();
	if (!file)
		return -3;
	TString fileName = file->GetFileName();

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
	if (control.get())
	{
		//TrecPointer<TContainer> tc(new TContainer());
		//tc->setTControl(control);
		control->onCreate(location);
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
		if (content3.get())
			content3->onDraw();
		else if (content1.get())
			content1->onDraw();
		if (border3.get())
			border3->onDraw();
		else if (border1.get())
			border1->onDraw();
		if (text3.get())
			text3->onDraw(snip, obj);
		else if (text1.get())
			text1->onDraw(snip, obj);
	}
	else if (mState == mouseHover)
	{
		if (content2.get())
			content2->onDraw();
		else if (content1.get())
			content1->onDraw();
		if (border2.get())
			border2->onDraw();
		else if (border1.get())
			border1->onDraw();
		if (text2.get())
			text2->onDraw(snip, obj);
		else if (text1.get())
			text1->onDraw(snip, obj);
	}
	else
	{
		if (content1.get())
			content1->onDraw();
		if (border1.get())
			border1->onDraw();
		if (text1.get())
			text1->onDraw(snip, obj);
	}

	if (vScroll)
		vScroll->updateDraw();
	if (hScroll)
		hScroll->updateDraw();

	if (tabs.get())
		tabs->onDraw(obj);
	if (currentControl.get())
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
	if (tabs.get())
	{
		TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|TabStyle"));
		TrecPointer<TControl> tc(new TControl(renderTarget, styles));
		if (valpoint.get())
		{
			styleTable* st;
			for (UINT c = 0; c < styles->Count(); c++)
			{
				if (!styles->ElementAt(c).get())
					continue;
				st = styles->ElementAt(c).get();

				if (st->style == valpoint.get())
				{

					for (UINT rust = 0; rust < st->names.count(); rust++)
					{

						tEntry<TString>* ent = st->names.GetEntryAt(rust).get();
						if (!ent)
							continue;
						tc->addAttribute(ent->key, ent->object);

					}

				}
			}

		}
		if (t == TString(L""))
		{
			t.Format(L"Unknown (%d)", unknownTab++);
		}

		tc->addAttribute(TString(L"|Caption"), TrecPointer<TString>(new TString(t)));
		UINT indexAdd = tabs->AddCol(40);
		if (indexAdd)
		{
			tabs->addChild(tc, indexAdd - 1, 0);
			RECT tabLoc = tabs->getRawSectionLocation(0, indexAdd - 1);
			tc->onCreate(tabLoc);
		}
	}
}

/*
* Method: AnafaceUI - GetControlArea
* Purpose: Retrieves the location of the main body control (tabs might reduce the actual size)
* Parameters: void
* Returns: RECT - the location of the main body area
*/
RECT AnafaceUI::GetControlArea()
{
	RECT returnable = location;
	returnable.top += tabHeight;
	return returnable;
}

void AnafaceUI::Resize(RECT r)
{
	location = r;
	r.top += tabHeight;

	for (UINT Rust = 0; Rust < children.Count(); Rust++)
	{
		if (children.ElementAt(Rust).get())
			children.ElementAt(Rust)->Resize(r);
	}

	if (tabs.get())
	{
		RECT tabLoc = location;
		tabLoc.bottom = tabLoc.top + tabHeight;
		tabs->Resize(tabLoc);
	}
	updateComponentLocation();
}
