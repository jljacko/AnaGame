#include "stdafx.h"
#include "TGadgetControl.h"

/*
* Method: TGadgetControl) (Constructor) 
* Purpose: Sets up a TGadet Control
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - the Render Target to use
*				TrecPointer<TArray<styleTable>> ta - List of Styles for the TControl
*				bool isGadgetBase - the gadget base
* Return: void
*/
TGadgetControl::TGadgetControl(TrecComPointer<ID2D1RenderTarget> rt, TrecPointer<TArray<styleTable>> ta, bool isGadgetBase):TControl(rt, ta, false)
{
	isGadBase = isGadgetBase;
	if (isGadBase)
		isTextControl = false;
	bSize = 30;
	checker = RECT{ 0,0,0,0 };
	brush = nullptr;
	thickness = 1.0;
}

/*
* Method: (TGadgetControl) (Destructor)
* Purpose: Cleans up Gadget Control
* Parameters: void
* Return: void
*/
TGadgetControl::~TGadgetControl()
{
	//TControl::~TControl();
	brush.Delete();
	
}

/*
* Method: TGadgetControl - storeInTML
* Purpose: Saves the Gadget Control to a TML file
* Parameters: CArchive * ar - the File to use
*				int childLevel - the level control is in the tree
*				bool ov - UNUSED
* Return: void
*/
void TGadgetControl::storeInTML(CArchive * ar, int childLevel, bool ov)
{
	//_Unreferenced_parameter_(ov);

	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);

	appendable.Append(L"|BoxSize:");
	appendable.AppendFormat(_T("%d"), bSize);
	_WRITE_THE_STRING;

	TControl::storeInTML(ar, childLevel,ov);

}

/*
* Method: TGadgetControl - onCreate
* Purpose: To set up the Gadget Control
* Parameters: RECT r - the location control will be
* Return: bool - false, ignore for now
*/
bool TGadgetControl::onCreate(RECT r)
{
	marginPriority = false;

	TControl::onCreate(r);

	bSize = location.bottom-location.top;
	int height = bSize;

	if (bSize > location.right - location.left)
		bSize = location.right - location.left;

	if (bSize > 30)
		bSize = 30;

	if (!content1.get())
	{
		content1 = new TContent(renderTarget, this);
		content1->color = D2D1::ColorF(D2D1::ColorF::White);
		content1->onCreate(location,snip);                         // this this isn't covered by the TControl
															// as it didn't exist yet
	}


	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|BoxSize"));
	if(valpoint.get())
	{
		int tSize = bSize;
		if (!valpoint->ConvertToInt(&bSize))
		{
			bSize = tSize;
			
		}


	}
	int offset = (height - bSize) / 2;
	checker.top = location.top + offset;
	checker.bottom = location.bottom - offset;
	checker.left = location.left;
	checker.right = checker.left + bSize;

	DxLocation.bottom = checker.bottom;
	DxLocation.left = checker.left;
	DxLocation.right = checker.right;
	DxLocation.top = checker.top;

	if (renderTarget.get())
	{
		ID2D1SolidColorBrush* brushRaw;
		renderTarget->CreateSolidColorBrush(color, &brushRaw);
		brush = brushRaw;
	}

	return false;
}

/*
* Method: TGadgetControl - GetAnaGameType
* Purpose: Returns the AnaGame format of the classes type
* Parameters: void
* Return: UCHAR* reference to the AnaGame type
*/
UCHAR * TGadgetControl::GetAnaGameType()
{
	return nullptr;
}

void TGadgetControl::Resize(RECT r)
{
	TControl::Resize(r);
	int height = location.bottom - location.top;
	int offset = (height - bSize) / 2;
	checker.top = location.top + offset;
	checker.bottom = location.bottom - offset;
	checker.left = location.left;
	checker.right = checker.left + bSize;

	DxLocation.bottom = checker.bottom;
	DxLocation.left = checker.left;
	DxLocation.right = checker.right;
	DxLocation.top = checker.top;
}
