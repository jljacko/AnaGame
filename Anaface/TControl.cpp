//#include "stdafx.h"
#include "AnafaceUI.h"
#include <Logger.h>
#include <wincodec.h>

/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/

/*
* Method: (TControl) (Contructor)
* Purpose: Sets up a TControl with a RenderTarget to draw to and a style table to draw from
* Parameters: TrecComPointer<ID2D1RenderTarget> rt - Smart Pointer to the Render Target to draw on
*				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
*				bool base - (DEPRECIATED) added to distinguish between base control and sub-classes
* Contstructor - No Return
*/
TControl::TControl(TrecComPointer<ID2D1RenderTarget> rt,TrecPointer<TArray<styleTable>> styTab, bool base)
{
	arrayID = -1;
	eventHandler = NULL;
	isActive = true;
	treeLevel = 0;
	vScroll = hScroll = nullptr;
	renderTarget = rt;
	styles = styTab;
	location.bottom = 0;
	location.top = 0;
	location.left = 0;
	location.right = 0;
	
	boundsPreset = false;
	location = RECT{ 0,0,0,0 };
	mState = normal;
	overrideParent = true;
	marginPriority = true;
	dimensions = NULL;
	margin = snip = RECT{ 0,0,0,0 };
	marginSet = false;
	//PointerCase = TrecPointer<TControl>(this);
	
	if (base)
	{
		isLayout = false;
		isTextControl = false;
	}

	contextMenu = NULL;
	flyout = NULL;
	shape = T_Rect;
	fixHeight = fixWidth = false;
	rightBorder = leftBorder = topBorder = bottomBorder = onFocus = onClickFocus = false;

	//eventList = new TDataArray<EventTypeID>();
	contextMenu = nullptr;
	

	TString logMessage;
	logMessage.Format(L"CREATE %p TControl(TrecComPointer<ID2D1RenderTarget>,TrecPointer<TArray<styleTable>>, bool)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TControl) (Contructor)
* Purpose: Provide a Copy Contructor so that it can be moved easily from one medium to another
* Parameters: TControl &rCont - the Constructor to draw from
* Returns: void
*/
TControl::TControl(TControl & rCont)
{
	arrayID = rCont.arrayID;
	contextMenu = rCont.contextMenu;
	flyout = rCont.flyout;
	eventHandler = rCont.eventHandler;
	isActive = rCont.isActive;

	boundsPreset = rCont.boundsPreset;
	className = rCont.className;
	ID = rCont.ID;
	location = rCont.location;
	snip = rCont.snip;
	margin = rCont.margin;
	vScroll = rCont.vScroll;
	hScroll = rCont.hScroll;
	factory = rCont.factory;
	renderTarget = rCont.renderTarget;
	parent = rCont.parent;
	//PointerCase = TrecPointer<TControl>(PointerCase);
	treeLevel = rCont.treeLevel;

	// Quickly null the contents, borders, and text
	//text1 = text2 = text3 = NULL;
	//content1 = content2 = content3 = NULL;
	//border1 = border2 = border3 = NULL;

	if(rCont.border1.Get())
	border1 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border1),(this));
	if (rCont.border2.Get())
		border2 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border2), (this));
	if (rCont.border3.Get())
		border3 = TrecPointerKey::GetNewTrecPointer<TBorder>((rCont.border3), (this));
	if (rCont.text1.Get())
		text1 = TrecPointerKey::GetNewTrecPointer<TText>((rCont.text1), (this));
	if (rCont.text2.Get())
		text2 = TrecPointerKey::GetNewTrecPointer<TText>((rCont.text2), (this));
	if (rCont.text3.Get())
		text3 = TrecPointerKey::GetNewTrecPointer<TText>((rCont.text3), (this));
	if (rCont.content1.Get())
		content1 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content1), (this));
	if(rCont.content2.Get())
		content2 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content2), (this));
	if(rCont.content3.Get())
		content3 = TrecPointerKey::GetNewTrecPointer<TContent>((rCont.content3), (this));

	ellipse = rCont.ellipse;
	roundedRect = rCont.roundedRect;
	shape = rCont.shape;


	if (rCont.dimensions)
	{
		dimensions = new sizeControl;
		dimensions->height = rCont.dimensions->height;
		dimensions->maxHeight = rCont.dimensions->maxHeight;
		dimensions->maxWidth = rCont.dimensions->maxWidth;
		dimensions->minHeight = rCont.dimensions->minHeight;
		dimensions->minWidth = rCont.dimensions->minWidth;
		dimensions->width = rCont.dimensions->width;
	}
	else
		dimensions = NULL;

	isLayout = rCont.isLayout;
	isTextControl = rCont.isTextControl;


	marginPriority = rCont.marginPriority;
	marginSet = rCont.marginSet;
	fixHeight = rCont.fixHeight;
	fixWidth = rCont.fixWidth;

	attributes = rCont.attributes;
		
	styles = rCont.styles;
	children = rCont.children;
	holder = rCont.holder;

	// To-Do Later
	overrideParent; 
	messageState mState; // mechanism as to which version of the TControl should be drawn based off of user interaction
	TFlyout* flyout;     // Generic flyout, which can appear if attached
	TContextMenu* contextMenu; // specific flyout primed for dealing with right clicks, as is traditional

							   // and Builder Messages
	rightBorder= rCont.rightBorder; 
	leftBorder=rCont.leftBorder;
	topBorder=rCont.topBorder;
	bottomBorder=rCont.bottomBorder;
	onFocus=rCont.onFocus;
	onClickFocus=rCont.onClickFocus;

	eventList = rCont.eventList;

	TString logMessage;
	logMessage.Format(L"CREATE %p TControl(TControl&)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TControl) (Contructor)
* Purpose: Provide a Default contructor
* Parameters: void
* Returns: void
*/
TControl::TControl()
{
	arrayID = -1;

	contextMenu = nullptr;
	eventHandler = NULL;
	isActive = true;
	flyout = nullptr;

	vScroll = hScroll = nullptr;
	treeLevel = 0;

	location.bottom = 0;
	location.top = 0;
	location.left = 0;
	location.right = 0;
	//parent = NULL;
	boundsPreset = false;
	location = RECT{ 0,0,0,0 };
	mState = normal;
	overrideParent = true;
	marginPriority = true;
	dimensions = NULL;
	margin = snip = RECT{ 0,0,0,0 };
	marginSet = false;
	
	isLayout = false;
	isTextControl = false;

	flyout = nullptr;
	contextMenu = nullptr;
	//PointerCase =TrecPointer<TControl>(this);
	shape = T_Rect;
	fixHeight = fixWidth = false;
	rightBorder = leftBorder = topBorder = bottomBorder = onFocus = onClickFocus = false;

	//eventList = new TDataArray<EventTypeID>();
	TString logMessage;
	logMessage.Format(L"CREATE %p TControl()", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TControl) (Destructor)
* Purpose: Clean up Memory it consumes
* Parameters: void
* Returns: void
*/
TControl::~TControl()
{
	text1.Delete();
	text2.Delete();
	text3.Delete();
	border1.Delete();
	border2.Delete();
	border3.Delete();
	content1.Delete();
	content2.Delete();
	content3.Delete();

	for (int c = 0; c < children.Count(); c++)
	{
		TrecPointer<TControl> tc;
		tc = children.ElementAt(c);
		tc.Delete();
	}
	children.Clear();
	TString logMessage;
	logMessage.Format(L"DELETE %p TControl", this);

	Log(lt_memory, logMessage);
}

void TControl::SetSelf(TrecPointer<TControl> self)
{
	if (this != self.Get())
		throw L"Error! Function expected to recieve a protected reference to 'this' Object!";
	tThis = TrecPointerKey::GetSoftPointerFromTrec<TControl>(self);
}

extern TDataArray<TTextField*> TextList;

/*
* Method: TControl - loadFromHTML
* Purpose: Enables Control to load itself from an HTML File
* Parameters: CArchive* ar - archive to read the HTML file
* Returns: int - error
* Note: DEPRECIATED - Functionality should be handled by an HTML parser that's Anaface Compatible
*/
int TControl::loadFromHTML(TFile * ar)
{
	return 0;
}

/*
* Method: TControl - storeInTML
* Purpose: Allows a Control to save itself in a TML File
* Parameters: CArchive* ar - the file to save to
*				int childLevel - the generation if the TControl
*				bool overrideChildren - whether to ignore the children to save
* Returns: void
*/
void TControl::storeInTML(TFile * ar, int childLevel, bool overrideChildren)
{
	TString appendable;
	resetAttributeString(&appendable, childLevel + 1);

	const char* type = typeid(this).name();


	// Only write Object type if this object is a pure TControl
	if (!strcmp( type, "class TControl *"))
	{
		appendable.Append(L">TControl");
		_WRITE_THE_STRING
	}


	if (className.GetSize())
	{
		appendable.Append(L"|Class:");
		appendable.Append(className);
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}
	if (ID.GetSize())
	{
		appendable.Append(L"|id:");
		appendable.Append(ID);
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}
	appendable.Append(L"|VerticalScroll:"); 
	appendable.Append(boolToString(vScroll));
	ar->WriteString(appendable);
	ar->WriteString(L"\n");
	resetAttributeString(&appendable, childLevel + 1);
	appendable.Append(L"|HorizontalScroll:");
	appendable.Append(boolToString(hScroll));
	ar->WriteString(appendable);
	ar->WriteString(L"\n");
	resetAttributeString(&appendable, childLevel + 1);

	appendable.Append(L"|FixedHeight:");
	appendable.Append(boolToString(fixHeight));
	_WRITE_THE_STRING;

	appendable.Append(L"|FixedWidth:");
	appendable.Append(boolToString(fixWidth));
	_WRITE_THE_STRING;
	
	if (marginSet)
	{
		appendable.Append(L"|Margin:");
		appendable.Append(convertRectToString(margin));
		ar->WriteString(appendable);
		ar->WriteString(L"\n");
		resetAttributeString(&appendable, childLevel + 1);
	}


	if (dimensions)
	{
		if (dimensions->height)
		{
			appendable.Append(L"|Height:");
			appendable.AppendFormat(L"%d", dimensions->height);
			_WRITE_THE_STRING
		}
		if (dimensions->width)
		{
			appendable.Append(L"|Width:");
			appendable.AppendFormat(L"%d", dimensions->width);
			_WRITE_THE_STRING
		}
		if (dimensions->maxHeight)
		{
			appendable.Append(L"|MaxHeight:");
			appendable.AppendFormat(L"%d", dimensions->maxHeight);
			_WRITE_THE_STRING
		}
		if (dimensions->maxWidth)
		{
			appendable.Append(L"|MaxWidth:");
			appendable.AppendFormat(L"%d", dimensions->maxWidth);
			_WRITE_THE_STRING
		}
		if (dimensions->minHeight)
		{
			appendable.Append(L"|MinHeight:");
			appendable.AppendFormat(L"%d", dimensions->minHeight);
			_WRITE_THE_STRING
		}
		if (dimensions->minWidth)
		{
			appendable.Append(L"|MinWidth:");
			appendable.AppendFormat(L"%d", dimensions->minWidth);
			_WRITE_THE_STRING
		}
	}


	resetAttributeString(&appendable, childLevel + 1);

	if (shape == T_Ellipse)
	{
		appendable.Append(L"|Shape:Ellipse");
		_WRITE_THE_STRING;
	}
	else if (shape == T_Rounded_Rect)
	{
		appendable.Append(L"|Shape:RoundedRectangle");
		_WRITE_THE_STRING;
		appendable.Append(L"|RoundedRectX:");
		appendable.AppendFormat(L"%f", ellipse.radiusX);
		_WRITE_THE_STRING;
		appendable.Append(L"|RoundedRectY:");
		appendable.AppendFormat(L"%f", ellipse.radiusY);
		_WRITE_THE_STRING;
	}


	if (border1.Get())
	{
		border1->storeInTML(ar, childLevel,normal);
	}
	if (border2.Get())
	{
		border2->storeInTML(ar, childLevel,mouseHover);
	}
	if (border3.Get())
		border3->storeInTML(ar, childLevel,mouseLClick);
	if (text1.Get())
	{
		text1->storeInTML(ar, childLevel,normal);
	}
	if (text2.Get())
	{
		text2->storeInTML(ar, childLevel,mouseHover);
	}
	if (text3.Get())
		text3->storeInTML(ar, childLevel,mouseLClick);
	if (content1.Get())
		content1->storeInTML(ar, childLevel,normal);
	if (content2.Get())
		content2->storeInTML(ar, childLevel,mouseHover);
	if (content3.Get())
		content3->storeInTML(ar, childLevel,mouseLClick);


	if (!overrideChildren)
	{
		for (int c = 0; c < children.Count(); c++)
		{
			children.ElementAt(c)->storeInTML(ar, childLevel + 1);
		}
	}

	resetAttributeString(&appendable, childLevel + 1);
	ar->WriteString(appendable);
	ar->WriteString(L"/\n");
}

/*
* Method: TControl - storeInHTML
* Purpose: Enables a Control to save itself in an HTML file
* Parameters: TFile* ar - the file to save to
* Returns: void
*/
void TControl::storeInHTML(TFile * ar)
{

	
}

/*
* Method: TControl - onCreate
* Purpose: Allows the Control To contstruct itself based off of the location it has and the
*		screen space it is given
* Parameters: RECT contain - the area it can use
* Returns: bool - success
* Note: You Must call this on the Root Control before any control can be drawn on sreen
*/
bool TControl::onCreate(RECT contain)
{
	/* Check the status of the scroll bars. By default, they are off.
	 * If the default holds, then the location of the TControl will have
	 * to be contained. However, if scrollBars are present, then the
	 * actual size of the TControl, represented by it's location, could theoretically
	 * be as large as the developer desires. The Snip will serve as the location to draw */
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|VerticalScroll"));
	if (valpoint.Get() && !valpoint->Compare(L"True") && !vScroll) // don't make a new one if one already exists
		vScroll = new TScrollBar(*this, SCROLL_VERTICAL);

	valpoint = attributes.retrieveEntry(TString(L"|HorizontalScroll"));
	if(valpoint.Get() && !valpoint->Compare(L"True") && !hScroll)
		hScroll = new TScrollBar(*this,SCROLL_HORIZONTAL);

	checkMargin(contain);
	checkHeightWidth(contain);

	if (!marginPriority && dimensions && !vScroll && !hScroll)
	{
		int heightOffset = 0;
		int widthOffset =0;
		if (dimensions->height == 0 && location.bottom == 0)
			heightOffset = 0;
		else
			heightOffset = ((contain.bottom - contain.top) - dimensions->height) / 2;

		if (dimensions->width == 0 && location.right == 0)
			widthOffset = 0;
		else
			widthOffset = ((contain.right - contain.left) - dimensions->width) / 2;
		location.top = contain.top + heightOffset;
		location.bottom = contain.bottom - heightOffset;
		location.left = contain.left + widthOffset;
		location.right = contain.right - widthOffset;
		snip = location;
	}
	else
	{
		if (!dimensions || !dimensions->height) // Use Margin regardless, nothing to override it
		{
			snip.bottom = contain.bottom - margin.bottom;
			snip.top = contain.top + margin.top;
		}
		if (!dimensions || !dimensions->width)
		{
			snip.left = contain.left + margin.left;
			snip.right = contain.right - margin.right;
		}
	}

	if (vScroll)
	{
		if (dimensions && dimensions->height)
		{
			location.top = snip.top;
			location.bottom = location.top + dimensions->height;
			if ((snip.bottom - snip.top) > (location.bottom - location.top))
				location.bottom = snip.bottom;
		}
		else
			location = snip; // if conlfict with hScroll, next code will resolve


	}
	else if (marginPriority)
	{
		location.top = snip.top;
		location.bottom = snip.bottom;
		if (dimensions && dimensions->height)
			dimensions->height = location.bottom - location.top;
	}

	if (hScroll)
	{
		if (dimensions && dimensions->width)
		{
			location.left = snip.left;
			location.right = location.left + dimensions->width;
			if ((snip.right - snip.left) > (location.right - location.left))
				location.right = snip.right;
		}
		else
		{
			location.left = snip.left;
			location.right = snip.right;
		}


	}
	else if (marginPriority)
	{
		location.left = snip.left;
		location.right = snip.right;
		if (dimensions && dimensions->width)
			dimensions->width = location.right - location.left;
	}

	valpoint = attributes.retrieveEntry(TString(L"|FixedHeight"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"true"))
			fixHeight = true;
	}
	valpoint = attributes.retrieveEntry(TString(L"|FixedWidth"));
	if (valpoint.Get())
	{
		if (!valpoint->Compare(L"true"))
			fixWidth = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|ArrayID"));
	if (valpoint.Get())
	{
		valpoint->ConvertToInt(&arrayID);
	}

	valpoint = attributes.retrieveEntry(TString(L"|Shape"));
	if (valpoint.Get())
	{
		if (valpoint->Compare(L"Ellipse"))
		{
			shape = T_Ellipse;
			ellipse.point = D2D1::Point2F((location.right + location.left) / 2, (location.top + location.bottom) / 2);
			ellipse.radiusX = (location.right - location.left) / 2;
			ellipse.radiusY = (location.bottom - location.top) / 2;
		}
		else if (valpoint->Compare(L"RoundedRectangle"))
		{
			shape = T_Rounded_Rect;
			float xRound = (location.left - location.right) / 10;
			float yRound = (location.bottom - location.top) / 10;
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectX"));
			if (valpoint.Get() )
			{
				valpoint->ConvertToFloat(&xRound);
			}
			valpoint = attributes.retrieveEntry(TString(L"|RoundedRectY"));
			if (valpoint.Get())
			{
				valpoint->ConvertToFloat(&yRound);
			}
			convertCRectToD2DRect(&location, &(roundedRect.rect));
			roundedRect.radiusX = xRound;
			roundedRect.radiusY = yRound;
		}
		else if (valpoint->Compare(L"Custom"))
		{

		}
	}


	int occ1 = 0;
	valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	while (valpoint.Get())
	{
		className += *valpoint.Get();
		className.Trim();
		className += L";";
		valpoint = attributes.retrieveEntry(TString(L"|Class"), occ1++);
	}
TrecPointer<styleTable> classy;

	TrecPointer<TDataArray<TString>> classes = className.split(L";");

	for (int c = 0; c < classes->Size(); c++)
	{
		if (classes->at(c).GetSize())
		{
			if (styles.Get())
			{

				for (int c = 0; c < styles->Count(); c++)
				{
					if (styles->ElementAt(c)->style == classes->at(c))
					{
						classy = styles->ElementAt(c);
						if (classy.Get())
							onCreate(&classy->names, contain);
						//break;
					}
				}
			}
		}
	}

	/*
	valpoint = attributes.retrieveEntry(TString(L"|Class"));
	
	if (valpoint.Get())
	{
		className = *valpoint.Get();
		if (styles.Get())
		{
			
			for (int c = 0; c < styles->Size(); c++)
			{
				if (styles->at(c)->style == className)
				{
					classy = styles->at(c);
					break;
				}
			}
		}
	}
	*/
	valpoint = attributes.retrieveEntry(TString(L"|id"));
	if (valpoint.Get())
	{
		ID = *valpoint.Get();
	}
	else
	{
		valpoint = attributes.retrieveEntry(TString(L"|ID"));
		if (valpoint.Get())
		{
			ID = *valpoint.Get();
		}
	}

	if (classy.Get())
		onCreate(&classy->names, contain); // if there is a style to use, derive apperance based off of its specifications

	onCreate(&attributes,contain); // then derive appearence based off of the individual control's spec. If there are any conflicts between
	                       // a style attribute from a style spec and that of the controls' spec, the latter is used 
	onCreate2(&attributes, contain);
	onCreate3(&attributes, contain);

	if (border1.Get())
	{
		switch (shape)
		{
		case T_Rect:
			border1->onCreate(location);
			break;
		case T_Rounded_Rect:
			border1->onCreate(roundedRect);
			break;
		case T_Ellipse:
			border1->onCreate(ellipse);
			break;
		case T_Custom_shape:
			break;
		}
	}
		
	if (content1.Get())
	{
		switch (shape)
		{
		case T_Rect:
			content1->onCreate(location,snip);
			break;
		case T_Rounded_Rect:
			content1->onCreate(roundedRect);
			break;
		case T_Ellipse:
			content1->onCreate(ellipse);
			break;
		case T_Custom_shape:
			break;
		}
	}
	if (text1.Get())
		text1->onCreate(location);
	if (border2.Get())
	{
		switch (shape)
		{
		case T_Rect:
			border2->onCreate(location);
			break;
		case T_Rounded_Rect:
			border2->onCreate(roundedRect);
			break;
		case T_Ellipse:
			border2->onCreate(ellipse);
			break;
		case T_Custom_shape:
			break;
		}
	}
	if (content2.Get())
	{
		switch (shape)
		{
		case T_Rect:
			content2->onCreate(location,snip);
			break;
		case T_Rounded_Rect:
			content2->onCreate(roundedRect);
			break;
		case T_Ellipse:
			content2->onCreate(ellipse);
			break;
		case T_Custom_shape:

			break;
		}
	}
	if (text2.Get())
		text2->onCreate(location);
	if (border3.Get())
	{
		switch (shape)
		{
		case T_Rect:
			border3->onCreate(location);
			break;
		case T_Rounded_Rect:
			border3->onCreate(roundedRect);
			break;
		case T_Ellipse:
			border3->onCreate(ellipse);
			break;
		case T_Custom_shape:
			break;
		}
	}
	if (content3.Get())
	{
		switch (shape)
		{
		case T_Rect:
			content3->onCreate(location,snip);
			break;
		case T_Rounded_Rect:
			content3->onCreate(roundedRect);
			break;
		case T_Ellipse:
			content3->onCreate(ellipse);
			break;
		case T_Custom_shape:
			break;
		}
	}
	if (text3.Get())
		text3->onCreate(location);
	int occ = 0;
	valpoint = attributes.retrieveEntry(TString(L"|ContainerLoc"), occ);

	while (valpoint.Get())
	{
		if (occ < children.Count())
		{
			TrecPointer<TControl> contt = children.ElementAt(occ);
			if (!contt.Get())
			{
				// return error code;
			}

			RECT chLoc = convertStringToRECT(valpoint.Get());

			
			chLoc.left += location.left;
			chLoc.top += location.top;
			chLoc.bottom += location.top;
			chLoc.right += location.left;
			contt->onCreate(chLoc);
		}
		else
		{
			// return error code;
		}
		valpoint = attributes.retrieveEntry(TString(L"|ContainerLoc"), ++occ);

	}

/*	for (int c = 0; c < children.Count();c++)
	{
		if(children.ElementAt(c))
		children.ElementAt(c)->onCreate(location);
	}*/

	valpoint = attributes.retrieveEntry(TString(L"|FlyoutLocation"));
	if (valpoint.Get() && flyout)
		flyout->onCreate(convertStringToRECT(valpoint.Get()));

	if (contextMenu)
		contextMenu->onCreate(location);


	updateComponentLocation();

	return true;
}

/*
* Method: TControl - Resize
* Purpose: Resizes the control upon the window being resized
* Parameters: RECT r - the new location for the control
*/
void TControl::Resize(RECT r)
{
	if (children.Count())
	{
		float w_ratio = (r.right - r.left) / (location.right - location.left);
		float h_ratio = (r.bottom - r.top) / (location.bottom - location.top);

		float t_ratio;
		if (location.top)
			t_ratio = r.top / location.top;
		else
			t_ratio = r.top;
		float l_ratio;
		if (location.left)
			l_ratio = r.left / location.left;
		else
			l_ratio = r.left;

		for (UINT rust = 0; rust < children.Count(); rust++)
		{
			if (!children.ElementAt(rust).Get())
				continue;
			RECT curLoc = children.ElementAt(rust)->getLocation();
			TPoint curPoint = TPoint(curLoc.left - location.left, curLoc.top - location.top);
			TPoint curSize = TPoint(curLoc.right - curLoc.left, curLoc.bottom - curLoc.top);
			curLoc.top = curLoc.top - curPoint.y + curPoint.y * t_ratio;
			curLoc.left = curLoc.left - curPoint.x + curPoint.x * l_ratio;
			curLoc.bottom = curLoc.bottom - curSize.y + curSize.y * h_ratio;
			curLoc.right = curLoc.right - curSize.x + curSize.x * w_ratio;
			children.ElementAt(rust)->setLocation(curLoc);
		}
	}
	location = r;
	updateComponentLocation();
}

/*
* Method: TControl - setEventHandler
* Purpose: Sets up an event handler
* Parameters: EventTarget& eh - the Event Class
* Returns: bool - just returns true
* Note: Possible redundence, expect to be DEPRECIATED
*/
bool TControl::setEventHandler(EventTarget & eh)
{
	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc = children.ElementAt(c).Get();
		if (tc)
			tc->setEventHandler(eh);
	}
	eventHandler = &eh;
	return true;
}

/*
* Method:  TControl - updateArrayID
* Purpose: Stores an index to an external array, to be determined by the software using the control
* Parameters: int aid - the array index to store, default -1
* Returns: void
*/
void TControl::updateArrayID(int aid)
{
	arrayID = aid;
}

/* Purpose: to allow the TControl to shift it's contents according to how it is scrolled.
 * Caller: Member TScrollBars, either vScroll or hScroll
 * Parameter: int (x) how to shift itself horizontally
 * Parameter: int (y) how to shift itself vertically
 * return: bool, just because
 * Details: Changes the contents location on the board, reflected in the changes to
 *		TControl's location, but not where it draws, hence why it's own snip stays the same
 *		However, the contents might include child locations so their snips might have to be 
 *		updated*/
bool TControl::onScroll(int x, int y)
{
	location.left += x;
	location.right += x;
	location.bottom += y;
	location.top += y;
	for (int c = 0; c < children.Count(); c++)
	{
		TControl* cont = children.ElementAt(c).Get();
		if (!cont)    // don't work with a null pointer
			continue;
		cont->onBeingScrolled(x, y, snip);
	}

	updateComponentLocation();

	return false;
}

/*
* Method: TControl - onBeingScrolled
* Purpose: To be called whenever a parent control is scrolling
* Parameters: int x - the degree to which the control is being moved left or right
*				int y - the degrees to which the control is being moved up or down
*				const RECT& p_snip - the area of the parent control
* Returns: bool - success
*/
bool TControl::onBeingScrolled(int x, int y, const RECT & p_snip)
{
	// Update real location of TControl
	location.left += x;
	location.right += x;
	location.bottom += y;
	location.top += y;

	// To-Do: check to see if zero'd snips need to be restored



	// if p_snip is zero, just zero snip, else update snip as usual
	if (isSnipZero(p_snip))
		zeroSnip(snip);
	else
	{
		snip.left += x;
		snip.right += x;
		snip.bottom += y;
		snip.top += y;
	}

	// Make the appropriate adjustments to the snip based off of provided snip
	if (snip.bottom < p_snip.top)
		zeroSnip(snip);
	else if (snip.top < p_snip.top)
		snip.top = p_snip.top;

	if (snip.top > p_snip.bottom)
		zeroSnip(snip);
	else if (snip.bottom > p_snip.bottom)
		snip.bottom = p_snip.bottom;

	if (snip.left > p_snip.right)
		zeroSnip(snip);
	else if (snip.right > p_snip.right)
		snip.right = p_snip.right;

	if (snip.right < p_snip.left)
		zeroSnip(snip);
	else if (snip.left < p_snip.left)
		snip.left = p_snip.left;

	for (int c = 0; c < children.Count(); c++)
	{
		TControl* cont = children.ElementAt(c).Get();
		if (!cont)    // don't work with a null pointer
			continue;
		cont->onBeingScrolled(x, y, snip);
	}

	updateComponentLocation();

	return false;
}

/*
* Method: TControl - scroll
* Purpose: Use in the event that scrolling needs to be done by a parent so control ends up in view
* Parameters: RECT& loc - the location of the control that needs to be in view
* Returns: void
*/
void TControl::scroll(RECT& loc)
{
	// Make sure that this Control is not out of view

	TControl* sh_parent = parent.Get();

	if (isSnipZero(snip) && sh_parent)
		sh_parent->scroll(loc);

	// Test for need to Vertically Scroll
	if (loc.bottom < snip.top)
	{
		if (!vScroll && sh_parent)
		{
			sh_parent->scroll(loc);
			return;
		}

	}
	else if (loc.top > snip.bottom)
	{
		if (!vScroll && sh_parent)
		{
			sh_parent->scroll(loc);
			return;
		}
	}

	if (loc.left > snip.right)
	{
		if (!hScroll && sh_parent)
		{
			sh_parent->scroll(loc);
			return;
		}
	}
	else if (loc.right < snip.left)
	{
		if (!hScroll && sh_parent)
		{
			sh_parent->scroll(loc);
			return;
		}
	}

}

/*
* Method: TControl - SetContextMenu
* Purpose: Set up a Context Menu to show if Control is ever right-Clicked
* Parameters: TrecPointer<TControl> cm - Smart Pointer to a Control to serve as the Context Menu
* Returns: bool - true if successful, false if error occurs
*/
bool TControl::SetContextMenu(TrecPointer<TControl> cm)
{
	if(contextMenu)
	return false;
	if (!cm.Get())
		return false;
	try
	{
		contextMenu = dynamic_cast<TContextMenu*>(cm.Get());
		
		return true;
	}
	catch (std::bad_cast& bc)
	{
		return false;
	}

}

/*
* Method: TControl - BreakShared
* Purpose: Allows Tcontrol to be deleted by removing links to it by its conponents
* Parameters: void
* Returns: void
* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
*/
void TControl::BreakShared()
{
	if (text1.Get())
		text1->BreakShared();
	if (content1.Get())
		content1->BreakShared();
	if (border1.Get())
		border1->BreakShared();

	if (text2.Get())
		text2->BreakShared();
	if (content2.Get())
		content2->BreakShared();
	if (border2.Get())
		border2->BreakShared();

	if (text3.Get())
		text3->BreakShared();
	if (content3.Get())
		content3->BreakShared();
	if (border3.Get())
		border3->BreakShared();
}

/*
* Method: TControl - AddClass
* Purpose: Labels to serve as id's to draw styles from, useful in determining what HTML/TML element was used in creating the control
* Parameters: TString& t - Class Name to add
* Returns: void
*/
void TControl::AddClass(const TString & t)
{
	TString trimmedT = t.GetTrim();
	if (className.GetSize())
	{

		className += L";";
		className += trimmedT;
	}
	else
		className = trimmedT;
}
/*
* Method: TControl - GetID
* Purpose: Retrieves the ID for the Control
* Parameters: void
* Returns: TString - the ID of the Control (Careful, String could be blank)
*/
TString TControl::GetID()
{
	return ID;
}

UCHAR * TControl::GetAnaGameType()
{
	return nullptr;
}

/*
* Method: TControl - onDraw
* Purpose: Draws the control
* Parameters: void
* Returns: void
*/
void TControl::onDraw(TObject* obj)
{
	if (!isActive)
		return;
	if (mState == mouseLClick)
	{
		if (content3.Get())
			content3->onDraw(location, snip);
		else if (content1.Get())
			content1->onDraw(location, snip);
		if (border3.Get())
			border3->onDraw(location, snip);
		else if (border1.Get())
			border1->onDraw(location, snip);
		if (text3.Get())
			text3->onDraw(location, snip, obj);
		else if (text1.Get())
			text1->onDraw(location, snip, obj);
	}
	else if (mState == mouseHover)
	{
		if (content2.Get())
			content2->onDraw(location, snip);
		else if (content1.Get())
			content1->onDraw(location, snip);
		if (border2.Get())
			border2->onDraw(location, snip);
		else if (border1.Get())
			border1->onDraw(location, snip);
		if (text2.Get())
			text2->onDraw(location, snip, obj);
		else if (text1.Get())
			text1->onDraw(location, snip, obj);
	}
	else
	{
		if (content1.Get())
			content1->onDraw(location, snip);
		if (border1.Get())
			border1->onDraw(location, snip);
		if (text1.Get())
			text1->onDraw(location, snip, obj);
	}

	if (vScroll)
		vScroll->updateDraw();
	if (hScroll)
		hScroll->updateDraw();

	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->onDraw(obj);
	}
}

/*
* Method: TControl - getLocation
* Purpose: Retrieves the current physical location on the RenderTarget of the control
* Parameters: void
* Returns: RECT - the location
*/
RECT TControl::getLocation()
{
	return location;
}

/*
* Method: TControl - getSnip
* Purpose: Retrieves the area of the screen the control can currently draw to
* Parameters: void
* Returns: RECT - the snip that can be drawn
*/
RECT TControl::getSnip()
{
	return snip;
}

/*
* Method: TControl - getMargin
* Purpose: Retrieves the space between the control's location and the area it was given
* Parameters: void
* Returns: RECT - the margin
*/
RECT TControl::getMargin()
{
	return margin;
}

/*
* Method: TControl - getRenderTarget
* Purpose: Retrieves the RenderTarget the Control is currently set to draw to
* Parameters: void
* Returns: TrecComPointer<ID2D1RenderTarget> - Smart Pointer holding the Render Target 
*/
TrecComPointer<ID2D1RenderTarget> TControl::getRenderTarget()
{
	return renderTarget;
}

/*
* Method: TControl - getParent
* Purpose: Retrieves the parent control of the current control
* Parameters: void
* Returns: TrecPointer<TControl> - Smart Pointer to the Parent Control
*/
TrecPointer<TControl> TControl::getParent()
{
	return TrecPointerKey::GetTrecPointerFromSoft<TControl>(parent);
}

/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/
void TControl::setExternalBounds(RECT r)
{
	location = r;
	boundsPreset = true;
}

/*
* Method:  TControl - getLayoutStatus
* Purpose: Returns whether or not the control is a TLayout 
* Parameters: void
* Returns: bool - whether or not control is a TLayout
* Note: DEPRECIATED - C++ RTTI functionality can assume this purpose with greater presision
*/
bool TControl::getLayoutStatus()
{
	return isLayout;
}

/*
* Method: TControl - offsetLocation
* Purpose: Moves the Control to the specified point
* Parameters: TPoint cp - the point to move the control to 
* Returns: void
*/
void TControl::offsetLocation(TPoint cp)
{
	int height = location.bottom - location.top;
	int width = location.right - location.left;

	location.top = cp.y;
	location.left = cp.x;
	location.bottom = location.top + height;
	location.right = location.left + width;

	snip = location;

	updateComponentLocation();
}

/*
* Method: TControl - ShiftHorizontal
* Purpose: Moves the Control left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the control and which direction
* Returns: void
*/
void TControl::ShiftHorizontal(int degrees)
{
	location.left += degrees;
	location.right += degrees;
	if (!isSnipZero(convertRECTToD2DRectF(snip)))
	{
		snip.left += degrees;
		snip.right += degrees;
	}

	if (text1.Get())
		text1->ShiftHorizontal(degrees);
	if (text2.Get())
		text2->ShiftHorizontal(degrees);
	if (text3.Get())
		text3->ShiftHorizontal(degrees);

	if (content1.Get())
		content1->ShiftHorizontal(degrees);
	if (content2.Get())
		content2->ShiftHorizontal(degrees);
	if (content3.Get())
		content3->ShiftHorizontal(degrees);

	if (border1.Get())
		border1->ShiftHorizontal(degrees);
	if (border2.Get())
		border2->ShiftHorizontal(degrees);
	if (border3.Get())
		border3->ShiftHorizontal(degrees);
}

/*
* Method: TControl - ShiftVertical
* Purpose:Moves the Control up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the control and which direction
* Returns: void
*/
void TControl::ShiftVertical(int degrees)
{
	location.top += degrees;
	location.bottom += degrees;
	if (!isSnipZero(convertRECTToD2DRectF(snip)))
	{
		snip.top += degrees;
		snip.bottom += degrees;
	}

	if (text1.Get())
		text1->ShiftVertical(degrees);
	if (text2.Get())
		text2->ShiftVertical(degrees);
	if (text3.Get())
		text3->ShiftVertical(degrees);

	if (content1.Get())
		content1->ShiftVertical(degrees);
	if (content2.Get())
		content2->ShiftVertical(degrees);
	if (content3.Get())
		content3->ShiftVertical(degrees);

	if (border1.Get())
		border1->ShiftVertical(degrees);
	if (border2.Get())
		border2->ShiftVertical(degrees);
	if (border3.Get())
		border3->ShiftVertical(degrees);
}

/*
* Method: TControl - setMinHeight
* Purpose: Sets the Minimum height this control can be
* Parameters: int h - the minimum height
* Returns: void
*/
void TControl::setMinHeight(int h)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->minHeight = h;
}

/*
* Method: TControl - setMaxHeight
* Purpose: Sets the Maximum height this control can be
* Parameters: int h - the maximum height this control can be
* Returns: void
*/
void TControl::setMaxHeight(int h)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->maxHeight = h;
}

/*
* Method: TControl - setMinWidth
* Purpose: Sets the minimum width the control can be
* Parameters: int w - the minimum width
* Returns: void
*/
void TControl::setMinWidth(int w)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->minWidth = w;
}

/*
* Method: TControl - setMaxWidth
* Purpose: Sets the maximum width the control can be
* Parameters: int w - the max width
* Returns: void
*/
void TControl::setMaxWidth(int w)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->maxWidth = w;
}

/*
* Method: TControl - setWidth
* Purpose: Sets the current witdh of the control
* Parameters: int w - the new width
* Returns: void
*/
void TControl::setWidth(int w)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->width = w;
}

/*
* Method: TControl - setHeight
* Purpose: Sets the current height of the control
* Parameters: int h - the height to set the control to
* Returns: void
*/
void TControl::setHeight(int h)
{
	if (!dimensions)
		generateSizeControl();
	dimensions->height = h;
}

/*
* Method: TControl - addAttribute
* Purpose: Adds a new attribute to use during creation
* Parameters: TString& attr - the attribute name to add
*				TrecPointer<TString> val - the value of that attribute
* Returns: bool - success or error
* Note: Call this method BEFORE calling onCreate. Otherwise, it will have no effect on the Control's
*	appearence or functionality
*/
bool TControl::addAttribute(const TString& attr, TrecPointer<TString> val)
{
	if(!attr.GetSize() || !val.Get())
	return false;
	attributes.addEntry(attr, val);
	return true;
}

/*
* Method: TControl - addChild
* Purpose: Adds a child control to the Current Control to hold
* Parameters: TrecPointer<TContainer> tcon - the Child to add
* Returns: bool - success or error
*/
bool TControl::addChild(TrecPointer<TControl> tcon)
{
	if(!tcon.Get())
	return false;
	children.Add(tcon);
	
	tcon->setParent(TrecPointerKey::GetTrecPointerFromSoft<TControl>( tThis));
	return true;
	
}

/*
* Method: TControl - setLocation
* Purpose: Sets the Control in a new location
* Parameters: RECT R - the new location
* Returns: void
*/
void TControl::setLocation(RECT r)
{
	location = r;
}

/*
* Method: TControl - setMBottom
* Purpose: Sets the margin on the bottom, likely used by the Builder when visually designing a control
* Parameters: int b - the new bottom margin
* Returns: void
*/
void TControl::setMBottom(int b)
{
	margin.bottom = b;
	marginSet = true;
}

/*
* Method: TControl - setMBottom
* Purpose: Sets the margin on the top, likely used by the Builder when visually designing a control
* Parameters: int t - the new top margin
* Returns: void
*/
void TControl::setMTop(int t)
{
	margin.top = t;
	marginSet = true;
}

/*
* Method: TControl - setMRight
* Purpose:Sets the margin on the right, likely used by the Builder when visually designing a control
* Parameters: int r - the new right margin
* Returns: void
*/
void TControl::setMRight(int r)
{
	margin.right = r;
	marginSet = true;
}

/*
* Method: TControl - setMLeft
* Purpose: Sets the margin on the left, likely used by the Builder when visually designing a control
* Parameters: int l - the new left margin
* Returns: void
*/
void TControl::setMLeft(int l)
{
	margin.left = l;
	marginSet = true;
}

/*
* Method: TControl - determineMinHeightNeeded
* Purpose: Determines the minimum height needed, base returns the current height, useful for sub classes
* Parameters: void
* Returns: UINT - the minimum height needed
*/
UINT TControl::determineMinHeightNeeded()
{
	return location.bottom - location.top;
}

/*
* Method:
* Purpose:
* Parameters:
* Returns:
*/
void TControl::SetNewLocation(const RECT & r)
{
	location = r;
	if (!isSnipZero(convertRECTToD2DRectF(snip)))
		snip = r;

	if (text1.Get())
	{
		text1->bounds = convertRECTToD2DRectF(r);
		text1->reCreateLayout();
	}
	if (text2.Get())
	{
		text2->bounds = convertRECTToD2DRectF(r);
		text2->reCreateLayout();
	}
	if (text3.Get())
	{
		text3->bounds = convertRECTToD2DRectF(r);
		text3->reCreateLayout();
	}
	if (border1.Get())
	{
		border1->loci = convertRECTToD2DRectF(r);
		border1->snip = convertRECTToD2DRectF(snip);
	}
	if (border2.Get())
	{
		border2->loci = convertRECTToD2DRectF(r);
		border2->snip = convertRECTToD2DRectF(snip);
	}
	if (border3.Get())
	{
		border3->loci = convertRECTToD2DRectF(r);
		border3->snip = convertRECTToD2DRectF(snip);
	}
	if (content1.Get())
	{
		content1->location = convertRECTToD2DRectF(r);
		content1->snip = convertRECTToD2DRectF(snip);
	}
	if (content2.Get())
	{
		content2->location = convertRECTToD2DRectF(r);
		content2->snip = convertRECTToD2DRectF(snip);
	}
	if (content3.Get())
	{
		content3->location = convertRECTToD2DRectF(r);
		content3->snip = convertRECTToD2DRectF(snip);
	}
}

/*
* Method: TControl - ShrinkHeight
* Purpose: Reduces the height of the control down to what is needed --> just shrinks its children
*		some of whom might find ways to shrink themselves
* Parameters: void
* Returns: void
*/
void TControl::ShrinkHeight()
{
	for (UINT c = 0; c < children.Count(); c++)
	{
		if (!children.ElementAt(c).Get())
			continue;
		children.ElementAt(c)->ShrinkHeight();
	}
}

/*
* Method: TControl - setParent
* Purpose: Sets the controls parent for reference in the tree
* Parameters: TrecPointer<TControl> tcp - the pointer to the parent
* Returns: void
* To-Do: Add code to make sure the parent is not a child or grand-child ( or lower)
*/
void TControl::setParent(TrecPointer<TControl> tcp)
{
	parent = TrecPointerKey::GetSoftPointerFromTrec<TControl>(tcp);
}

/*
* Method: TControl - getText
* Purpose: Returns the Text Component labeled by the id
* Parameters: int n - the id of the component
* Returns: TrecPointer<TText> - Smart Pointer to the target text component
*/
TrecPointer<TText> TControl::getText(int n)
{
	switch (n)
	{
	case 1:
		return text1;
	case 2:
		return text2;
	case 3:
		return text3;
	default:
		return TrecPointer<TText>();
	}

}

/*
* Method: TControl - getContent
* Purpose: Returns the Content Component labeled by the id
* Parameters: int n - the id of the component
* Returns: const TrecPointer<TContent> - Smart Pointer to the target content component
*/
TrecPointer<TContent> TControl::getContent(int n)
{
	switch (n)
	{
	case 1:
		return  content1;
	case 2:
		return  content2;
	case 3:
		return  content3;
	default:
		return  TrecPointer<TContent>();
	}
}

/*
* Method: TControl - getBorder
* Purpose: Returns the Border Component labeled by the id
* Parameters: int n - the id of the component
* Returns: const TrecPointer<TBorder> - Smart Pointer to the target Border Component 
*/
TrecPointer<TBorder> TControl::getBorder(int n)
{
	switch (n)
	{
	case 1:
		return  border1;
	case 2:
		return  border2;
	case 3:
		return  border3;
	default:
		return  TrecPointer<TBorder>();
	}
	
}

/*
* Method: TControl - setNewText
* Purpose: Prepares a new TText component, likely called by the Builder when designing controls
* Parameters: int n - the id of the new TText o override
* Returns: void
*/
void TControl::setNewText(int n)
{
	switch (n)
	{
	case 1:
		text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		break;
	case 2:
		text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		break;
	case 3:
		text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		break;
	}
}

/*
* Method: TControl - setWrapperPointer
* Purpose: Sets the wrapper component, DO NOT USE
* Parameters: TrecPointer<TControl> tcp -  the wrapper control
* Returns: void
* Note: DEPRECIATED - Written when TypeSafetyControls were used (functionality being replaced by C++ RTTI

void TControl::setWrapperPointer(TrecPointer<TControl> tcp)
{
	//PointerCase = tcp;
}*/

/*
* Method: TControl - onCreate
* Purpose: Sets up the basic components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate(TMap<TString>* att, RECT loc)
{
	TrecPointer<TString> valpoint;	// 
	
	valpoint = att->retrieveEntry(TString(L"|BorderThickness"));

	borderThick:
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		valpoint->ConvertToFloat(&(border1->thickness));         // Logic Bug needs fixing
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto borderThick;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|BorderColor"));
	borderColor_:
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		border1->color = convertStringToD2DColor(valpoint.Get());
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto borderColor_;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|ContentThickness"));
	contThick:
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		valpoint->ConvertToFloat(&(content1->thickness));
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto contThick;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|ContentColor"));
	contColor:
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		content1->color = convertStringToD2DColor(valpoint.Get());
	}
	/*else
	{
		valpoint = att->retrieveEntry(TString(L""));
		goto contColor;
	}*/

	// 
	valpoint = att->retrieveEntry(TString(L"|Caption"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));

		text1->text = valpoint.Get();

	}

	// 
	valpoint = att->retrieveEntry(TString(L"|CaptionLocale"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->locale = valpoint.Get();

	}

	// 
	valpoint = att->retrieveEntry(TString(L"|Font"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->font = valpoint.Get();

	}

	// 
	valpoint = att->retrieveEntry(TString(L"|FontColor"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|FontSize"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		 valpoint->ConvertToFloat(&(text1->fontSize) );
	}

	valpoint = att->retrieveEntry(TString(L"|HorizontalAlignment"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->setNewHorizontalAlignment(convertStringToTextAlignment(valpoint.Get()));
	}
	valpoint = att->retrieveEntry(TString(L"|VerticalAlignment"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->setNewVerticalAlignment(convertStringToParagraphAlignment(valpoint.Get()));
	}
	valpoint = att->retrieveEntry(TString(L"|ContentColor2"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		content1->color2 = convertStringToD2DColor(valpoint.Get());
		content1->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ContentGrad1"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//content1->secondColor = true;
		content1->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|ContentGrad2"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//content1->secondColor = true;
		content1->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|BorderColor2"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		border1->color2 = convertStringToD2DColor(valpoint.Get());
		border1->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|BorderGrad1"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//border1->secondColor = true;
		border1->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|BorderGrad2"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//border1->secondColor = true;
		border1->gradients[1].position = entry;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|Caption"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));

		text1->text = valpoint.Get();
		;
	}

	valpoint = att->retrieveEntry(TString(L"|TextColor2"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		text1->color2 = convertStringToD2DColor(valpoint.Get());
		text1->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|TextGrad1"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//text1->secondColor = true;
		text1->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|TextGrad2"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//text1->secondColor = true;
		text1->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|TextGradMode"));
	if (valpoint.Get())
	{
		if (!text1.Get())
			text1 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		if (valpoint->Compare(L"Radial"))
			text1->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|ContentGradMode"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		if (valpoint->Compare(L"Radial"))
			content1->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|BorderGradMode"));
	if (valpoint.Get())
	{
		if (!border1.Get())
			border1 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		if (valpoint->Compare(L"Radial"))
			border1->useRadial = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ImageSource"));
	if (valpoint.Get())
	{
		if (!content1.Get())
			content1 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		valpoint->Replace(L"/", L"\\");
		int res = generateImage(content1, valpoint, loc);
	}
	return false;
}

/*
* Method: TControl - onCreate2
* Purpose: Sets up the mouse hover components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate2(TMap<TString>* att, RECT loc)
{
	TrecPointer<TString> valpoint;	// 

	valpoint = att->retrieveEntry(TString(L"|HoverBorderThickness"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(border2->thickness));         // Logic Bug needs fixing
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverBorderColor"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		border2->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverContentThickness"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(content2->thickness));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverContentColor"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		content2->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverCaption"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->text = valpoint.Get();

	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverCaptionLocale"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->locale = valpoint.Get();
		;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverFont"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->font = valpoint.Get();
		;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverFontColor"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverFontSize"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(text2->fontSize));
	}
	valpoint = att->retrieveEntry(TString(L"|HoverHorizontalAlignment"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->setNewHorizontalAlignment(convertStringToTextAlignment(valpoint.Get()));
	}
	valpoint = att->retrieveEntry(TString(L"|HoverVerticalAlignment"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->setNewVerticalAlignment(convertStringToParagraphAlignment(valpoint.Get()));
	}

	valpoint = att->retrieveEntry(TString(L"|HoverContentColor2"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		content2->color2 = convertStringToD2DColor(valpoint.Get());
		content2->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverContentGrad1"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//content2->secondColor = true;
		content2->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverContentGrad2"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//content2->secondColor = true;
		content2->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverBorderColor2"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		border2->color2 = convertStringToD2DColor(valpoint.Get());
		border2->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverBorderGrad1"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//border2->secondColor = true;
		border2->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverBorderGrad2"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//border2->secondColor = true;
		border2->gradients[1].position = entry;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|HoverCaption"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->text = valpoint.Get();
		;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverTextColor2"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text2->color2 = convertStringToD2DColor(valpoint.Get());
		text2->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverTextGrad1"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//text2->secondColor = true;
		text2->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverTextGrad2"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//text2->secondColor = true;
		text2->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverTextGradMode"));
	if (valpoint.Get())
	{
		if (!text2.Get())
		{
			if (text1.Get())
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text2 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			text2->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverContentGradMode"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			content2->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|HoverBorderGradMode"));
	if (valpoint.Get())
	{
		if (!border2.Get())
		{
			if (border1.Get())
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border2 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			border2->useRadial = true;
	}

	valpoint = att->retrieveEntry(TString(L"|HoverImageSource"));
	if (valpoint.Get())
	{
		if (!content2.Get())
		{
			if (content1.Get())
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content2 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		generateImage(content2, valpoint, loc);
	}
	return false;

}

/*
* Method: TControl - onCreate3
* Purpose: Sets up the click event components based off of the available attributes
* Parameters: TMap<TString>* att - list of attributes
*				RECT loc - the location ot use
* Returns: bool - false
*/
bool TControl::onCreate3(TMap<TString>* att, RECT loc)
{
	TrecPointer<TString> valpoint;	// 

	valpoint = att->retrieveEntry(TString(L"|ClickBorderThickness"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(border3->thickness));         // Logic Bug needs fixing
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickBorderColor"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		border3->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickContentThickness"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(content3->thickness));
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickContentColor"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		content3->color = convertStringToD2DColor(valpoint.Get());
	}

	valpoint = att->retrieveEntry(TString(L"|ClickContentColor2"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		content3->color2 = convertStringToD2DColor(valpoint.Get());
		content3->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickContentGrad1"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//content3->secondColor = true;
		content3->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickContentGrad2"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//content3->secondColor = true;
		content3->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickBorderColor2"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		border3->color2 = convertStringToD2DColor(valpoint.Get());
		border3->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickBorderGrad1"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//border3->secondColor = true;
		border3->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickBorderGrad2"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//border3->secondColor = true;
		border3->gradients[1].position = entry;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickCaption"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->text = valpoint.Get();
		;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickTextColor2"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->color2 = convertStringToD2DColor(valpoint.Get());
		text3->secondColor = true;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickTextGrad1"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		float entry = 0.0f;
		valpoint->ConvertToFloat(&entry);
		//text3->secondColor = true;
		text3->gradients[0].position = entry;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickTextGrad2"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		float entry = 1.0f;
		valpoint->ConvertToFloat(&entry);
		//text3->secondColor = true;
		text3->gradients[1].position = entry;
	}

	valpoint = att->retrieveEntry(TString(L"|ClickTextGradMode"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			text3->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickContentGradMode"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			content3->useRadial = true;
	}
	valpoint = att->retrieveEntry(TString(L"|ClickBorderGradMode"));
	if (valpoint.Get())
	{
		if (!border3.Get())
		{
			if (border1.Get())
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(border1, (this));
			else
				border3 = TrecPointerKey::GetNewTrecPointer<TBorder>(renderTarget, (this));
		}
		if (valpoint->Compare(L"Radial"))
			border3->useRadial = true;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickCaptionLocale"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->locale = valpoint.Get();
		;
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickFont"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->font = valpoint.Get();
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickFontColor"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->color = convertStringToD2DColor(valpoint.Get());
	}

	// 
	valpoint = att->retrieveEntry(TString(L"|ClickFontSize"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		valpoint->ConvertToFloat(&(text3->fontSize));
	}
	valpoint = att->retrieveEntry(TString(L"|ClickHorizontalAlignment"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->setNewHorizontalAlignment(convertStringToTextAlignment(valpoint.Get()));
	}
	valpoint = att->retrieveEntry(TString(L"|ClickVerticalAlignment"));
	if (valpoint.Get())
	{
		if (!text3.Get())
		{
			if (text1.Get())
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(text1, (this));
			else
				text3 = TrecPointerKey::GetNewTrecPointer<TText>(renderTarget, (this));
		}
		text3->setNewVerticalAlignment(convertStringToParagraphAlignment(valpoint.Get()));
	}

	valpoint = att->retrieveEntry(TString(L"|ClickImageSource"));
	if (valpoint.Get())
	{
		if (!content3.Get())
		{
			if (content1.Get())
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(content1, (this));
			else
				content3 = TrecPointerKey::GetNewTrecPointer<TContent>(renderTarget, (this));
		}
		generateImage(content3, valpoint,loc);
	}

	return false;

}

/*
* Method: TControl - updateComponentLocation
* Purpose: Update the locations of the control's components - call whenever the control's location is updated
* Parameters:
* Returns:
*/
void TControl::updateComponentLocation()
{
	// Borders
	if (border1.Get())
	{
		convertCRectToD2DRect(&snip, &(border1->snip));
		convertCRectToD2DRect(&location, &(border1->loci));
	}
	if (border2.Get())
	{
		convertCRectToD2DRect(&snip, &(border2->snip));
		convertCRectToD2DRect(&location, &(border2->loci));
	}
	if (border3.Get())
	{
		convertCRectToD2DRect(&snip, &(border3->snip));
		convertCRectToD2DRect(&location, &(border3->loci));
	}

	// Content
	if (text1.Get())
	{
		convertCRectToD2DRect(&location, &(text1->bounds));
	}
	if (text2.Get())
	{
		convertCRectToD2DRect(&location, &(text2->bounds));
	}
	if (text3.Get())
	{
		convertCRectToD2DRect(&location, &(text3->bounds));
	}

	// Content
	if (content1.Get())
	{
		convertCRectToD2DRect(&snip, &(content1->snip));
		convertCRectToD2DRect(&location, &(content1->location));
	}
	if (content2.Get())
	{
		convertCRectToD2DRect(&snip, &(content2->snip));
		convertCRectToD2DRect(&location, &(content2->location));
	}
	if (content3.Get())
	{
		convertCRectToD2DRect(&snip, &(content3->snip));
		convertCRectToD2DRect(&location, &(content3->location));
	}

}




/*
* Method: TControl - generateImage
* Purpose: Sets up an image for the Control if an attribute calls for it
* Parameters: TrecPointer<TContent> tcon - the Content component to attach the image to
*				TrecPointer<TString> p - the file name of the target image
*				RECT loc - the location where the image should be
* Returns: int - error (0 if successful)
*/
int TControl::generateImage(TrecPointer<TContent> tcon, TrecPointer<TString> p, RECT loc)
{
	if (!tcon.Get() || !(p.Get()))
		return 1;
	IWICImagingFactory* imageFactory = NULL;
	IWICBitmapDecoder* decoder = NULL;
	IWICBitmapFrameDecode* framDec = NULL;
	IWICFormatConverter* converter = NULL;
	IWICBitmapScaler* scale = nullptr;
	UINT frameCount = 0;
	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&imageFactory);
	if (!SUCCEEDED(result))
		return 2;
	WCHAR* buff = p->GetBufferCopy();
	result = imageFactory->CreateDecoderFromFilename(buff,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder);
	delete[] buff;
	buff = nullptr;
	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		return 3;
	}
	
	result = decoder->GetFrame(0, &framDec);
	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		return 4;
	}

	UINT h = 0, w = 0;

	framDec->GetSize(&w, &h);
	result = imageFactory->CreateFormatConverter(&converter);
	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		return 5;
	}
	
	result = imageFactory->CreateBitmapScaler(&scale);

	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return 6;
	}


	//TO-Do - add mechanism for image being it's own size
	result = scale->Initialize(framDec, loc.right - loc.left, loc.bottom - loc.top,
		WICBitmapInterpolationModeFant);

	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return 7;
	}

	result = converter->Initialize(scale, GUID_WICPixelFormat32bppPRGBA,
		WICBitmapDitherTypeNone,NULL,0.0f,
		WICBitmapPaletteTypeMedianCut);

	scale->Release();
	scale = nullptr;

	if (!SUCCEEDED(result))
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return 8;
	}
	if (!renderTarget.Get())
	{
		imageFactory->Release();
		decoder->Release();
		converter->Release();
		return 9;
	}
	TrecComPointer<ID2D1Bitmap>::TrecComHolder map;
	result = renderTarget->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
	tcon->image = map.Extract();
	HRESULT result2 = renderTarget->CreateBitmapFromWicBitmap(converter, map.GetPointerAddress());
	tcon->cropImage = map.Extract();
	//decoder->GetFrameCount(&frameCount);
	imageFactory->Release();
	decoder->Release();
	converter->Release();
	if (!SUCCEEDED(result) || !SUCCEEDED(result2))
		return 10;
	return 0;
}

/*
* Method: TControl - checkMargin
* Purpose: Sets up the margin attribute if available, called by onCreate
* Parameters: RECT contain - the location given to onCreate
* Returns: void
*/
void TControl::checkMargin(RECT contain)
{
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Margin"));
	//margin = RECT{ 0,0,0,0 };
	if (valpoint.Get())
	{
		margin = convertStringToRECT(valpoint.Get());
		marginSet = true;
	}
	
	if (snip.bottom == 0 && snip.right == 0)
	{
		snip.bottom = contain.bottom - margin.bottom;
		snip.left = contain.left + margin.left;
		snip.top = contain.top + margin.top;
		snip.right = contain.right - margin.right;
	}
	else
	{
		snip.bottom = snip.bottom - margin.bottom;
		snip.left = snip.left + margin.left;
		snip.right = snip.right - margin.right;
		snip.top = snip.top + margin.top;
	}
	if (!hScroll)
	{
		location.left = snip.left;
		location.right = snip.right;
	}
	if (!vScroll)
	{
		location.top = snip.top;
		location.bottom = snip.bottom;
	}
}

/*
* Method: TControl - checkHeightWidth
* Purpose: checks for the height and width attributes of the control, called by onCreate
* Parameters: RECT r the location given to onCreate
* Returns: void
*/
void TControl::checkHeightWidth(RECT r)
{
	//

	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|Height"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->height)))
			dimensions->height = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|Width"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->width)))
			dimensions->width = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MaxHeight"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->maxHeight)))
			dimensions->maxHeight = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MaxWidth"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->maxWidth)))
			dimensions->maxWidth = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MinHeight"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->minHeight)))
			dimensions->minHeight = 0;
	}

	valpoint = attributes.retrieveEntry(TString(L"|MinWidth"));
	if (valpoint.Get())
	{
		generateSizeControl();
		if (valpoint->ConvertToInt(&(dimensions->minWidth)))
			dimensions->minWidth = 0;
	}
	if (dimensions)
	{
		if (dimensions->height > r.bottom - r.top && !vScroll)
			dimensions->height = r.bottom - r.top;
		if (dimensions->width > r.right - r.left && !hScroll)
			dimensions->width = r.right - r.left;
	}
}

/*
* Method: TControl - generateSizeControl
* Purpose: Sets up the heights and widths of the control (stored as pointer so controls that
*		don't have this attribute can save memory this way
* Parameters: void
* Returns: void
*/
void TControl::generateSizeControl()
{
	if (!dimensions)
	{
		dimensions = new sizeControl;

		// only do if dimensions is new, otherwise, we risk overriding previously set values
		dimensions->height = 0;
		dimensions->maxHeight = 0;
		dimensions->minHeight = 0;
		dimensions->width = 0;
		dimensions->maxWidth = 0;
		dimensions->minWidth = 0;
	}
}

/*
* Method: TControl - hasEvent
* Purpose: Checks to see whether the control is set to respond to a certain event with a handler
* Parameters: R_Message_Type type - type of message to respond to
* Returns: bool - presense of type handler
*/
bool TControl::hasEvent(R_Message_Type type)
{

		for (int c = 0; c < eventList.Size(); c++)
		{
			if ((eventList)[c].eventType == type)
				return true;
		}
	return false;
}

/*
* Method: TControl::resetArgs
* Purpose: resets the Arguements handlers have to their default values
* Parameters: void
* Returns: void
*/
void TControl::resetArgs()
{
	

	args.control = this;
	args.isClick = false;
	args.isLeftClick = false;
	args.methodID = -1;
	args.point.x = 0.0f;
	args.point.y = 0.0f;
	args.positive = false;
	args.text.Empty();
	args.type = L'\0';
}

/*
* Method: TControl - getEventID
* Purpose: Controls never touch their handlers. Instead, they're given an index to a collection of handlers
* Parameters: R_Message_Type type -  the type of message to respond to
* Returns: int - index of handler to call (-1 if handler not provided)
*/
int TControl::getEventID(R_Message_Type type)
{

	for (int c = 0; c < eventList.Size(); c++)
	{
		if (eventList[c].eventType == type)
			return eventList.at(c).eventID;
	}
	
	return -1;
}

/*
* Method: TControl - SetToRenderTarget
* Purpose: Updates each component to the controls current render target, call if render target is ever switched
* Parameters: void
* Returns: void
*/
void TControl::SetToRenderTarget()
{
	if (border1.Get())
		border1->rt = renderTarget;
	if (border2.Get())
		border2->rt = renderTarget;
	if (border3.Get())
		border3->rt = renderTarget;

	if (content1.Get())
		content1->rt = renderTarget;
	if (content2.Get())
		content2->rt = renderTarget;
	if (content3.Get())
		content3->rt = renderTarget;

	if (text1.Get())
		text1->rt = renderTarget;
	if (text2.Get())
		text2->rt = renderTarget;
	if (text3.Get())
		text3->rt = renderTarget;
}

/*
* Method: TControl - addEventID
* Purpose: Adds the Event Type with a handler index to associate it with
* Parameters: R_Message_Type rmt -  message type
*				int e_id - the idex of the message handler that goes with it
* Returns: void
*/
void TControl::addEventID(R_Message_Type rmt, int e_id)
{
	EventTypeID et;
	et.eventID = e_id;
	et.eventType = rmt;
	eventList.push_back(et);
}




// Messages TControls can expect to get from the Central Program (or when they are part of a running TAP

/*
* Method: TControl - OnRButtonUp
* Purpose: Allows Control to catch the Right Mouse button release event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;

	for (int c = 0; c < children.Count(); c++)
	{
		children.ElementAt(c)->OnLButtonUp(nFlags, point, mOut,eventAr);
	}
	if (!isContained(&point, &location))
	{
		mState = normal;
		return;
	}
	if (*mOut == positiveContinue)
	{
		if (mState == mouseRClick)
		{
			*mOut = positiveContinueUpdate;
			if (hasEvent(On_Right_Release))
			{
				// Set args
				resetArgs();
				args.eventType = On_Right_Release;
				args.point = point;
				args.methodID = getEventID(On_Right_Release);
				args.isClick = true;
				args.isLeftClick = false;
				args.control = this;

				eventAr.push_back(EventID_Cred{ On_Right_Release, this });
			}
		}
	}
	mState = mouseHover;
}

/*
* Method: TControl - OnLButtonDown
* Purpose: Allows Control to catch the Left Mouse Button Down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;

	if (!isContained(&point, &location))
	{
		if (mState != normal)
		{
			mState = normal;
			*mOut = negativeUpdate;
		}
		else
			*mOut = negative;
		return;
	}

	for (int c = 0; c < children.Count();c++)
	{
		if (children.ElementAt(c).Get())
			children.ElementAt(c)->OnLButtonDown(nFlags, point, mOut,eventAr);
	}

	if (*mOut == positiveOverride || *mOut == positiveOverrideUpdate)
	{
		mState = normal;
		return;
	}

	if (mState != mouseLClick)
	{
		if (overrideParent)
			*mOut = positiveOverrideUpdate;
		else
			*mOut = positiveContinueUpdate;
	}
	else if (*mOut != positiveContinueUpdate && *mOut != positiveOverrideUpdate && *mOut != negativeUpdate)
	{
		if (overrideParent)
			*mOut = positiveOverride;
		else
			*mOut = positiveContinue;
	}
	mState = mouseLClick;
	
	if (hasEvent(On_Click))
	{
		// Set args
		resetArgs();
		args.eventType = On_Click;
		args.point = point;
		args.methodID = getEventID(On_Click);
		args.isClick = true;
		args.isLeftClick = true;
		args.control = this;

		eventAr.push_back(EventID_Cred{ On_Click,this });
	}




	/*
	if (!isContained(&point, &location))
	{
		if (mState != normal)
		{
			mState = normal;
			*mOut = negativeUpdate;
		}
		else
			*mOut = negative;
		return;
	}
	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->child->OnLButtonDown(nFlags, point, mOut);
		if (*mOut == negative)
			continue;
		if (*mOut == positiveOverride || *mOut == positiveOverrideUpdate)
			return;
	}
	if (*mOut == positiveContinue)
	{
		if (mState != mouseLClick)
			*mOut = positiveContinueUpdate;
		
	}
	mState = mouseLClick;*/
}

/*
* Method: TControl - OnRButtonDown
* Purpose: Allows Control to catch the Right Mouse button down event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnRButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;


	if (!isContained(&point, &location))
	{
		if (mState != normal)
		{
			mState = normal;
			*mOut = negativeUpdate;
		}
		else
			*mOut = negative;
		return;
	}
	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->OnLButtonDown(nFlags, point, mOut,eventAr);
		if (*mOut == negative)
			continue;
		if (*mOut == positiveOverride || *mOut == positiveOverrideUpdate)
			return;
	}
	if (*mOut == positiveContinue || *mOut == positiveContinueUpdate)
	{
		if (mState != mouseRClick)
			*mOut = positiveContinueUpdate;
		
	}
	mState = mouseRClick;

	if (hasEvent(On_Right_Click))
	{	
		// Set args
		resetArgs();
		args.eventType = On_Right_Click;
		args.point = point;
		args.methodID = getEventID(On_Right_Click);
		args.isClick = true;
		args.isLeftClick = false;
		args.control = this;
		eventAr.push_back(EventID_Cred{ On_Right_Click,this });
	}

	if (contextMenu)
	{
		contextMenu->Show(appear_onRightClick);
	}

}

/*
* Method: TControl - OnMouseMove
* Purpose: Allows Controls to catch the Mouse Move event and deduce if the cursor has hovered over it
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;

	if (!isContained(&point, &location))
	{
		if (mState != normal)
		{
			mState = normal;
			*mOut = negativeUpdate;
		}
		else
			*mOut = negative;
		return;
	}

	*mOut = positiveContinueUpdate;

	for (int c = 0; c < children.Count();c++)
	{
		if (children.ElementAt(c).Get())
			children.ElementAt(c)->OnMouseMove(nFlags, point, mOut, eventAr);
	}

	if (*mOut == positiveOverride || *mOut == positiveOverrideUpdate)
	{
		mState = normal;
		return;
	}

	if (mState != mouseHover)
	{
		if (overrideParent)
			*mOut = positiveOverrideUpdate;
		else
			*mOut = positiveContinueUpdate;
	}
	else if (*mOut != positiveContinueUpdate && *mOut != positiveOverrideUpdate && *mOut != negativeUpdate)
	{
		if (overrideParent)
			*mOut = positiveOverride;
		else
			*mOut = positiveContinue;
	}
	mState = mouseHover;

	if (hasEvent(On_Hover))
	{
		// Set args
		resetArgs();
		args.eventType = On_Hover;
		args.point = point;
		args.methodID = getEventID(On_Hover);
		args.control = this;
		eventAr.push_back(EventID_Cred{ On_Hover,this });
	}
}

/*
* Method: TControl - OnContextMenu
* Purpose: Allows Anaface to catch onContextMenu calls from MFC
* Parameters: CWnd* pWnd - The window involved
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
* Note: UNUSED - Anaface's own Context Menu system is preferable

afx_msg void TControl::OnContextMenu(CWnd* pWnd, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;
}*/

/*
* Method: TControl - OnLButtonDblClk
* Purpose: Allows control to catch the DOuble Click event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;
}

/*
* Method: TControl - OnLButtonUp
* Purpose: Allows control to catch the Left Button Up event and act accordingly
* Parameters: UINT nFlags - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg void TControl::OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return;

	/*if (!parent.Get())
	{
		for (int c = 0; c < TextList.Size(); c++)
		{
			if (TextList[c])
				TextList[c]->OnLButtonUp(nFlags, point, mOut,eventAr);
		}
	}*/


	for (int c = 0; c < children.Count(); c++)
	{
		TControl* tc_temp = children.ElementAt(c).Get();
		if(tc_temp)
		tc_temp->OnLButtonUp(nFlags, point, mOut,eventAr);
		if (*mOut != negative && *mOut != negativeUpdate)
			break;
	}
	if (!isContained(&point, &location))
	{
		mState = normal;
		*mOut = negative;
		return;
	}

	*mOut = positiveContinue;
	if (mState == mouseLClick)
		*mOut = positiveContinueUpdate;

	mState = mouseHover;

	if (hasEvent(On_Click_Release))
	{
		// Set args
		resetArgs();
		args.eventType = On_Click_Release;
		args.point = point;
		args.methodID = getEventID(On_Click_Release);
		args.isClick = true;
		args.isLeftClick = true;
		args.control = this;
		eventAr.push_back(EventID_Cred{ On_Click_Release, this });
	}
}

/*
* Method: TControl - OnChar
* Purpose: Allows Controls to repond to character input
* Parameters: bool fromChar - can be called either from on Key Down or OnChar
*				UINT nChar - The ID of the character that was pressed
*				UINT nRepCnt - how many times the character was processed for this event
*				UINT nFlags - flags provided by MFC's Message system, not used
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*/
afx_msg bool TControl::OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr)
{
	if (!isActive)
		return false;

	TControl* tcCall = NULL;
	for (int c = 0; c < children.Count(); c++)
	{
		if (children.ElementAt(c).Get())
			tcCall = children.ElementAt(c).Get();

		if (tcCall && tcCall->OnChar(fromChar, nChar, nRepCnt, nFlags, mOut, eventAr))
		{
			if (hasEvent(On_Char))
			{
				resetArgs();
				args.eventType = On_Char;
				args.control = this;
				args.methodID = getEventID(On_Char);
				args.type = static_cast<WCHAR>(LOWORD(nChar));
				args.control = this;
				eventAr.push_back(EventID_Cred{ On_Char, this });
			}

			return true;
		}
		
		
	}
	return false;
}

// Messages TControls can get while under construction in the Builder

/*
* Method: TControl - Builder_OnLButtonUp
* Purpose: Allows special calls from the Builder to know when the control is no longer being dragged
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
* Returns: void 
*/
afx_msg void TControl::Builder_OnLButtonUp(UINT flags, TPoint point, TControl** mOut)
{
	for (int c = 0; c < children.Count();c++)
	{
		if (!children.ElementAt(c).Get())
			continue;
		TControl* tc = children.ElementAt(c).Get();
		tc->Builder_OnLButtonUp(flags, point, mOut);
	}
	rightBorder = leftBorder = topBorder = bottomBorder = onClickFocus = false;
}

/*
* Method: TControl - Builder_OnLButtonDown
* Purpose: Allows Controls to tell if a use of the Builder is planning to move them around
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
*				messageOutput* o - Allows control to keep track of how other controls respond to the call
* Returns: void
*/
afx_msg void TControl::Builder_OnLButtonDown(UINT flags, TPoint point, TControl** mOut, messageOutput* o)
{
	for (int c = 0; c < children.Count();c++)
	{
		if (!children.ElementAt(c).Get()|| !children.ElementAt(c).Get())
			continue;
		TControl* tc = children.ElementAt(c).Get();
		tc->Builder_OnLButtonDown(flags, point, mOut, o);
		if (*o != negative && *o != negativeUpdate)
		{
			onFocus = onClickFocus = leftBorder = rightBorder = topBorder = bottomBorder = false; // other focus flags rendered false by LButtonUp method
			return;
		}
	}
	
	if (isContained(&point, &snip))
	{
		*o = positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		*mOut = (this);
	}
	else
	{
		onClickFocus = onFocus = leftBorder = rightBorder = topBorder = bottomBorder = false;
		*o = negativeUpdate;
	}
	if (abs((double)point.x - (double)snip.left) < 5)
	{
		*o = positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		leftBorder = true;
		rightBorder = false;
		topBorder = false;
		bottomBorder = false;
		return;
	}
	if (abs((double)point.x - (double)snip.right) < 5)
	{
		*o = positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = true;
		leftBorder = false;
		topBorder = false;
		bottomBorder = false;
		return;
	}
	if (abs((double)point.y - (double)snip.top) < 5)
	{
		*o = positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = false;
		leftBorder = false;
		topBorder = true;
		bottomBorder = false;
		return;
	}
	if (abs((double)point.y - (double)snip.bottom) < 5)
	{
		*o = positiveOverrideUpdate;
		onClickFocus = true;
		onFocus = true;
		rightBorder = false;
		leftBorder = false;
		topBorder = false;
		bottomBorder = true;
		return;
	}

}

/*
* Method: TControl - Builder_OnMouseMove
* Purpose: Allows controls to know when they are being moved
* Parameters: UINT flags - Flags provided by MFC, not used
*				TPoint point - the Point where event occured
*				TrecPointer<TControl>& mOut - allows control to mark itself - not used in this method
*				const RECT& bounds - bounds of the parent
*				messageOutput* o - Allows control to keep track of how other controls respond to the call
* Returns: void
*/
afx_msg void TControl::Builder_OnMouseMove(UINT flags, TPoint cp, TControl** mOut, const RECT& bounds, messageOutput* o)
{
	if (!onClickFocus)
	{
		TControl* point;
		for (int c = 0; c < children.Count();c++)
		{
			point = children.ElementAt(c).Get();
			if (point)
			{
				//const TControlHolder** hold = &PointerCase;
				point->Builder_OnMouseMove(flags, cp, mOut, location, o);
			}
			if (*o == negative && *o == negativeUpdate) // then it is positive
				return;
		}
	}

	// Okay, this Control Does have focus
	if (isContained(&cp, &bounds))
	{
		if (leftBorder)
		{
			margin.left = abs(bounds.left - cp.x);


			snip.left = cp.x;
			if (snip.left > snip.right)
				switchLongs(snip.left, snip.right);
			*o = positiveOverrideUpdate;
			if (!hScroll)
			{
				location.left = snip.left;
				location.right = snip.right;
			}

			if (content1.Get())
			{
				content1->location.left = location.left;
				content1->snip.left = snip.left;
			}
			if (content2.Get())
			{
				content2->location.left = location.left;
				content2->snip.left = snip.left;
			}
			if (content3.Get())
			{
				content3->location.left = location.left;
				content3->snip.left = snip.left;
			}
			if (border1.Get())
			{
				border1->loci.left = location.left;
			}
			if (border2.Get())
			{
				border2->loci.left = location.left;
			}
			if (border3.Get())
			{
				border3->loci.left = location.left;
			}
			

			return;
		}
		if (rightBorder)
		{

			margin.right = abs(bounds.right - cp.x);

			snip.right = cp.x;
			if (snip.left > snip.right)
				switchLongs(snip.left, snip.right);
			*o = positiveOverrideUpdate;
			if (!hScroll)
			{
				location.left = snip.left;
				location.right = snip.right;
			}

			if (content1.Get())
			{
				content1->location.right = location.right;
				content1->snip.right = snip.right;
			}
			if (content2.Get())
			{
				content2->location.right = location.right;
				content2->snip.right = snip.right;
			}
			if (content3.Get())
			{
				content3->location.right = location.right;
				content3->snip.right = snip.right;
			}
			if (border1.Get())
			{
				border1->loci.right = location.right;
			}
			if (border2.Get())
			{
				border2->loci.right = location.right;
			}
			if (border3.Get())
			{
				border3->loci.right = location.right;
			}


			return;
		}
		if (topBorder)
		{
			margin.top = abs(bounds.top - cp.y);


			snip.top = cp.y;
			if (snip.top > snip.bottom)
				switchLongs(snip.top, snip.bottom);
			if (!vScroll)
			{
				location.bottom = snip.bottom;
				location.top = snip.top;
			}
			*o = positiveOverrideUpdate;

			if (content1.Get())
			{
				content1->location.top = location.top;
				content1->snip.top = snip.top;
			}
			if (content2.Get())
			{
				content2->location.top = location.top;
				content2->snip.top = snip.top;
			}
			if (content3.Get())
			{
				content3->location.top = location.top;
				content3->snip.top = snip.top;
			}
			if (border1.Get())
			{
				border1->loci.top = location.top;
			}
			if (border2.Get())
			{
				border2->loci.top = location.top;
			}
			if (border3.Get())
			{
				border3->loci.top = location.top;
			}

			return;
		}
		if (bottomBorder)
		{
			margin.bottom = abs(bounds.bottom - cp.y);

			snip.bottom = cp.y;
			if (snip.top > snip.bottom)
				switchLongs(snip.top, snip.bottom);
			if (!vScroll)
			{
				location.bottom = snip.bottom;
				location.top = snip.top;
			}
			*o = positiveOverrideUpdate;


			if (content1.Get())
			{
				content1->location.bottom = location.bottom;
				content1->snip.bottom = snip.bottom;
			}
			if (content2.Get())
			{
				content2->location.bottom = location.bottom;
				content2->snip.bottom = snip.bottom;
			}
			if (content3.Get())
			{
				content3->location.bottom = location.bottom;
				content3->snip.bottom = snip.bottom;
			}
			if (border1.Get())
			{
				border1->loci.bottom = location.bottom;
			}
			if (border2.Get())
			{
				border2->loci.bottom = location.bottom;
			}
			if (border3.Get())
			{
				border3->loci.bottom = location.bottom;
			}

			return;
		}
	} // end if "isContained" if
}

/*
* Method: TControl - Remove_Builder_Click_Focus
* Purpose: Removes the burden of being dragged by the Builder
* Parameters: void
* Returns: void
*/
void TControl::Remove_Builder_Click_Focus()
{
	if (resistFocusRemoval)
		resistFocusRemoval = false;
	else
		onFocus = false;
}

/*
* Method: TControl - setActive
* Purpose: Enables controls to be either active (default) or inactive (they don't draw or respond to events)
* Parameters: bool act - whether control should be active or not
* Returns: void
*/
void TControl::setActive(bool act)
{
	isActive = act;
}

/*
* Method: TControl - getEventArgs
* Purpose: Allows Action Methods to retrieve Event Arguements before calling the target Event Handler
* Parameters: void
* Returns: EventArgs - structure of relevent arguements to provide an Event Handler
*/
EventArgs TControl::getEventArgs()
{
	return args;
}

/*
* Method:  (TBorder) (Constructor)
* Purpose: Sets up the Border for a given TControl
* Parameters: TrecComPointer<ID2D1RenderTarget> rtp - Smart Pointer to the Render target to draw against
*				TControl* tc - the TControl to which the TBorder is a party to 
* Returns: void
*/
TBorder::TBorder(TrecComPointer<ID2D1RenderTarget>rtp, TControl*tc)
{
	rt = rtp;
	cap = tc;

	thickness = 1.0;
	color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;
	if (rt.Get())
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder br;
		rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), br.GetPointerAddress());
		BuilderFocusBrush = br.Extract();
	}
	else
		BuilderFocusBrush.Nullify();

	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method:  (TBorder) (Constructor)
* Purpose: Sets up the border for a given TControl based off of an existing Border
* Parameters: TrecPointer<TBorder> & rBord - Reference to the Border to copy
*				TControl* tc_holder - the Control to work for
* Returns: void
*/
TBorder::TBorder(TrecPointer<TBorder> & rBord,TControl* tc_holder)
{	
	if (!rBord.Get())return;

	thickness = rBord->thickness;
	style = rBord->style;
	color =rBord->color;
	color2 = rBord->color2;
	rt = rBord->rt;

	cap = tc_holder;
	shape = rBord->shape;
	secondColor = rBord->secondColor;

	linearProp = rBord->linearProp;
	radialProp = rBord->radialProp;

	gradients[0] = rBord->gradients[0];
	gradients[1] = rBord->gradients[1];
	useRadial = rBord->useRadial;

	// Alternate shapes
	loci = rBord->loci;
	circle = rBord->circle;
	roundedRect = rBord->roundedRect;

	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder(TrecPointer<TBorder>&,TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TBorder) (Constructor)
* Purpose: Sets up a basic TBorder
* Parameters: void
* Returns: void
*/
TBorder::TBorder()
{
	thickness = 1.0;
	color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;
	gradients->color = D2D1::ColorF(0.0f,0.0f,0.0f,1.0f);

	TString logMessage;
	logMessage.Format(L"CREATE %p TBorder()", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TBorder) (Destructor)
* Purpose: Cleans up the TBorder upond deletion
* Parameters: void
* Returns: void
*/
TBorder::~TBorder()
{
	brush.Delete();
	
	image.Delete();
	stopColl.Delete();
	BuilderFocusBrush.Delete();
	bitBrush.Delete();

	TString logMessage;
	logMessage.Format(L"DELETE %p TBorder", this);

	Log(lt_memory, logMessage);
}

/*
* Method: TBorder - onCreate
* Purpose: Sets up a basic Rectangular border
* Parameters: RECT location - the location the Border should draw on
* Returns: bool - success
*/
bool TBorder::onCreate(RECT location)
{
	if (!rt.Get())
		return false;

	brush.Nullify();

	stopColl.Nullify();
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopCollRaw;
	shape = T_Rect;
	loci.bottom = location.bottom;
	loci.left = location.left;
	loci.right = location.right;
	loci.top = location.top;

	if (secondColor)
	{

		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, stopCollRaw.GetPointerAddress());
		stopColl = stopCollRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom),
				location.right-location.left,location.bottom-location.top),
				stopColl.Get(), radBrush.GetPointerAddress());
			
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom)),
				stopColl.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}
		
	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}


	return true;
}

/*
* Method: TBorder - onCreate
* Purpose: Sets up a border optimized for dawing ovals
* Parameters: D2D1_ELLIPSE e - the oval to draw
* Returns: bool - success
*/
bool TBorder::onCreate(D2D1_ELLIPSE e)
{


	shape = T_Ellipse;
	if (!rt.Get())
		return false;
	loci.left = e.point.x - e.radiusX;
	loci.right = e.point.x + e.radiusX;
	loci.top = e.point.y - e.radiusY;
	loci.bottom = e.point.y + e.radiusY;

	brush.Nullify();

	stopColl.Nullify();
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopCollRaw;
	if (secondColor)
	{

		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, stopCollRaw.GetPointerAddress());
		stopColl = stopCollRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom),
				loci.right - loci.left, loci.bottom - loci.top),
				stopColl.Get(), radBrush.GetPointerAddress());

			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom)),
				stopColl.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}

	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	circle = e;
	return true;
}

/*
* Method: TBorder - onCreate
* Purpose: Sets up a Border that has a rectangle with rounded corners
* Parameters: D2D1_ROUNDED_RECT rr - the border to draw
* Returns: bool - success
*/
bool TBorder::onCreate(D2D1_ROUNDED_RECT rr)
{

	shape = T_Rounded_Rect;
	if (!rt.Get())
		return false;
	roundedRect = rr;

	loci.left = rr.rect.left;
	loci.right = rr.rect.right;
	loci.top = rr.rect.top;
	loci.bottom = rr.rect.bottom;

	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopCollRaw;
	if (secondColor)
	{

		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, stopCollRaw.GetPointerAddress());
		stopColl = stopCollRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom),
				loci.right - loci.left, loci.bottom - loci.top),
				stopColl.Get(), radBrush.GetPointerAddress());

			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(loci.left, loci.top),
				D2D1::Point2F(loci.right, loci.bottom)),
				stopColl.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}

	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	return true;
}


/*
* Method: TBorder - onDraw
* Purpose: Draws the border
* Parameters: void
* Returns: void
*/
void TBorder::onDraw(RECT& loc, RECT& snip)
{
	
	//TrecPointer<TControl> cap = TrecPointer<TControl>(cap);
	if (rt.Get() && brush.Get() && cap)
	{
		D2D1_RECT_F f_loc, f_snip;
		convertCRectToD2DRect(&snip, &f_snip);
		convertCRectToD2DRect(&loc, &f_loc);
		bool drawRect = true;
		//if (snip.left > loci.left || snip.top > loci.top ||
			//snip.right < loci.right || snip.bottom < loci.bottom)
		//	drawRect = false;


		if (loc == snip)
		{
			switch (shape)
			{
			case T_Rect:
				rt->DrawRectangle(&loci, brush.Get(), thickness);
				break;
			case T_Rounded_Rect:

				roundedRect.rect = f_snip;
				rt->DrawRoundedRectangle(&roundedRect, brush.Get(), thickness);
				break;
			case T_Ellipse:

				circle.point.x = (f_snip.right + f_snip.left) / 2;
				circle.point.y = (f_snip.bottom + f_snip.top) / 2;
				circle.radiusX = f_snip.right - f_snip.left;
				circle.radiusY = f_snip.bottom - f_snip.top;
				rt->DrawEllipse(&circle, brush.Get(), thickness);
				break;
			case T_Custom_shape:
				break;
			}
		}
		else
		{
			if (snip.left >= loci.left)
				rt->DrawLine(D2D1::Point2F(snip.left, snip.top), D2D1::Point2F(snip.left, snip.bottom), brush.Get(), thickness);
			if (snip.top >= loci.top)
				rt->DrawLine(D2D1::Point2F(snip.left, snip.top), D2D1::Point2F(snip.right, snip.top), brush.Get(), thickness);
			if (snip.right <= loci.right)
				rt->DrawLine(D2D1::Point2F(snip.right, snip.top), D2D1::Point2F(snip.right, snip.bottom), brush.Get(), thickness);
			if (snip.bottom >= loci.bottom)
				rt->DrawLine(D2D1::Point2F(snip.left, snip.bottom), D2D1::Point2F(snip.right, snip.bottom), brush.Get(), thickness);
		}
		
		/*if (drawRect)
		{
			if (cap->onFocus && BuilderFocusBrush.Get())
			{
				switch (shape)
				{
				case T_Rect:
					rt->DrawRectangle(&loci, BuilderFocusBrush.Get(), 10.0);
					break;
				case T_Rounded_Rect:
					rt->DrawRoundedRectangle(&roundedRect, BuilderFocusBrush.Get(), 10.0);
					break;
				case T_Ellipse:
					rt->DrawEllipse(&circle, BuilderFocusBrush.Get(), 10.0);
					break;
				case T_Custom_shape:
					break;
				}
			}
			else
			{
				switch (shape)
				{
				case T_Rect:

					rt->DrawRectangle(&loci, brush.Get(), thickness);
					break;
				case T_Rounded_Rect:
					rt->DrawRoundedRectangle(&roundedRect, brush.Get(), thickness);
					break;
				case T_Ellipse:
					rt->DrawEllipse(&circle, brush.Get(), thickness);
					break;
				case T_Custom_shape:
					break;
				}
			}
		}
		else
		{
			
		}*/


	}
}

/*
* Method: TBorder - setColor
* Purpose: Allows the color of the border to change even after creation
* Parameters: D2D1::ColorF cf -  the color to set the border to
* Returns: void
*/
void TBorder::setColor(D2D1::ColorF cf)
{
	color = cf;
	if (brush.Get())
	{
		ID2D1SolidColorBrush* sb = dynamic_cast<ID2D1SolidColorBrush*>(brush.Get());
		if(sb)
			sb->SetColor(cf);
	}
}

/*
* Method: TBorder - getColor
* Purpose: Allows outside software to check the current color of the border
* Parameters: void
* Returns: D2D1::ColorF - the current color
*/
D2D1::ColorF TBorder::getColor()
{
	return color;
}

/*
* Method: TBorder - setThickness
* Purpose: Allows the thickness to be updated after creation
* Parameters: float f - the thickness to set to
* Returns: void
*/
void TBorder::setThickness(float f)
{
	thickness = f;
}

/*
* Method: TBorder - setOpaquency
* Purpose: Allows the Opaquency to be updated after creation
* Parameters: float f - the opaquency to set to (no upadte occurs if value is out of range)
* Returns: void 
*/
void TBorder::setOpaquency(float f)
{
	if (f > 1.0000000 || f < 0.0f)
		return;
	color.a = f;
}

/*
* Method: TBorder - getThickness
* Purpose: Allows the current thickness to be seen
* Parameters: void
* Returns: float - the current thickness
*/
float TBorder::getThickness()
{
	return thickness;
}

/*
* Method: TBorder - getLocation
* Purpose: Retrieves the Location of the current border
* Parameters: void
* Returns: RECT - the current location
*/
RECT TBorder::getLocation()
{
	return convertD2DRectToRECT(loci);
}

/*
* Method: TBorder - ShiftHorizontal
* Purpose: Moves the Border left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the Border and which direction
* Returns: void
*/
void TBorder::ShiftHorizontal(int degrees)
{
	loci.left += degrees;
	loci.right += degrees;
	if (!isSnipZero(snip))
	{
		snip.left += degrees;
		snip.right += degrees;
	}
}

/*
* Method: TBorder - ShiftVertical
* Purpose:Moves the Border up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the border and which direction
* Returns: void
*/
void TBorder::ShiftVertical(int degrees)
{
	loci.top += degrees;
	loci.bottom += degrees;
	if (!isSnipZero(snip))
	{
		snip.top += degrees;
		snip.bottom += degrees;
	}
}

/*
* Method: TBorder - storeInTML
* Purpose: Allows te Border to save itself in the Anaface TML format
* Parameters: CArchive * ar - the File object to save to
*				int childLevel - the child level (in TML this lets the Border know how many dashes to write for each attribute)
*				messageState ms - which Border the current Border is (the Border itself does not know)
* Returns:
*/
int TBorder::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	TString writable;
	resetAttributeString(&writable, childLevel +1);
	TString val;
	val.Format(L"%f", thickness);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|BorderThickness:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverBorderThickness:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickBorderThickness:");
		break;
	}
	
	ar->WriteString(val);
	ar->WriteString("\n");
	val = convertD2DColorToString(color);
	//ar->WriteString(writable);

	resetAttributeString(&writable, childLevel + 1);

	switch (ms)
	{
	case normal:
		writable.Append(L"|BorderColor:");
		break;
	case mouseHover:
		writable.Append(L"|HoverBorderColor:");
		break;
	case mouseLClick:
		writable.Append(L"|ClickBorderColor:");
	}
	ar->WriteString(writable);

	ar->WriteString(val);
	ar->WriteString(L"\n");
	return 0;
}

/*
* Method: TBorder - BreakShared
* Purpose: Breaks the link between the Border and the Parent Control, allowing deletion
* Parameters: void
* Returns: void
* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
*/
void TBorder::BreakShared()
{
	cap = nullptr;
}

/*
* Method: (TText) (Contstructor)
* Purpose: Set up a new text element with the control and render target set up
* Parameters: TrecComPointer<ID2D1RenderTarget> rtp - Smart Pointer to the Render Target to draw to
*				TControl* tc - the TControl to work for
* Returns: void
*/
TText::TText(TrecComPointer<ID2D1RenderTarget>rtp,TControl* tc)
{
	rt = rtp;
	cap = tc;
	fontSize = 12.0;
	locale.Set( L"en-us" );
	font.Set(L"Ariel" );
	color = D2D1::ColorF(D2D1::ColorF::Black);
	fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	horizontalAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
	verticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	textLength = 0;
	
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;

	TString logMessage;
	logMessage.Format(L"CREATE %p TText(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TText) (Contstructor)
* Purpose: Sets up a Text element based off of an existing Text element
* Parameters: TrecPointer<TText> & rText -  the text element to copy
*				TControl* tc_holder - The TControl to work for
* Returns: void
*/
TText::TText(TrecPointer<TText> & rText, TControl* tc_holder)
{
	if (!rText.Get())
		return;

	rt = rText->rt;
	writeFact = rText->writeFact;

	bounds = rText->bounds;
	setCaption(rText->text);

	fontWeight = rText->fontWeight;
	fontStyle = rText->fontStyle;
	fontStretch = rText->fontStretch;

	verticalAlignment = rText->verticalAlignment;
	horizontalAlignment = rText->horizontalAlignment;
	
	fontSize = rText->fontSize;
	setLocale(rText->locale);
	setFont(rText->font);
	color = rText->color;
	cap = tc_holder;

	secondColor = rText->secondColor;
	color2 = rText->color2;
	linearProp = rText->linearProp;
	radialProp = rText->radialProp;
	gradients[0] = rText->gradients[0];
	gradients[1] = rText->gradients[1];
	useRadial = rText->useRadial;
	gradStop = rText->gradStop;

	fontLayout = rText->fontLayout;
	format = rText->format;


	TString logMessage;
	logMessage.Format(L"CREATE %p TText(TrecPointer<TText> &, TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TText) (Contstructor)
* Purpose: Sets up a Basic Clean Slate Text element
* Parameters: void
* Returns: void
*/
TText::TText()
{
	fontSize = 12.0;
	locale.Set( L"en-us" );
	font.Set(L"Ariel" );
	color = D2D1::ColorF(D2D1::ColorF::Black);
	fontWeight = DWRITE_FONT_WEIGHT_REGULAR;
	fontStyle = DWRITE_FONT_STYLE_NORMAL;
	fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	textLength = 0;


	horizontalAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;
	verticalAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;

	TString logMessage;
	logMessage.Format(L"CREATE %p TText()", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TText) (Destructor)
* Purpose: Cleans up the Content
* Parameters: void
* Returns: void
*/
TText::~TText()
{

	TString logMessage;
	logMessage.Format(L"DELETE %p TText - %S", this, this->text);

	Log(lt_memory, logMessage);

	writeFact.Nullify();


	format.Delete();

	fontLayout.Delete();

	penBrush.Delete();
	
}

/*
* Method: TText - onCreate
* Purpose: Sets up the Text element
* Parameters: RECT loc - the screen space to use
* Returns: int success (0 means no error)
*/
int TText::onCreate(RECT loc)
{

	if (rt.Get() == NULL)
		return 1;


	writeFact.Nullify();
	TrecComPointer<IDWriteFactory>::TrecComHolder rawFact;

	HRESULT results = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(rawFact.GetPointerAddress()));

	if (!SUCCEEDED(results))
		return 2;

	writeFact = rawFact.Extract();

	if(fontSize <= 0.0)
		fontSize = 12.0;

	format.Nullify();
	TrecComPointer<IDWriteTextFormat>::TrecComHolder wtf;
	results = writeFact->CreateTextFormat(
		font.GetConstantBuffer(),
		NULL,
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		locale.GetConstantBuffer(),
		wtf.GetPointerAddress());
	format = wtf.Extract();
	
	if (!SUCCEEDED(results))
		return 3;

	results = format->SetTextAlignment(horizontalAlignment);
	if (SUCCEEDED(results))
		results = format->SetParagraphAlignment(verticalAlignment);

	if (!SUCCEEDED(results))
		return 4;

	bounds.bottom = loc.bottom;
	bounds.left = loc.left;
	bounds.right = loc.right;
	bounds.top = loc.top;


	penBrush.Nullify();
	gradStop.Nullify();
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder gradStopRaw;
	if (secondColor)
	{
		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, gradStopRaw.GetPointerAddress());
		gradStop = gradStopRaw.Extract();
		
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(bounds.left, bounds.top),
				D2D1::Point2F(bounds.right, bounds.bottom),
				bounds.right - bounds.left, bounds.bottom - bounds.top),
				gradStop.Get(), radBrush.GetPointerAddress());
			penBrush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(bounds.left, bounds.top),
				D2D1::Point2F(bounds.right, bounds.bottom)),
				gradStop.Get(), linBrush.GetPointerAddress());
			penBrush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}
		
	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		penBrush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	if (!SUCCEEDED(results))
		return 5;

	
	TrecComPointer<IDWriteTextLayout>::TrecComHolder wtl;
	results = writeFact->CreateTextLayout(text.GetConstantBuffer(), text.GetSize(), format.Get(), loc.right - loc.left, loc.bottom - loc.top, wtl.GetPointerAddress());
	fontLayout = wtl.Extract();

	if(SUCCEEDED(results))
	return 0;
	else return 6;
}

/*
* Method: TText - reCreateLayout
* Purpose: Resets the Text Layout upon changes made to the format, text, or location
* Parameters: void 
* Returns: void
* Note: Call this method whenever you make a change to the format of the control, the text, or the location,
*		Otherwise, your changes might not be seen 
*/
void TText::reCreateLayout()
{
	reCreateLayout(text);
}

void TText::reCreateLayout(TString & str)
{
	fontLayout.Nullify();
	if (text.GetSize() && format.Get())
	{
		TrecComPointer<IDWriteTextLayout>::TrecComHolder wfl;
		writeFact->CreateTextLayout(str.GetConstantBuffer(), str.GetSize(), format.Get(), bounds.right - bounds.left, bounds.bottom - bounds.top, wfl.GetPointerAddress());
		fontLayout = wfl.Extract();
	}
}

/*
* Method: TText - onDraw
* Purpose: Draws the text
* Parameters: RECT r - the area to draw
* Returns: bool success
*/
bool TText::onDraw(RECT& loc, RECT& snip, TObject* obj)
{
	if (!penBrush.Get() || !rt.Get())
		return false;
	D2D1_RECT_F snipF = { 0.0f,0.0f,0.0f,0.0f };
	convertCRectToD2DRect(&loc, &snipF);
	if (bounds != snipF)
		bounds = snipF;
	// TString print;
	if (obj && text.GetSize() > 0 && text.GetAt(0) == L'{' && text.GetAt(text.GetSize() - 1) == L'}')
	{
		TString print = obj->getVariableValueStr(text.SubString(1, text.GetSize() - 1));
		reCreateLayout(print);
	}
	
	if (convertCRectToD2DRect(&snip, &snipF))
	{
		bounds = snipF;
		if (fontLayout.Get())
		{
			fontLayout->SetMaxHeight(snipF.bottom - snipF.top);
			fontLayout->SetMaxWidth(snipF.right - snipF.left);
			rt->DrawTextLayout(D2D1::Point2F(snipF.left, snipF.top), fontLayout.Get(), penBrush.Get());
		}
		
	}
	else
		rt->DrawTextLayout(D2D1::Point2F(snipF.left, snipF.top), fontLayout.Get(), penBrush.Get());
	/*
	rt->DrawTextW(text,
		(UINT32)wcslen(text),
		format,
		bounds,
		penBrush);*/
	return true;
}

/*
* Method: TText - setColor
* Purpose: Updates the color of the text after the initial creation
* Parameters: D2D1::ColorF cf - the color to set the text to
* Returns: void
*/
void TText::setColor(D2D1::ColorF cf)
{
	color = cf;
	if (penBrush.Get())
	{
		ID2D1SolidColorBrush* sb = dynamic_cast<ID2D1SolidColorBrush*>(penBrush.Get());
		if (sb)
			sb->SetColor(cf);
	}
}

/*
* Method: TText - getColor
* Purpose: Retrieves the current color of the text
* Parameters: void
* Returns: D2D1::ColorF - the current color of the Text
*/
D2D1::ColorF TText::getColor()
{
	return color;
}

/*
* Method: TText - setNewFont
* Purpose: Sets up the new font for the text
* Parameters: TString& pFont - String holding the new fnt to use
* Returns: bool - true (redundant return value)
*/
bool TText::setNewFont(TString& pFont)
{
	font.Set( pFont);
	RECT r = convertD2DRectToRECT(bounds);
	onCreate(r);
	reCreateLayout();
	return true;
}

/*
* Method: TText - setNewFontSize
* Purpose: Updates the size of the font to print
* Parameters: float fs - the new font size
* Returns: void
*/
void TText::setNewFontSize(float fs)
{
	fontSize = abs(fs);
	RECT r = convertD2DRectToRECT(bounds);
	onCreate(r);
	reCreateLayout();
}

/*
* Method: TText - setNewHorizontalAlignment
* Purpose: Updates the Horizontal Alignment of the text element
* Parameters: DWRITE_TEXT_ALIGNMENT ha - the new horizontal alignment
* Returns: void
*/
void TText::setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT ha)
{
	horizontalAlignment = ha;
	if (format.Get())
	{
		format->SetTextAlignment(ha);
		reCreateLayout();
	}
}

/*
* Method: TText - setNewVerticalAlignment
* Purpose: Updates the Vertical Alignment of the Text element
* Parameters: DWRITE_PARAGRAPH_ALIGNMENT va - the new vertical alignment
* Returns: void
*/
void TText::setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT va)
{
	verticalAlignment = va;
	if (format.Get())
	{
		format->SetParagraphAlignment(va);
		reCreateLayout();
	}
}

/*
* Method: TText - setOpaquency
* Purpose: Updates the opaquency of the text
* Parameters: float o - the new opequency
* Returns: bool - true if data is valid, false otherwise
*/
bool TText::setOpaquency(float o)
{
	if(o > 1.0000000000f || o < 0.0f)
	return false;
	color.a = o;
	return true;
}

void TText::setNewLocation(RECT r)
{
	if (r.bottom < r.top || r.left > r.right)
		return;

	bounds = convertRECTToD2DRectF(r);
	reCreateLayout();
}

/*
* Method: TText - getFontSize
* Purpose: Retrieves the current font size of the text
* Parameters: void
* Returns: float - the current font size
*/
float TText::getFontSize()
{
	return fontSize;
}

/*
* Method: TText - getFont
* Purpose: Retrieves the current font of the text
* Parameters: void
* Returns: TString - the current font
*/
TString TText::getFont()
{
	return font;
}

/*
* Method: TText - getCaption
* Purpose: Retrieves the new caption for the text element
* Parameters: void
* Returns: TString - the current caption
*/
TString TText::getCaption()
{
	return text;
}

/*
* Method: TText - setCaption
* Purpose: Updates the text with a new caption
* Parameters: TString& string - the new Caption to use
* Returns: void
*/
void TText::setCaption(TString& string)
{
	text.Set(string);
	reCreateLayout();
}



/*
* Method: TText - setLocale
* Purpose: Sets a new Local for the text
* Parameters: TString& loc - the String holding the new locale
* Returns: void
*/
void TText::setLocale(TString& loc)
{
	locale = loc;
	reCreateLayout();
}

/*
* Method: TText - setFont
* Purpose: Sets up the new font for the text
* Parameters: TString& fo - String holding the new fnt to use
* Returns: bool - true (redundant return value)
* Note: DEPRECIATED - us the method TText - setNewFont
*/
void TText::setFont(TString& fo)
{
	font = fo;
	reCreateLayout();
}

/*
* Method: TText - removeCaption
* Purpose: Removes the Text and sets it to zero
* Parameters: void
* Returns: void
*/
void TText::removeCaption()
{

	text.Empty();
	textLength = 0;
}

/*
* Method: TText - getHorizontalAlignment
* Purpose: Gets the Horizontal alignment of the text
* Parameters: void
* Returns: DWRITE_TEXT_ALIGNMENT - the Horizontal alignment
*/
DWRITE_TEXT_ALIGNMENT TText::getHorizontalAlignment()
{
	return horizontalAlignment;
}

/*
* Method: TText - getVerticalAlignment
* Purpose: Gets the Vertical alignment of the text
* Parameters: void
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the vetical alignment if the text
*/
DWRITE_PARAGRAPH_ALIGNMENT TText::getVerticalAlignment()
{
	return verticalAlignment;
}

/*
* Method: TText - getLength
* Purpose: Retrieves the current length of the text being held
* Parameters: void
* Returns: int - the length of the text
*/
int TText::getLength()
{
	return textLength;
}

/*
* Method: TText - getLocation
* Purpose: Retrieves the Location of the Text Control
* Parameters: void
* Returns: RECT - the location of the text
*/
RECT TText::getLocation()
{
	return convertD2DRectToRECT(bounds);
}

/*
* Method: TText - ShiftHorizontal
* Purpose: Moves the Text element left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the text and which direction
* Returns: void
*/
void TText::ShiftHorizontal(int degrees)
{
	bounds.left += degrees;
	bounds.right += degrees;
	//if (!isSnipZero(snip))
	//{
	//	snip->left += degrees;
	//	snip->right += degrees;
	//}
}

/*
* Method: TText - ShiftVertical
* Purpose:Moves the Text Element up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the text and which direction
* Returns: void
*/
void TText::ShiftVertical(int degrees)
{
	bounds.top += degrees;
	bounds.bottom += degrees;
}



/*
* Method: TText - storeInTML
* Purpose: Allows the text to store it's attributes to a TML file
* Parameters: CArchive* ar - the file system to write to
*				int childLevel - the level this control is - how many dashes to write
*				messageState ms - the version of the text it is (TText doesn't know if it is normal, hover, or click)
* Returns: int - 0
*/
int TText::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	TString writable;
	resetAttributeString(&writable, childLevel + 1);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|Caption:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverCaption:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickCaption:");
	}

	ar->WriteString(text);
	ar->WriteString(L"\n");

	ar->WriteString(writable);

	switch (ms)
	{
	case normal:
		ar->WriteString(L"|CaptionLocale:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverCaptionLocale:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickCaptionLocale:");
	}
	ar->WriteString(locale);
	ar->WriteString(L"\n");

	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|Font:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverFont:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickFont:");
	}

	ar->WriteString(font);
	ar->WriteString(L"\n");

	TString val = convertD2DColorToString(color);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|FontColor:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverFontColor:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickFontColor:");
	}

	ar->WriteString(val);
	ar->WriteString(L"\n");
	val.Format(L"%f", fontSize);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|FontSize:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverFontSize:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickFontSize:");
	}

	ar->WriteString(val);
	ar->WriteString(L"\n");
	resetAttributeString(&writable, childLevel +1);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|VerticalAlignment:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverVerticalAlignment:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickVerticalAlignment:");
	}
	switch (verticalAlignment)
	{
	case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
		ar->WriteString(L"Bottom\n");
		break;
	case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
		ar->WriteString(L"Top\n");
		break;
	default:
		ar->WriteString(L"Center\n");
	}
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|HorizontalAlignment:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverHorizontalAlignment:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickHorizontalAlignment:");
	}
	switch (horizontalAlignment)
	{
	case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
		ar->WriteString(L"Justified\n");
		break;
	case DWRITE_TEXT_ALIGNMENT_LEADING:
		ar->WriteString(L"Right\n");
		break;
	case DWRITE_TEXT_ALIGNMENT_TRAILING:
		ar->WriteString(L"Left\n");
		break;
	default:
		ar->WriteString(L"Center\n");
	}


	return 0;
}

/*
* Method: TText - BreakShared
* Purpose: Breaks the link between the Text Element and the Parent Control, allowing deletion
* Parameters: void
* Returns: void
* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
*/
void TText::BreakShared()
{
	cap = nullptr;
}

/*
* Method: (TContent) (Contstructor)
* Purpose: Sets up a TContent component based on the render target and a parent TControl
* Parameters: TrecComPointer<ID2D1RenderTarget> rtp - Smart Pointer to the Render Target to draw to
*				TControl* tc - the parent Control to work for
* Returns: void
*/
TContent::TContent(TrecComPointer<ID2D1RenderTarget>rtp, TControl* tc)
{
	rt = rtp;
	cap = tc;
	
	thickness = 1.0;
	color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	snip = location = D2D1_RECT_F{ 0,0,0,0 };
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;

	TString logMessage;
	logMessage.Format(L"CREATE %p TContent(TrecComPointer<ID2D1RenderTarget>, TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TContent) (Contstructor)
* Purpose: Sets up a blank TContent component
* Parameters: void 
* Returns: void
*/
TContent::TContent()
{
	thickness = 1.0;
	color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	snip = location = D2D1_RECT_F{ 0,0,0,0 };
	color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	secondColor = false;
	gradients[0].position = 0.0;
	gradients[1].position = 1.0f;

	TString logMessage;
	logMessage.Format(L"CREATE %p TContent()", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TContent) (Contstructor)
* Purpose: Sets up a TContent component based on an exiting content and a parent TControl
* Parameters: TrecPointer<TContent> &rCont - an exiting Content to copy from
*				TControl* tc_holder - the Control to work for
* Returns: void
*/
TContent::TContent(TrecPointer<TContent> &rCont, TControl* tc_holder)
{
	if (!rCont.Get())return;

	rt = rCont->rt;
	
	thickness =rCont->thickness;
	style = rCont->style;
	color = rCont->color;
	location = rCont->location;
	snip = rCont->snip;
	
	cap = tc_holder;
	shape = rCont->shape;
	secondColor = rCont->secondColor;
	color2 = rCont->color2;

	linearProp = rCont->linearProp;
	radialProp = rCont->radialProp;
	gradients[0] = rCont->gradients[0];
	gradients[1] = rCont->gradients[1];
	useRadial = rCont->useRadial;

	// Alternate shapes
	circle = rCont->circle;
	roundedRect = rCont->roundedRect;

	TString logMessage;
	logMessage.Format(L"CREATE %p TContent(TrecPointer<TContent> &rCont, TControl*)", this);

	Log(lt_memory, logMessage);
}

/*
* Method: (TContent) (Destructor)
* Purpose: Cleans up the Resources held by the content
* Parameters: void
* Returns: void
*/
TContent::~TContent()
{

	bitmap.Delete();

	image.Delete();
	brush.Delete();

	cropImage.Delete();

	TString logMessage;
	logMessage.Format(L"DELETE %p TContent", this);

	Log(lt_memory, logMessage);
}

/*
* Method: TContent - onCreate
* Purpose: Sets up the details of the Content with a Rectangle to draw
* Parameters: RECT l - the Location
*				RECT s - the snip, where the control can appear
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(RECT l, RECT s)
{

	location.top = l.top;
	location.left = l.left;
	location.right = l.right;
	location.bottom = l.bottom;

	convertCRectToD2DRect(&s, &snip);

	if (!rt.Get())
		return false;

	brush.Nullify();

	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder gradStopRaw ;
	if (secondColor)
	{
		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, gradStopRaw.GetPointerAddress());
		gradStop = gradStopRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom),
				location.right - location.left, location.bottom - location.top),
				gradStop.Get(), radBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom)),
				gradStop.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}
		
	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}

	if (image.Get())
	{
		TrecComPointer<ID2D1BitmapBrush>::TrecComHolder bit;
		rt->CreateBitmapBrush(image.Get(), bit.GetPointerAddress());
		bitmap = bit.Extract();
	}

	return true;
}

/*
* Method: TContent - onCreate
* Purpose: Sets up the details of the Content with an oval to draw
* Parameters: D2D1_ELLIPSE e - the oval to draw
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(D2D1_ELLIPSE e)
{

	if (!rt.Get())
		return false;
	circle = e;
	brush.Nullify();

	location.left = e.point.x - e.radiusX;
	location.right = e.point.x + e.radiusX;
	location.top = e.point.y - e.radiusY;
	location.bottom = e.point.y + e.radiusY;
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder gradStopRaw ;
	if (secondColor)
	{
		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, gradStopRaw.GetPointerAddress());
		gradStop = gradStopRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom),
				location.right - location.left, location.bottom - location.top),
				gradStop.Get(), radBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom)),
				gradStop.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}
		
	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	return true;
}

/*
* Method: TContent - onCreate
* Purpose: Sets up the details of the Content with a rounded Rectangle to draw
* Parameters: D2D1_ROUNDED_RECT rr -  the rounded rectangle to draw
* Returns: bool - success (whether the resources are prepared)
*/
bool TContent::onCreate(D2D1_ROUNDED_RECT rr)
{
	if (!rt.Get())
		return false;
	roundedRect = rr;
	brush.Nullify();

	location.left = rr.rect.left;
	location.right = rr.rect.right;
	location.top = rr.rect.top;
	location.bottom = rr.rect.bottom;

	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder gradStopRaw;
	if (secondColor)
	{
		gradients[0].color = color;
		gradients[1].color = color2;
		rt->CreateGradientStopCollection(gradients, 2, gradStopRaw.GetPointerAddress());
		gradStop = gradStopRaw.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom),
				location.right - location.left, location.bottom - location.top),
				gradStop.Get(), radBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(location.left, location.top),
				D2D1::Point2F(location.right, location.bottom)),
				gradStop.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}

	}
	else
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		rt->CreateSolidColorBrush(color, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	return true;
}

/*
* Method: TContent - ShiftHorizontal
* Purpose: Moves the content left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the content and which direction
* Returns: void
*/
void TContent::ShiftHorizontal(int degrees)
{
	location.left += degrees;
	location.right += degrees;
	if (!isSnipZero(snip))
	{
		snip.left += degrees;
		snip.right += degrees;
	}
}

/*
* Method: TContent - ShiftVertical
* Purpose:Moves the content up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the content and which direction
* Returns: void
*/
void TContent::ShiftVertical(int degrees)
{
	location.top += degrees;
	location.bottom += degrees;
	if (!isSnipZero(snip))
	{
		snip.top += degrees;
		snip.bottom += degrees;
	}
}

/*
* Method: TContent - onDraw
* Purpose: Draws the content, including a picture if one is provided
* Parameters: void
* Returns: void
*/
void TContent::onDraw(RECT& loc, RECT& snip)
{
	if (!rt.Get())
		return;

	D2D1_RECT_F f_snip;
	convertCRectToD2DRect(&snip, &f_snip);

	if (brush.Get())
	{
		switch (shape)
		{
		case T_Rect:
			rt->FillRectangle(f_snip, brush.Get());
			break;
		case T_Rounded_Rect:
			roundedRect.rect = f_snip;
			rt->FillRoundedRectangle(&roundedRect, brush.Get());

			break;
		case T_Ellipse:
			circle.point.x = (f_snip.right + f_snip.left) / 2;
			circle.point.y = (f_snip.bottom + f_snip.top) / 2;
			circle.radiusX = f_snip.right - f_snip.left;
			circle.radiusY = f_snip.bottom - f_snip.top;
			rt->FillEllipse(&circle, brush.Get());
			break;
		case T_Custom_shape:
			break;
		}
	}
	if (image.Get() && bitmap.Get() && cropImage.Get())
	{
		D2D1_RECT_F f_loc;
		convertCRectToD2DRect(&loc, &f_loc);
		if(loc == snip)
			rt->DrawBitmap(image.Get(), f_snip);
		else
		{
			D2D1_POINT_2U point{ 0,0 };
			D2D1_RECT_U cropSnip;
			cropSnip.bottom = abs(f_loc.bottom - f_snip.bottom);
			cropSnip.left = abs(f_snip.left - f_loc.left);
			cropSnip.right = abs(f_loc.right - f_snip.right);
			cropSnip.top = abs(f_snip.top - f_loc.top);
			if (cropSnip.bottom < cropSnip.top || cropSnip.right < cropSnip.left)
				return;
			cropImage->CopyFromBitmap(&point, image.Get(), &cropSnip);
			rt->DrawBitmap(cropImage.Get(), f_snip);
		}
		//cropImage->CopyFromBitmap(NULL)
		
		//rt->DrawBitmap(image, snip, thickness, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, location);
		
	}
}

/*
* Method: TContent - getColor
* Purpose: returieves the current color of the content
* Parameters: void
* Returns: D2D1::ColorF - the current color
*/
D2D1::ColorF TContent::getColor()
{
	return color;
}

/*
* Method: TContent - setOpaquency
* Purpose: Updates the opaquency after creation
* Parameters: float f - the new opacuency to use
* Returns: bool whether the input was valid or not
*/
bool TContent::setOpaquency(float f)
{
	if(f > 1.000000000 || f < 0.0f)
		return false;
	color.a = f;
	return true;
}

/*
* Method: TContent - setColor
* Purpose: Updates the color of the content after creation
* Parameters: D2D1::ColorF cf - the new color
* Returns: void 
*/
void TContent::setColor(D2D1::ColorF cf)
{
	color = cf;
	if (brush.Get())
	{
		ID2D1SolidColorBrush* sb = dynamic_cast<ID2D1SolidColorBrush*>(brush.Get());
		if (sb)
			sb->SetColor(cf);
	}
}

/*
* Method: TContent - getLocation
* Purpose: Retrieves the current location of the content
* Parameters:
* Returns: RECT - the current location of the Control
*/
RECT TContent::getLocation()
{
	return convertD2DRectToRECT(location);
}

void TContent::SetRadialImage(TDataArray<D2D1_COLOR_F>& colors)
{
	TrecComPointer<ID2D1GradientStopCollection> stopper = getStopCollection(colors);
	if (!stopper.Get()) return;

	gradStop = stopper;
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
	rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
		D2D1::Point2F(location.left, location.top),
		D2D1::Point2F(location.right, location.bottom),
		location.right - location.left, location.bottom - location.top),
		stopper.Get(), radBrush.GetPointerAddress());
	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
}

void TContent::SetLinearImage(TDataArray<D2D1_COLOR_F>& colors)
{
	TrecComPointer<ID2D1GradientStopCollection> stopper = getStopCollection(colors);
	if (!stopper.Get()) return;

	gradStop = stopper;
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		D2D1::Point2F(location.left, location.top),
		D2D1::Point2F(location.right, location.bottom)),
		stopper.Get(), linBrush.GetPointerAddress());
	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
}

void TContent::SetRadialImage(TDataArray<D2D1_GRADIENT_STOP>& colors)
{
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopper;

	if (!rt.Get()) return;

	rt->CreateGradientStopCollection(colors.data(), colors.Size(), stopper.GetPointerAddress());

	if (!(*stopper.GetPointerAddress())) return;

	gradStop = stopper.Extract();
	TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
	rt->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
		D2D1::Point2F(location.left, location.top),
		D2D1::Point2F(location.right, location.bottom),
		location.right - location.left, location.bottom - location.top),
		gradStop.Get(), radBrush.GetPointerAddress());
	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
}

void TContent::SetLinearImage(TDataArray<D2D1_GRADIENT_STOP>& colors)
{
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stopper;

	if (!rt.Get()) return;
	
	rt->CreateGradientStopCollection(colors.data(), colors.Size(), stopper.GetPointerAddress());

	if (!(*stopper.GetPointerAddress())) return;

	gradStop = stopper.Extract();
	TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
	rt->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
		D2D1::Point2F(location.left, location.top),
		D2D1::Point2F(location.right, location.bottom)),
		gradStop.Get(), linBrush.GetPointerAddress());
	brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
}

TrecComPointer<ID2D1GradientStopCollection> TContent::getStopCollection(TDataArray<D2D1_COLOR_F>& colors)
{
	if (!rt.Get() || !colors.Size()) return TrecComPointer<ID2D1GradientStopCollection>();
	TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder stop;
	TDataArray<D2D1_GRADIENT_STOP> gradients;

	float stopValue = 1.0f / static_cast<float>(colors.Size());

	for (UINT Rust = 0; Rust < colors.Size(); Rust++)
	{
		D2D1_GRADIENT_STOP gStop{ static_cast<float>(Rust) * stopValue, colors[Rust] };
		gradients.push_back(gStop);
	}

	rt->CreateGradientStopCollection(gradients.data(), gradients.Size(), stop.GetPointerAddress());
	return stop.Extract();
}

/*
* Method: TContent - storeInTML
* Purpose: Enables the Content to store itself in a TML file
* Parameters: CArchive * ar - the file to write to
*				int childLevel - the generation level content is (how many dashes to write)
*				messageState ms - the state the content is (Object does not know this)
* Returns: int - 0
*/
int TContent::storeInTML(TFile * ar, int childLevel,messageState ms)
{
	TString writable;
	resetAttributeString(&writable, childLevel + 1);
	TString val;
	val.Format(L"%f", thickness);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|ContentThickness:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverContentThickness:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickContentThickness:");
	}

	ar->WriteString(val);
	ar->WriteString(L"\n");
	val = convertD2DColorToString(color);
	ar->WriteString(writable);
	switch (ms)
	{
	case normal:
		ar->WriteString(L"|ContentColor:");
		break;
	case mouseHover:
		ar->WriteString(L"|HoverContentColor:");
		break;
	case mouseLClick:
		ar->WriteString(L"|ClickContentColor:");
	}

	ar->WriteString(val);
	ar->WriteString(L"\n");
	return 0;
}

/*
* Method: TContent - BreakShared
* Purpose: Breaks the link between the Content and the Parent Control, allowing deletion
* Parameters: void
* Returns: void
* Note: DEPRECIATED - Method was made redundant when std:shared_ptr's were replaced with TrecPointers
*/
void TContent::BreakShared()
{
	cap = nullptr;
}

/*
* Method: (TContainer) (Constructor)
* Purpose: Sets up a Blank TContainer
* Parameters: void
* Returns: void
*
TContainer::TContainer()
{
	child = null<TControl>();
	snip = location = RECT{ 0,0,0,0 };
	beenCreated = false;
}

/*
* Method: (TContainer) (Destructor)
* Purpose: Cleans up the Container
* Parameters: void
* Returns: void
*
TContainer::~TContainer()
{
	child.Delete();
}

/*
* Method: TContainer - setTControl
* Purpose: Sets the control to hold
* Parameters: TrecPointer<TControl> tc - Smart pointer to the underlying TControl
* Returns: bool - whether the Smart Pointer held a null value
*
bool TContainer::setTControl(TrecPointer<TControl>tc)
{
	if (!tc.Get())
		return false;
	child = tc;
	return true;
}

/*
* Method: TContainer - onCreate
* Purpose: Sets up the location of the Container using a point and hwight and width
* Parameters: RECT ma - provides the top left margin as well as a height and width
*				RECT bounds - 
* Returns: bool - false if conditions weren't right (previously done, control not set), true otherwise
*
bool TContainer::onCreate(RECT ma,RECT bounds)
{
	if (beenCreated)
		return false;
	
	location.left = bounds.left + ma.left;
	location.top = bounds.top + ma.top;

	location.bottom = location.top + ma.bottom;
	location.right = location.left + ma.right;

	if(!child.Get())
		return false;
	beenCreated = true;
	ChildCreate(location);
	return true;
}

/*
* Method: TContainer - onCreate
* Purpose: Sets up the TContainer using a boundary
* Parameters: RECT l - the boundary to draw on
* Returns: bool - false if conditions weren't right (previously done, control not set), true otherwise
*
bool TContainer::onCreate(RECT l)
{

	location.top += l.top;
	location.left += l.left;
	location.right += l.right;
	location.bottom += l.bottom;
	
	if (beenCreated)
		return false;
	beenCreated = true;
	if (!child.Get())
		return false;
	else
		return ChildCreate(location);
	//return true;
}

/*
* Method: TContainer - onCreate
* Purpose: Sets up the TContainer using a pre-set location
* Parameters: void
* Returns: bool - false if conditions weren't right (previously done, control not set), true otherwise
* Note: You should call this method after you have set the location, otherwise, the control and it's children
*		will NOT be seen
*
bool TContainer::onCreate()
{
	if (beenCreated)
		return false;
	beenCreated = true;
	if (!child.Get())
		return false;
	else
		return ChildCreate(location);
}

/*
* Method: TContainer - onDraw
* Purpose: Calls the onDraw method of the underlying TControl
* Parameters: void
* Returns: void 
*
void TContainer::onDraw()
{
	if (child.Get())
		ChildDraw();
}

/*
* Method: TContainer - getLocation
* Purpose: Retrieves the Current Location of the container, not the Control
* Parameters: void
* Returns: RECT - the current location
*
RECT TContainer::getLocation()
{
	return location;
}

/*
* Method: TContainer - ShiftHorizontal
* Purpose: Moves the Container left-or right, expected to be used by TLayouts
* Parameters: int degrees - how much to move the containter and which direction
* Returns: void
*
void TContainer::ShiftHorizontal(int degrees)
{
	location.left += degrees;
	location.right += degrees;
	if (!isSnipZero(snip))
	{
		snip.left += degrees;
		snip.right += degrees;
	}
}

/*
* Method: TContent - ShiftVertical
* Purpose: Moves the container up or down, expected to be used by TLayouts
* Parameters: int degrees - how much to move the container and which direction
* Returns: void
*
void TContainer::ShiftVertical(int degrees)
{
	location.top += degrees;
	location.bottom += degrees;
	if (!isSnipZero(snip))
	{
		snip.top += degrees;
		snip.bottom += degrees;
	}
}

/*
* Method: TContainer - OnLButtonUp
* Purpose: Allows control to catch the Left Button Up event and act accordingly
* Parameters: UINT u - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*
void TContainer::OnLButtonUp(UINT u, TPoint cp, messageOutput * mo, TDataArray<EventID_Cred>& eventAr)
{
	if (child.Get())
	{
		child->OnLButtonUp(u, cp, mo, eventAr);
	}
}

/*
* Method: TContainer - OnLButtonDown
* Purpose: Allows Control to catch the Left Mouse Button Down event and act accordingly
* Parameters: UINT u - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*
void TContainer::OnLButtonDown(UINT u, TPoint cp, messageOutput* mo, TDataArray<EventID_Cred>& eventAr)
{
	if (child.Get())
	{
		child->OnLButtonDown(u, cp, mo, eventAr);
	}
}

/*
* Method: TContainer - OnMouseMove
* Purpose: Allows Controls to catch the Mouse Move event and deduce if the cursor has hovered over it
* Parameters: UINT u - flags provided by MFC's Message system, not used
*				TPoint point - the point on screen where the event occured
*				messageOutput* mOut - allows controls to keep track of whether ohter controls have caught the event
*				TDataArray<EventID_Cred>& eventAr - allows Controls to add whatever Event Handler they have been assigned
* Returns: void
*
void TContainer::OnMouseMove(UINT u, TPoint cp, messageOutput* mo, TDataArray<EventID_Cred>& eventAr)
{
	if (child.Get())
	{
		child->OnMouseMove(u, cp, mo, eventAr);
	}
}

/*
* Method: TContainer - getChildControl
* Purpose: Retireves the underlying Control held by the container
* Parameters: void
* Returns: TrecPointer<TControl> - the Underlying Control
*
TrecPointer<TControl> TContainer::getChildControl()
{
	return TrecPointer<TControl>(child);
}

/*
* Method: TContainer - storeInTML
* Purpose: Allows the Container to store the Control in a TML file
* Parameters: CArchive* ar - the file to write to
*				int childLevel - the generation count (how many dashes to write)
* Returns: int - 0
*
int TContainer::storeInTML(CArchive * ar, int childLevel)
{
	TString str;
	resetAttributeString(&str, childLevel);
	CString val = convertRectToString(location);
	ar->WriteString(str);
	ar->WriteString(L"|ContainerLoc:"));
	ar->WriteString(val);
	ar->WriteString(L"\n"));
	if (child.Get())
	{
		child->storeInTML(ar, childLevel + 1);
	}

	return 0;
}

/*
* Method: TContainer - storeInHTML
* Purpose: Allows the Container to store the Control in an HTML page
* Parameters: CArchive* ar - the file to write to
* Returns: void
*
void TContainer::storeInHTML(CArchive * ar)
{
}

/*
* Method: TContainer - setParent
* Purpose: Sets the parent of the Control this container holds
* Parameters: TrecPointer<TControl> tc - the parent Control that hold it
* Returns: void
*
void TContainer::setParent(TrecPointer<TControl> tc)
{
	if (child.Get())
	{
		child->setParent(tc);
	}
}

/*
* Method: TContainer - setLocation
* Purpose: Sets the location of the Container
* Parameters: RECT & r -  the new location
* Returns: void
*
void TContainer::setLocation(RECT & r)
{
	location = r;
}

/*
* Method: TContainer - ChildCreate
* Purpose: Calls the underlying onCreate method on the TControl held
* Parameters: RECT l - the location of the control
* Returns: bool - false if control is null, otherwise, see "TControl - onCreate"
*
bool TContainer::ChildCreate(RECT l)
{
	if (!child.Get())
		return false;
	return child->onCreate(l);
}

/*
* Method: TContainer - ChildDraw
* Purpose: Calls the TControl onDraw method that the Container holds
* Parameters: void
* Returns: void
*
void TContainer::ChildDraw()
{
	if (!child.Get())
		return;
	child->onDraw();
}

/*
* Function: resetAttributeString
* Purpose: Resets an attribute string for when saving to a TML String attribute
* Parameters: TString* st - The String to Adjust
*				int cou - the generation count
* Returns: void
*/
void resetAttributeString(TString* st, int cou)
{
	st->Empty();
	for (int c = 0; c < cou;c++)
		st->AppendChar(L'-');
}

/*
* Function: convertD2DColorToString
* Purpose: Creates a string that represents a D2D color
* Parameters: D2D1::ColorF - the color to convert
* Returns: CString - the Color in string form
*/
TString convertD2DColorToString(D2D1::ColorF color) 
{
	TString val;
	val.Format(L"%f", color.r);
	TString col = val + TString(",");
	val.Format(L"%f", color.g);
	col = col + val + TString(",");
	val.Format(L"%f", color.b);
	col = col + val + TString(",");
	val.Format(L"%f", color.a);
	col = col + val;
	return col;
}


/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: RECT - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString convertRectToString(RECT sty) 
{
	TString val;
	val.Format(L"%d", sty.top);

	TString comma(L",");

	TString col = val + comma;
	val.Format(L"%d", sty.left);
	col = col + val + comma;
	val.Format(L"%d", sty.bottom);
	col = col + val + comma;
	val.Format(L"%d", sty.right);
	col = col + val;
	return col;
}

/*
* Function: convertStringToRECT
* Purpose: Determines the Rectangle to use
* Parameters: TString* the String to read
* Returns: RECT - the resulting rectangle (Zero Rect if string is invalid)
*/
RECT convertStringToRECT(TString* str)
{
	RECT returnable = RECT{ 0,0,0,0 };
	if (str == NULL)
	{
		fallBack:
		returnable = RECT{ 0,0,0,0 };
		return returnable;
	}

	TrecPointer<TDataArray<TString>> strSpl = str->split(",");

	
	int res[4] = { 0,0,0,0 };
	if (strSpl.Get() == NULL || strSpl->Size() != 4)
	{
		goto fallBack;
	}
	res[0] = strSpl->at(0).ConvertToInt((int*)&returnable.top);
	res[1] = strSpl->at(1).ConvertToInt((int*)&returnable.left);
	res[2] = strSpl->at(2).ConvertToInt((int*)&returnable.bottom);
	res[3] = strSpl->at(3).ConvertToInt((int*)&returnable.right);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
		goto fallBack;
	return returnable; LONG i;
}

/*
* Function: convertStringToD2DColor
* Purpose: Converts a String to it's Color Representation (Black if invalid)
* Parameters: TString* - the string to read
* Returns: D2D1::ColorF - the color the string represents (Black if invalid)
*/
D2D1::ColorF convertStringToD2DColor(TString *str)
{
	int res[4] = { 0,0,0,0 };
	float rgba[4] = { 0,0,0,0 };
	if (str == NULL)
	{

		colorFallBack:
		return D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	}

	TrecPointer<TDataArray<TString>> strSpl = str->split(",");

	
	
	res[0] = strSpl->at(0).ConvertToFloat(&rgba[0]);
	res[1] = strSpl->at(1).ConvertToFloat(&rgba[1]);
	res[2] = strSpl->at(2).ConvertToFloat(&rgba[2]);
	res[3] = strSpl->at(3).ConvertToFloat(&rgba[3]);
	if (res[0] > 0 || res[1] > 0 || res[2] > 0 || res[3] > 0)
	{
		goto colorFallBack;
	}

	for (int c = 0; c < 4; c++)
	{
		if (rgba[c] > 1.0f && rgba[c] <= 10.0f)
			rgba[c] /= 10.0f;
		else if (rgba[c] > 10.0f) // In this case, color was given in 255 form, adjust accordingly
			rgba[c] /= 255.0f;

	}

	return D2D1::ColorF(rgba[0], rgba[1], rgba[2], rgba[3]);
}

/*
* Function: convertCRectToD2DRect
* Purpose: Turns an MFC RECT into a Direct2D Rect for compatibility purposes
* Parameters: RECT * - the source rectangle
*				D2D1_RECT_F* - the destination rectangle
* Returns: false if either parameter is null
*/
bool convertCRectToD2DRect(RECT* mfcr, D2D_RECT_F* d2dr)
{
	if (!mfcr || !d2dr)
		return false;
	d2dr->bottom = mfcr->bottom;
	d2dr->left = mfcr->left;
	d2dr->right = mfcr->right;
	d2dr->top = mfcr->top;
	return true;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint* - the point to check
*				const RECT* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint* cp, const RECT* r)
{
	if (!cp || !r)
		return false;
	return cp->x > r->left && cp->x < r->right &&
		cp->y > r->top && cp->y < r->bottom;
}

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint* - the point to check
*				const D2D1_RECT_F* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool isContained(const TPoint* cp, const D2D1_RECT_F* r)
{
	if (!cp || !r)
		return false;
	return cp->x > r->left && cp->x < r->right &&
		cp->y > r->top && cp->y < r->bottom;
}

/*
* Function: isContained
* Purpose: Checks if a point is in a certain ellipse
* Parameters: const TPoint* - the point to check
*				const D2D1_ELLIPSE* - the ellipse to check
* Returns: bool - whether the point is within the specified bounds
*/
// To-Do: improve function as more math is learned
bool isContained(const TPoint& cp, const D2D1_ELLIPSE& el) 
{

	return (pow((cp.x - el.point.x),2) / pow(el.radiusX,2)
		+ pow((cp.y - el.point.y),2) / pow(el.radiusY,2)) <= 1;
}

/*
* Function: convertStringToTextAlignment
* Purpose: Reads the String and returns whether the Text should be right, left, center, or justified
* Parameters: TString* t - the string to parse
* Returns: DWRITE_TEXT_ALIGNMENT - the Text position (Center if string is invalid)
*/
DWRITE_TEXT_ALIGNMENT convertStringToTextAlignment(TString* t)
{
	if (!t)
		return DWRITE_TEXT_ALIGNMENT_CENTER;
	if (!t->Compare(L"Left"))
		return DWRITE_TEXT_ALIGNMENT_TRAILING;
	if (!t->Compare(L"Right"))
		return DWRITE_TEXT_ALIGNMENT_LEADING;
	if (!t->Compare(L"Justified"))
		return DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	return DWRITE_TEXT_ALIGNMENT_CENTER;
}

/*
* Function: convertStringToParagraphAlignment
* Purpose: Reads the String and returns whether the Text should be top, bottom, or center
* Parameters: TString* t - the string to parse
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the text alignment (Center if string is invalid)
*/
DWRITE_PARAGRAPH_ALIGNMENT convertStringToParagraphAlignment(TString * t)
{
	if (!t)
		return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	if (!t->Compare(L"Top"))
		return DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	if (!t->Compare(L"Bottom"))
		return DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	return DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
}

/*
* Function: ZeroMemory_
* Purpose: "Zero's an Event Args, (Using Microsoft's ZeroMemory macro currupts the TString in the EVent Args)
* Parameters: EventArgs& ea - the Event arguement structure to reset
* Returns: void
*/
void ZeroMemory_(EventArgs & ea)
{
	ea.arrayLabel = -1;
	ea.control = nullptr;
	ea.eventType = static_cast<R_Message_Type>(0);
	ea.isClick = false;
	ea.isLeftClick = false;
	ea.methodID = -1;
	ea.point.x = 0.0f;
	ea.point.y = 0.0f;
	ea.positive = false;
	ea.text.Empty();
}



/*
* Function: switchLongs
* Purpose: Swaps longs
* Parameters: LONG& l1 - the first long to switch
*				LONG& l2 - the second long to switch
* Returns: void
*/
void switchLongs(LONG & l1, LONG & l2)
{
	LONG temp = l1;
	l1 = l2;
	l2 = temp;
}

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const RECT& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool isSnipZero(const RECT & snip)
{
	return !snip.bottom && !snip.left && !snip.right && !snip.top;
}

bool _ANAFACE_DLL isSnipZero(const D2D1_RECT_F& snip)
{
	return !snip.bottom && !snip.left && !snip.right && !snip.top;
}

/*
* Function: zeroSnip
* Purpose: Sets the Rectangle to 0, used to mark a control as off screen
* Parameters: RECT& snip - the Rectangle to Nullify
* Returns: void
*/
void zeroSnip(RECT & snip)
{
	snip.bottom = 0;
	snip.left = 0;
	snip.right = 0;
	snip.top = 0;
}

bool operator==(D2D1_RECT_F & op1, D2D1_RECT_F & op2)
{
	return op1.bottom == op2.bottom && op1.left == op2.left
		&& op1.right == op2.right && op1.top == op2.top;
}

bool operator!=(D2D1_RECT_F & op1, D2D1_RECT_F & op2)
{
	return op1.bottom != op2.bottom || op1.left != op2.left
		|| op1.right != op2.right || op1.top != op2.top;
}


/*
* Function: convertRECTToD2DRectF
* Purpose: Converts an MFC Rectangle to a D2D Rectangle
* Parameters: RECT - the Rectangle to Convert
* Returns: D2D1_RECT_F - the D2D Rectangle sought
*/
D2D1_RECT_F convertRECTToD2DRectF(RECT r)
{
	D2D1_RECT_F dRect;
	dRect.bottom = r.bottom;
	dRect.left = r.left;
	dRect.right = r.right;
	dRect.top = r.top;
	return dRect;
}

/*
* Function: convertD2DRectToRECT
* Purpose: Returns an MFC Rectangle from a Direct2D rectangle
* Parameters: D2D1_RECT_F f - the D2D Rectangle to convert
* Returns: RECT - the MFC rectangle created
*/
RECT convertD2DRectToRECT(D2D1_RECT_F f)
{
	RECT ret;
	ret.top = static_cast<LONG>(f.top);
	ret.left = static_cast<LONG>(f.left);
	ret.right = static_cast<LONG>(f.right);
	ret.bottom = static_cast<LONG>(f.bottom);
	return ret;
}
