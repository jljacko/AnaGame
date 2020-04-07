
#include "TFlyout.h"

TDataArray<TFlyout*> flyouts;


/*
* Method: (TFlyout) (Constructor) 
* Purpose: Sets up the basic features of the TFlyout
* Parameters: TrecPointer<DrawingBoard>cd, 
*			TrecPointer<TArray<styleTable>> ta - the list of styles to draw from
* Returns: void
*/
TFlyout::TFlyout(TrecPointer<DrawingBoard>cd, TrecPointer<TArray<styleTable>> ta):TLayout(cd,ta)
{
	appearWhen = appearCondition::appear_unset;
	isShown = false;
	arrayLoc = flyouts.push_back(this);
}

/*
* Method: (TFlyout) (Destructor)
* Purpose: Cleans up the TFlyout array list
* Parameters: void
* Returns: void
*/
TFlyout::~TFlyout()
{
	flyouts.RemoveAt(arrayLoc);
	for (int c = arrayLoc; c < flyouts.Size(); c++)
	{
		flyouts.at(c)->arrayLoc--;
	}
}

/*
* Method: TFlyout - onCreate
* Purpose: Sets up the attributes of the TFlyout
* Parameters: RECT r - the location of the Control
* Returns: bool - false
*/
bool TFlyout::onCreate(D2D1_RECT_F r, TrecPointer<TWindowEngine> d3d)
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Type"));

	if (!valpoint.Get() || valpoint->Compare(L"TCanvas"))
	{
		organization = orgLayout::tCanvas;
	}
	else
	{
		if (valpoint->Compare(L"TGrid"))
			organization = orgLayout::grid;
		else if (valpoint->Compare(L"TStack"))
			organization = orgLayout::VMix;
		else if (valpoint->Compare(L"TGallary"))
			organization = orgLayout::HMix;
		/*else if(valpoint->Compare(L""))

		else if(valpoint->Compare(L""))

		else if(valpoint->Compare(L""))*/
	}

	if (appearWhen == appearCondition::appear_unset) // this should change in the context menu
	{
		valpoint = attributes.retrieveEntry(TString(L"|OnAppear"));

		if (!valpoint.Get())
		{
			appearWhen = appearCondition::appear_onClick;

		}
		else
		{
			if (valpoint->Compare(L"Click"))
				appearWhen = appearCondition::appear_onClick;
			else if (valpoint->Compare(L"Hover"))
				appearWhen = appearCondition::appear_onHover;
			else if (valpoint->Compare(L"Click_Hover"))
				appearWhen = appearCondition::appear_onClickOrHover;
			else if (valpoint->Compare(L"R"))
				appearWhen = appearCondition::appear_onRightClick;
			else if (valpoint->Compare(L"Click_R"))
				appearWhen = appearCondition::appear_onEitherClick;
			else if (valpoint->Compare(L"Hover_R"))
				appearWhen = appearCondition::appear_onRClickOrHover;
			else if (valpoint->Compare(L"Click_Hover_R"))
				appearWhen = appearCondition::appear_onEitherClickOrHover;
			else if (valpoint->Compare(L"SriptOnly"))
				appearWhen = appearCondition::appear_onScript;
		}
	}

	int exitSize = 20;
	if (exitSize > r.bottom - r.top)
		exitSize = r.bottom - r.top;

	exitRect.left = r.left;
	exitRect.right = r.right;
	exitRect.top = r.top;
	exitRect.bottom = exitRect.top + exitSize;

	r.top = exitRect.bottom;

	if (organization == orgLayout::tCanvas)
		TControl::onCreate(r,d3d);
	else
		TLayout::onCreate(r,d3d);

	return false;
}

/*
* Method: TFlyout - onDraw
* Purpose: The call to draw the flyout, if shown
* Parameters: void
* Returns: void
*/
void TFlyout::onDraw(TObject* obj)
{
	if (!isShown)
		return;
	else
	{
		TrecPointer<TBrush> redBrush; 
		D2D1::ColorF redColor = D2D1::ColorF(D2D1::ColorF::Enum::Red);
		if (mState != messageState::mouseHover || mState == messageState::mouseLClick ||mState == messageState::mouseRClick)
			redColor.a = 0.5;

		redBrush = drawingBoard->GetBrush(redColor);

		redBrush->FillRectangle(exitRect);

		redColor.b = 1.0;
		redColor.g = 1.0;
		redColor.r = 1.0;
		redBrush->SetColor(redColor);
		redBrush->DrawLine(D2D1::Point2F(exitRect.left, exitRect.top), D2D1::Point2F(exitRect.right, exitRect.bottom), 2.0f);

		redBrush->DrawLine(D2D1::Point2F(exitRect.right, exitRect.top), D2D1::Point2F(exitRect.left, exitRect.bottom), 2.0f);

		if (organization == orgLayout::tCanvas)
			TControl::onDraw(obj);
		else
			TLayout::onDraw(obj);
	}
}

/*
* Method: TFlyout - Show
* Purpose: Sets a TFlyout to show itself (if the purpose is correct)
* Parameters: appearCondition ac - the condition on which the flyout is supposed to appear
* Returns: bool - whether the control will be shown
*/
bool TFlyout::Show(appearCondition ac)
{
	if (ac == appearCondition::appear_onScript)
		isShown = true;
	else
	{
		switch (ac)
		{
		case appearCondition::appear_onClick:
			if (static_cast<UINT>(appearWhen) == 2 || static_cast<UINT>(appearWhen) == 4 || static_cast<UINT>(appearWhen) == 6 || static_cast<UINT>(appearWhen) == 0)
				isShown = false;
			else
				isShown = true;
			break;
		case appearCondition::appear_onHover:
			if (static_cast<UINT>(appearWhen) == 1 || static_cast<UINT>(appearWhen) == 4 || static_cast<UINT>(appearWhen) == 5 || !static_cast<UINT>(appearWhen))
				isShown = false;
			else
				isShown = true;
			break;
		case appearCondition::appear_onRightClick:
			if (static_cast<UINT>(appearWhen) == 1 || static_cast<UINT>(appearWhen) == 2 || static_cast<UINT>(appearWhen) == 3 || !static_cast<UINT>(appearWhen))
				isShown = false;
			else
				isShown = true;
			break;
		}
	}

	return isShown;
}

/*
* Method: TFlyout - Hide
* Purpose: Sets the TFlyout to not show (or draw) itself
* Parameters: void
* Returns: void
*/
void TFlyout::Hide()
{
	isShown = false;
}

/*
* Method: TFlyout - OnLButtonDown
* Purpose: Responds to mouse clicks with flyout specific functionality
* Parameters: UINT nFlags - Details provided by Windows 
*				CPoint point - The location of the click 
*				messageOutput * mOut - The results of the Event (was a control updated?) 
*				TDataArray<EventID_Cred>& eventAr - the List of Events in a User's actions
* Returns: void
*/
void TFlyout::OnLButtonDown(UINT nFlags, TPoint point, messageOutput * mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedControl)
{
	if (!isShown)
	{
		*mOut = messageOutput::negative;return;
	}

	if (!isContained(&point, &location))
	{
		isShown = false;
		*mOut = messageOutput::negativeUpdate;return;
	}

	TControl::OnLButtonDown(nFlags, point, mOut, eventAr, clickedControl);
}

/*
* Method: TFlyout - DoDraw
* Purpose: Retrieves the active TFlyout
* Parameters: void
* Returns: TFlyout* - the flyout to draw (or null) 
*/
TFlyout * TFlyout::DoDraw()
{
	for (int c = 0; c < flyouts.Size(); c++)
	{
		if (flyouts[c])
		{
			if (flyouts[c]->isShown)
			{
				//flyouts[c]->onDraw();
				return flyouts[c];
			}
		}
	}
	return nullptr;
}

/*
* Method: TFlyout - GetAnaGameType
* Purpose: Retrieves the AnaGame type token
* Parameters: void
* Returns: UCHAR* the type representation of the TObject
*/
UCHAR * TFlyout::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: TFlyout - getFlyoutList
* Purpose: Retrieves the pointer to the list of currently active flyouts
* Parameters: void
* Returns: TDataArray<TFlyout*>* - the list of TFlyouts
*/
TDataArray<TFlyout*>* getFlyoutList()
{
	return &flyouts;
}
