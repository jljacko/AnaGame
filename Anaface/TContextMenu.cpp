#include "stdafx.h"
#include "TContextMenu.h"

/*
* Method: (TContextMenu) (Constructor) 
* Purpose: Sets up the context menu with drawing resources
* Parameters:  TrecComPointer<ID2D1RenderTarget> rt - the render target to draw to
*				TrecPointer<TArray<styleTable>> ta - the style list for Anaface
* Returns: void
*/
TContextMenu::TContextMenu(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta):TFlyout(rt,ta)
{
	appearWhen = appear_onRightClick;
	organization = VStack;
}

/*
* Method: (TContextMenu) (Destructor)
* Purpose: Cleans up Context Menu
* Parameters: void
* Returns: void
*/
TContextMenu::~TContextMenu()
{
}

/*
* Method: TContextMenu - onCreate
* Purpose: Ses up the Context Menu with items
* Parameters: RECT l - the location of the menu
* Returns: bool - false
*/
bool TContextMenu::onCreate(RECT l)
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|SubHeight"));
	if (valpoint.Get() && !valpoint->ConvertToInt(&childHeight))
	{

	}
	else
		childHeight = 30;

	location.bottom = childHeight;
	location.left = 0;
	location.right = 200;
	location.top = 0;

	int occ = 0;
	valpoint = attributes.retrieveEntry(TString(L"|ContextMenuItem"),occ++);
	
	while (valpoint.Get())
	{
		addOption(valpoint);
		valpoint = attributes.retrieveEntry(TString(L"|ContextMenuItem"), occ++);
	}

	TLayout::onCreate(location);

	return false;
}

/*
* Method: TContextMenu - addOption
* Purpose: Adds an option to the Context Menu
* Parameters: TrecPointer<TString> c - the option to add
* Returns: bool - false
*/
bool TContextMenu::addOption(TrecPointer<TString> c)
{
	//TControlPointer* tcp = getNewTControl();
	TrecPointer<TControl> tc = TrecPointerKey::GetNewTrecPointer<TControl>(renderTarget, styles);

	tc->addAttribute(TString(L"|BorderColor"), TrecPointerKey::GetNewTrecPointer<TString>(L"1.0,1.0,1.0,1.0"));
	tc->addAttribute(TString(L"|ContentColor"), TrecPointerKey::GetNewTrecPointer<TString>(L"0.0,0.4,1.0,0.7"));
	tc->addAttribute(TString(L"|HoverContentColor"), TrecPointerKey::GetNewTrecPointer<TString>(L"0.4,0.4,1.0,0.7"));
	tc->addAttribute(TString(L"|Caption"), c);
	tc->addAttribute(TString(L"|FontColor"), TrecPointerKey::GetNewTrecPointer<TString>(L"0.0,0.0,0.0,1.0"));
	tc->addAttribute(TString(L"|FontSize"), TrecPointerKey::GetNewTrecPointer<TString>(L"18"));
	tc->addAttribute(TString(L"|Font"), TrecPointerKey::GetNewTrecPointer<TString>(L"TimesNewRoman"));
	//TrecPointer<TContainer> tco = new TContainer();
	//tco->setTControl(tc);
	tc->onCreate(RECT{ location.left,0,location.right,childHeight });
	children.Add(tc);
	location.bottom = children.Count()*childHeight;

	return false;
}

/*
* Method: TContextMenu - storeInTML
* Purpose: Enables theContext menu to save itse;f to a TML file
* Parameters: CArchive* ar - the file to write to
*				int childLevel - the level in the tree the mnu is in (how many dashes to add)
*				bool ov - 
* Returns: void
*/
void TContextMenu::storeInTML(TFile * ar, int childLevel,bool ov)
{
//	_Unreferenced_parameter_(ov);


	TString appendable;
	resetAttributeString(&appendable, childLevel+1);
	appendable.Append(L"|SubHeight:");
	appendable.AppendFormat(_T("%d"), childHeight);
	_WRITE_THE_STRING;


	TControl* tc = NULL;
	for (int c = 0; c < children.Count();c++)
	{
		appendable.Append(L"|ContextMenuItem:");
		if (children.ElementAt(c).Get() && children.ElementAt(c).Get())

		tc = children.ElementAt(c).Get();

		if (tc && tc->getText(1).Get() && tc->getText(1)->text.GetSize())
		{
			appendable.Append(tc->getText(1)->text);
		}
		else
			appendable.Append(L"NULL");
		_WRITE_THE_STRING;
	}

	TControl::storeInTML(ar, childLevel, true);
	
}

/*
* Method: TContextMenu - onDraw
* Purpose: Draws the Context Menu at a specific point
* Parameters: CPoint cp - the location to draw the context menu
* Returns: void
*/
void TContextMenu::onDraw(CPoint cp)
{
	this->offsetLocation(cp);

	for (int c = 0; c < children.Count(); c++)
	{

		if (children.ElementAt(c).Get())
		{
			children.ElementAt(c)->offsetLocation(cp);
			cp.y += childHeight;
		}
	}

	TLayout::onDraw();
}

/*
* Method: TContextMenu - GetAnaGameType
* Purpose: Retrieves the AnaGame type token for the TObject to report as a Context Menu
* Parameters: void
* Returns: UCHAR* - the AnaGame representation of the TContextMenu class 
*/
UCHAR * TContextMenu::GetAnaGameType()
{
	return nullptr;
}
