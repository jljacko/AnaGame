
#include "TComboBox.h"

TDataArray<TComboBox*> boxList;

/*
* Function: DrawActiveComboBox
* Purpose: Draws Active Combo-box
* Parameters: ID2D1RenderTarget * rt - the render target to use
* Returns: void
*/
void  DrawActiveComboBox(ID2D1RenderTarget * rt)
{
	for (int c = 0; c < boxList.Size(); c++)
	{
		if (!boxList[c])
			continue;
		if (boxList[c]->GetExtensionStatus())
		{
			boxList[c]->onDraw(rt);
			break;
		}
	}
}

/*
* Function: GetActiveComboBox
* Purpose: retrieves the currently acting combo-box, if one is active
* Parameters: void
* Returns: TComboBox* - currently active combo-box (or null)
*/
TComboBox * GetActiveComboBox()
{
	for (int c = 0; c < boxList.Size(); c++)
	{
		if (boxList[c] && boxList[c]->GetExtensionStatus())
			return boxList[c];
	}
	return nullptr;
}

/*
* Function: ResetComboBoxes
* Purpose:  Makes sure all but the current ComboBox closes itself
* Parameters: void
* Returns: void
*/
void ResetComboBoxes()
{
	for (int c = 0; c < boxList.Size(); c++)
	{
		if (boxList[c])
			boxList[c]->FinalizeUpdate();
	}
}

/*
* Method: (TComboBox) (Constructor)
* Purpose: Set up the ComboBox with default values
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - The Render Target to draw to
*				TrecPointer<TArray<styleTable>> ta - The Style Table to draw from
* Returns: void
*/
TComboBox::TComboBox(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta):TGadgetControl(rt,ta)
{
	childHeight = 40;
	showExtended = prepShowExtended = false;
	reacted = false;
	boxList.push_back(this);
	boxLoc = boxList.Size() - 1;
	extendedSpace = D2D1_RECT_F{ 0,0,0,0 };
	vertexPoint = D2D1::Point2F();
	//defaultText = NULL;
}

/*
* Method: (TComboBox) (Destructor)
* Purpose: Cleans up TComboBox resources
* Parameters: void
* Returns: void
* NOTE: Code currently commented out to avoid memory corruption
*/
TComboBox::~TComboBox()
{
	//TGadgetControl::~TGadgetControl();
//	boxList.RemoveAt(boxLoc);
	for (int c = boxLoc;c < boxList.Size();c++)
	{
		if (boxList[c] == this)
			boxList.setAt(nullptr, c);
		//boxList[c]->decrimentLocation();
	}

}

/*
* Method: TComboBox - onCreate
* Purpose: Sets up The TControl attributes and the Combo-box specific attributes
* Parameters: RECT r - the Location the Basic Box would be in
* Returns: bool
*/
bool TComboBox::onCreate(RECT r)
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|SubHeight"));
	if (valpoint.Get() && !valpoint->ConvertToInt(&childHeight))
	{

	}
	else
		childHeight = 30;
	valpoint = attributes.retrieveEntry(TString(L"|DefaultText"));
	if (valpoint.Get())
	{
		if (!text1.Get())
		{
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget,this);
		}
		text1->removeCaption();
		defaultText = valpoint.Get();

		text1->setCaption(defaultText);

	}
	int occ = 0;
	valpoint = attributes.retrieveEntry(TString(L"|BoxEntry"),occ++);

	while (valpoint.Get())
	{
		elements.Add(valpoint);
		valpoint = attributes.retrieveEntry(TString(L"|BoxEntry"), occ++);
	}
	
	// To-Do later when features are being added:
	// add mechanism to control the style of the sub ojbects

	//TrecPointer<TContainer> cont;
	TrecPointer<TControl> tc;

	TGadgetControl::onCreate(r);

	leftpoint = D2D1::Point2F(DxLocation.left, DxLocation.top);
	rightPoint = D2D1::Point2F(DxLocation.right, DxLocation.top);

	float midDx = DxLocation.left + ((DxLocation.right - DxLocation.left) / 2);

	vertexPoint = D2D1::Point2F(midDx, DxLocation.bottom);


	RECT entrybox = RECT{ 0,0,0,0 };
	for (int c = 0; c < elements.Count(); c++)
	{
		tc = TrecPointerKey::GetNewSelfTrecPointer<TControl>(renderTarget, styles);
		//cont = new TContainer();
		//cont->setTControl(tc);


		tc->setNewText(1);
		TString CapParam = elements.ElementAt(c).Get();


		tc->text1->setCaption(CapParam); 

		entrybox.left = location.left;
		entrybox.right = location.right;
		entrybox.top = location.bottom + c * childHeight;
		entrybox.bottom = location.bottom + (c + 1)*childHeight;
		children.Add(tc);
		//cont->setLocation(entrybox);
		tc->onCreate(entrybox);
	}



	// now set up the extended background
	extendedSpace = D2D1::RectF(location.left, location.bottom,
		location.right, location.bottom + (childHeight) * (children.Count()));
	D2D1_COLOR_F whiteColor = D2D1::ColorF(D2D1::ColorF::White, 0.9f);

	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder extendedBrushRaw;
	HRESULT brushRes = renderTarget->CreateSolidColorBrush(whiteColor, extendedBrushRaw.GetPointerAddress());
	extendedBrush = extendedBrushRaw.Extract();
	return SUCCEEDED(brushRes);

	//return false;
}

/*
* Method: TComboBox - onDraw
* Purpose: Draws the ComboBox
* Parameters: void
* Returns: void
* Note: This method is almost identical to the TControl version except that children
*	are only drawn when set to
*/
void TComboBox::onDraw(TObject* obj)
{
	if (!isActive)
		return;
	if (!renderTarget.Get())
		return;

	// Although this code is seen in the TControl implementation of onDraw, 
	// that method also calls on Draw on it's children. we don't want to do that
	// here unless showExtended is true.
	if (mState == mouseLClick)
	{
		if (content3.Get())
			content3.Get()->onDraw(location);
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

	renderTarget->DrawLine(leftpoint, vertexPoint, brush.Get());
	renderTarget->DrawLine(vertexPoint, rightPoint, brush.Get());

if (showExtended)
{
	renderTarget->FillRectangle(extendedSpace, extendedBrush.Get());

	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->onDraw(obj);
	}


}

}

/*
* Method: TComboBox - onDraw
* Purpose: ?
* Parameter: ID2D1RenderTarget * rt - render target to use
* Returns: void
*/
void TComboBox::onDraw(ID2D1RenderTarget * rt)
{
	if (rt == renderTarget.Get())
		onDraw();
}

/*
* Method: TComboBox - addElement
* Purpose: Adds an element to the Drop-Down Menu
* Parameters: TString
* Returns: void
* Note: Method currently has no functionality and should be considered DEPRECIATED until further notice
*/
void TComboBox::addElement(TString)
{

}

/*
* Method: TComboBox - removeElement
* Purpose: Removes an Element from the drop down menu
* Parameters: TString
* Returns: bool
* Note: Method currently has no functionality and should be considered DEPRECIATED until further notice
*/
bool TComboBox::removeElement(TString)
{
	return false;
}

/*
* Method: TComboBox - storeInTML
* Purpose: Stores the Combo-box specific attributes in a TML file
* Parameters:  CArchive* ar - the file to save to
*				int childLevel - the generation if the TControl
*				bool overrideChildren - whether to ignore the children to save
* Returns: void
*/
void TComboBox::storeInTML(TFile* ar, int childLevel, bool ov)
{
	//	_Unreferenced_parameter_(ov);


	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);
	appendable.Append(L"|SubHeight:");
	appendable.AppendFormat("%d", childHeight);
	_WRITE_THE_STRING;


	if (defaultText.GetSize())
	{
		appendable.Append(L"|DefaultText:");
		appendable.Append(defaultText);
		_WRITE_THE_STRING;
	}

	TControl* tc = nullptr;
	for (int c = 0; c < children.Count(); c++)
	{
		appendable.Append(L"|BoxEntry:");
		if (children.ElementAt(c).Get())

		tc = children.ElementAt(c).Get();

		if (tc && tc->getText(1).Get() && tc->text1->getCaption().GetSize())
		{
			appendable.Append(tc->text1->getCaption());
		}
		else
			appendable.Append(L"NULL");
		_WRITE_THE_STRING;
	}

	TGadgetControl::storeInTML(ar, childLevel, true);



}

/*
* Method: TComboBox - GetExtensionStatus
* Purpose: Reports whether the drop-down menu is showing
* Parameters: void
* Returns: bool - whether the Combo-Box is active
*/
bool TComboBox::GetExtensionStatus()
{
	return showExtended;
}

void TComboBox::SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>rt)
{
	TControl::SetNewRenderTarget(rt);
	D2D1_COLOR_F whiteColor = D2D1::ColorF(D2D1::ColorF::White, 0.9f);
	TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder extendedBrushRaw;
	HRESULT brushRes = renderTarget->CreateSolidColorBrush(whiteColor, extendedBrushRaw.GetPointerAddress());
	extendedBrush = extendedBrushRaw.Extract();
}

/*
* Method: TComboBox - OnLButtonDown
* Purpose: Determins if the Combo-Box was clicked and if so, which element
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				CPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
void TComboBox::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	resetArgs();

	// first check to see if it covers the whole area. Update show extended to false if
	// necessary
	RECT MFC_extended_Space;
	MFC_extended_Space.bottom = extendedSpace.bottom;
	MFC_extended_Space.left = extendedSpace.left;
	MFC_extended_Space.right = extendedSpace.right;
	MFC_extended_Space.top = extendedSpace.top;
	if (!isContained(&point, &location) && !isContained(&point, &MFC_extended_Space))
	{
		showExtended =prepShowExtended = false;
		TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
		return;
	}
//	*mOut = positiveOverrideUpdate;
	if (isContained(&point, &location))
	{
		//reacted = true;
		// To-Do:: When features are added
		// enable Text Support



		// end new Text edit feature
		bool extensionClear = true;
		for (int c = 0; c < boxList.Size(); c++)
		{
			if(boxList[c])
				if (boxList[c]->showExtended)
				{
					extensionClear = false;
					break;
				}
		}

		if(extensionClear)
			prepShowExtended = !prepShowExtended;
		TControl::OnLButtonDown(nFlags, point, mOut,eventAr, clickedControl);
		
	}

	if (showExtended && isContained(&point, &MFC_extended_Space))
	{
		*mOut = negativeUpdate;
		TrecPointer<TControl> tc;
		for (int c = 0; c < children.Count(); c++)
		{
			if (children.ElementAt(c).Get())
				tc = children.ElementAt(c);
			if (tc.Get())
			{
				tc->OnLButtonDown(nFlags, point, mOut,eventAr, clickedControl);
				if (*mOut == negative || *mOut == negativeUpdate)
					continue;

				if (text1.Get())
				{
					text1->removeCaption();

					TString tempText = tc->text1->getCaption();


					text1->setCaption(tempText);
					text1->reCreateLayout();
				}
				prepShowExtended = false;
				*mOut = positiveOverrideUpdate;

				// Set args
				resetArgs();
				args.eventType = On_sel_change;
				args.point = point;
				args.methodID = getEventID(On_sel_change);
				args.isClick = false;
				args.isLeftClick = false;
				args.control = this;
				if (text1.Get() && text1->getCaption().GetSize())
					args.text.Set(text1->getCaption());

				eventAr.push_back({On_sel_change,this});



				return;
			}
		}
	}
}

/*
* Method: TComboBox - decrimentLocation
* Purpose: Moves itself lower in the list of comboboxes to make room for new Combo-Boxes
* Parameters: void
* Returns: void
* Note: Method currently not called
*/
void TComboBox::decrimentLocation()
{
	boxLoc--;
}

/*
* Method: TComboBox - FinalizeUpdate
* Purpose: Makes sure the Combo_box knows whtehr to show the menu or not
* Parameters: void
* Returns: void
*/
void TComboBox::FinalizeUpdate()
{
	showExtended = prepShowExtended;
}

/*
* Method: TComboBox - GetAnaGameType
* Purpose: Retrieves the AnaGame type for Combo-boxes
* Parameters: void
* Returns: UCHAR* - pointer to AnaGame type
*/
UCHAR * TComboBox::GetAnaGameType()
{
	return nullptr;
}

void TComboBox::Resize(RECT r)
{
	TGadgetControl::Resize(r);
	leftpoint = D2D1::Point2F(DxLocation.left, DxLocation.top);
	rightPoint = D2D1::Point2F(DxLocation.right, DxLocation.top);

	float midDx = DxLocation.left + ((DxLocation.right - DxLocation.left) / 2);

	vertexPoint = D2D1::Point2F(midDx, DxLocation.bottom);

	RECT entrybox = RECT{ 0,0,0,0 };
	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc = children.ElementAt(c).Get();
		if (!tc)
			continue;


		entrybox.left = location.left;
		entrybox.right = location.right;
		entrybox.top = location.bottom + c * childHeight;
		entrybox.bottom = location.bottom + (c + 1)*childHeight;
		
		//cont->setLocation(entrybox);
		tc->Resize(entrybox);
	}



	// now set up the extended background
	extendedSpace = D2D1::RectF(location.left, location.bottom,
		location.right, location.bottom + (childHeight) * (children.Count()));
}
