#pragma once

#include <d2d1.h>					// Direct 2D support
#include <d2d1_1.h>					// Help with the D2D1Context Device if necessary
#include <dwrite.h>					// Direct Write
#include <TString.h>				// Use AnaGame's TString for simple conversion to numbers
#include <wincodec.h>				
#include "Anaface.h"				
#include "ControlTypeSafety.h"
#include "TScrollBar.h"
#include "EventTarget.h"
#include <TrecReference.h>
#include <TDataArray.h>
#include <TFile.h>
#include <TPoint.h>

#include <TWindowEngine.h>

//#define _TREC_LIB_DLL __declspec(dllimport)
#include <TMap.h>

#define afx_msg

class TControl;
class TFlyout;
class TContextMenu;
//using namespace ControlTypeSafety;

// Declare existance of classes so that they can be attributes
// Only implementations need class definitions


#define _WRITE_THE_STRING 		ar->WriteString(appendable);      \
	ar->WriteString(L"\n");                  \
	resetAttributeString(&appendable, childLevel + 1);

/* Used by the control to determine which to draw */
typedef enum messageState
{
	normal,
	mouseHover,
	mouseLClick,
	mouseRClick
} messageState;

/* Allows individual Controls to communicate with the message engine that called it
typedef enum messageOutput
{
	negative,
	negativeUpdate,
	positiveOverride,
	positiveContinue,
	positiveOverrideUpdate,
	positiveContinueUpdate
}messageOutput;*/

// Determines the message type for UI Responsiveness
typedef enum R_Message_Type
{
	On_Click,
	On_Hold_Click,
	On_Hover,
	On_Hover_Leave,
	On_Right_Click,
	On_Click_Release,
	On_Text_Change,
	On_Right_Release,
	On_sel_change,
	On_check,
	On_radio_change,
	On_Char,
	On_Focus,
	On_Lose_Focus
}R_Message_Type;

// Event Arguements, used to supply Event Handlers with the necessary information to 
//		do their job properly
typedef struct EventArgs
{
	TString text;
	bool positive;
	TPoint point;
	bool isClick;
	bool isLeftClick;
	R_Message_Type eventType;
	TControl* control;
	int methodID;
	int arrayLabel;
	WCHAR type;
} EventArgs;

// Used by controls to register themselves in the message queue if they have a message handler
typedef struct EventID_Cred
{
	R_Message_Type eventType;
	TControl* control;
}EventID_Cred;

// Stored by Controls for checking whether or not they actually have a message handler
typedef struct EventTypeID
{
	R_Message_Type eventType;
	int eventID;
}EventTypeID;


// Conrols the size of a Given TControl
typedef struct sizeControl
{
	int width;
	int height;
	int minHeight;
	int minWidth;
	int maxWidth;
	int maxHeight;
}sizeControl;

// Determines the basic size of the TControl
//	NOTE: Feature is unstable, stick with T_Rect for now
typedef enum TShape {
	T_Rect,
	T_Rounded_Rect,
	T_Ellipse,
	T_Custom_shape
}TShape;

// Allows TControls to draw with special styles if specified in the ML
typedef enum BrushMarker
{
	BM_SolidBrush,
	BM_RadientBrush,
	BM_LinearBrush
}BrushMarker;




/*
 * Class TBorder
 * Purpose: Draws the Border of a Given TControl
 */
class _ANAFACE_DLL TBorder
{
	friend class TControl;
	friend class TBorder;
public:
	TBorder(TrecComPointer<ID2D1RenderTarget>,TControl*);
	TBorder(TrecPointer<TBorder>&,TControl*);
	TBorder();
	~TBorder();

	// Various On Create Methods depending on the shape of the control
	bool onCreate(RECT);
	bool onCreate(D2D1_ELLIPSE);
	bool onCreate(D2D1_ROUNDED_RECT);

	// Drawing Method
	void onDraw(RECT& loc);

	// Handling Colors
	// NOTE: For Compatibility between AnaGame for Windows and an eventual AnaGame for Unix,
	//	Parameters and Return values should be replaced with a cross-platform structure 
	void setColor(D2D1::ColorF cf);
	D2D1::ColorF getColor();

	// Methods for handling Opaquency. Note, there is no getOpaquency() method as it is handled by the getColored method 
	void setThickness(float f);
	void setOpaquency(float f);
	float getThickness();

	// Getting the Location
	RECT getLocation();
	void ShiftHorizontal(int degrees);
	void ShiftVertical(int degrees);
private:
	//CMap<CString, CString, CString, CString> styles;

	TrecComPointer<ID2D1Brush> brush;												// The Brush to Paint with

	void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>);
	void ResetBrush();

	TrecComPointer <ID2D1SolidColorBrush> BuilderFocusBrush;						// Used by the Builder to Highlight Controls under editing
	float thickness = 1.0f;											// Thickness
	TString style;													// Holds a special style, NOTE: Feature not Implemented
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);	// The Color of the Border
	D2D1::ColorF color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);	// The Second color used by multi color styles of the Control
	TrecComPointer<ID2D1RenderTarget> rt;											// Pointer to the Render Target to Draw to
	TrecComPointer<ID2D1BitmapBrush> bitBrush;										// In the event that an image is part of the Border
	TrecComPointer<ID2D1Bitmap> image;												// the Image of the above structure
	int storeInTML(TFile* ar, int childLevel,messageState);		// Method to save the border in a TML File for later reconstrcting
	TControl* cap;									// The TControl that owns the Border
	TShape shape;													// Shape to Draw
	bool secondColor;												// Whether or not a second color is being used

	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearProp;				// Used for Linear style color graphics
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES radialProp;				// Used for Radial style color graphics

	D2D1_GRADIENT_STOP gradients[2];			// Used for Multi Color
	bool useRadial;
	TrecComPointer<ID2D1GradientStopCollection> stopColl;

	// Alternate shapes
	D2D1_RECT_F loci;
	D2D1_ELLIPSE circle;
	D2D1_ROUNDED_RECT roundedRect;
	void BreakShared();
};

/*
 * Class TText
 * Purpose: Handles the Text in a TControl. Note, Normally the text would be static, but extensions of the 
 *	TControl Class might add additional dynamic functionality using this structure
 */
class _ANAFACE_DLL TText
{
	friend class TText;
	friend class TControl;
	friend class TTextField;
	friend class TCheckBox;
	friend class TRadioButton;
//	friend class TComboBox;
	friend class TContextMenu;
public:
	TText(TrecComPointer<ID2D1RenderTarget>,TControl*);
	TText(TrecPointer<TText>&,TControl*);
	TText();
	~TText();

	// Creation Method, only a rect is needed as the text doesn't care for other shapes
	int onCreate(RECT);

	// Resets the layout in the event one of it's underlying attributes changes
	void reCreateLayout();
	void reCreateLayout(TString& str);

	// Draw the Text
	bool onDraw(RECT& loc, TObject* obj = nullptr);

	// Handling Colors
	// NOTE: For Compatibility between AnaGame for Windows and an eventual AnaGame for Unix,
	//	Parameters and Return values should be replaced with a cross-platform structure 
	void setColor(D2D1::ColorF);
	D2D1::ColorF getColor();

	// Setter methods
	bool setNewFont(TString&);

	void setNewFontSize(float fs);
	void setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT ha);
	void setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT va);
	bool setOpaquency(float o);
	void setNewLocation(RECT r);

	// Get Methods
	float getFontSize();
	TString getFont();
	TString getCaption();

	// More Set Methods
	void setCaption(TString& string);

	void setLocale(TString& loc);
	void setFont(TString& f);

	// Remove the Text completely
	void removeCaption();

	DWRITE_TEXT_ALIGNMENT getHorizontalAlignment();
	DWRITE_PARAGRAPH_ALIGNMENT getVerticalAlignment();
	int getLength();
	RECT getLocation();
	void ShiftHorizontal(int degrees);
	void ShiftVertical(int degrees);

private:
	//CMap<CString, CString, CString, CString> styles;
	TrecComPointer<ID2D1RenderTarget> rt;								// Render Target to focus on
	TrecComPointer<IDWriteFactory> writeFact;// Direct Write Factory
	
	void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>);
	void ResetBrush();
	
	D2D1_RECT_F bounds;									// Location of the text
	TString text;										// Text stored
	UINT32 textLength;									// Length of text
	TrecComPointer<ID2D1Brush> penBrush;								// Brush to draw with


	DWRITE_FONT_WEIGHT fontWeight;
	DWRITE_FONT_STYLE fontStyle;
	DWRITE_FONT_STRETCH fontStretch;

	DWRITE_PARAGRAPH_ALIGNMENT verticalAlignment;
	DWRITE_TEXT_ALIGNMENT horizontalAlignment;

	float fontSize = 12;
	TString locale;
	TString font;
	D2D1::ColorF color = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	int storeInTML(TFile* ar, int childLevel,messageState);
	TControl* cap;

	bool secondColor;
	D2D1::ColorF color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);
	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearProp;
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES radialProp;
	D2D1_GRADIENT_STOP gradients[2];
	bool useRadial;
	TrecComPointer<ID2D1GradientStopCollection> gradStop;

	// Determine how to draw text
	TrecComPointer<IDWriteTextLayout> fontLayout;
	TrecComPointer<IDWriteTextFormat> format;
	//bool useLayout;

	void BreakShared();
};


class _ANAFACE_DLL TContent
{
	friend class TControl;
	friend class TGadgetControl;
public:
	TContent(TrecComPointer<ID2D1RenderTarget>, TControl*);
	TContent();
	TContent(TrecPointer<TContent>&, TControl*);
	~TContent();

	bool onCreate(RECT);
	bool onCreate(D2D1_ELLIPSE);
	bool onCreate(D2D1_ROUNDED_RECT);
	void ShiftHorizontal(int degrees);
	void ShiftVertical(int degrees);
	void onDraw(RECT& loc);
	D2D1_COLOR_F getColor();
	bool setOpaquency(float f);
	void setColor(D2D1_COLOR_F);
	RECT getLocation();

	void SetRadialImage(TDataArray<D2D1_COLOR_F>& colors);
	void SetLinearImage(TDataArray<D2D1_COLOR_F>& colors);

	void SetRadialImage(TDataArray<D2D1_GRADIENT_STOP>& colors);
	void SetLinearImage(TDataArray<D2D1_GRADIENT_STOP>& colors);

private:
	TrecComPointer<ID2D1GradientStopCollection> getStopCollection(TDataArray<D2D1_COLOR_F>& colors);
	void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>);
	void ResetBrush();

	//CMap<CString, CString, CString, CString> styles;
	TrecComPointer<ID2D1RenderTarget> rt;

	TrecComPointer<ID2D1Brush> brush;

	TrecComPointer<ID2D1BitmapBrush> bitmap;

	float thickness = 1.0f;
	TString style;
	D2D1_COLOR_F color;
	D2D1_RECT_F location;
	TrecComPointer<ID2D1Bitmap> image;
	TrecComPointer<ID2D1Bitmap> cropImage;


	int storeInTML(TFile* ar, int childLevel,messageState);
	TControl* cap;
	TShape shape;
	bool secondColor;
	D2D1::ColorF color2 = D2D1::ColorF(D2D1::ColorF::Black, 1.0);

	D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearProp;
	D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES radialProp;
	D2D1_GRADIENT_STOP gradients[2];
	bool useRadial;
	TrecComPointer<ID2D1GradientStopCollection> gradStop;

	// Alternate shapes
	D2D1_ELLIPSE circle;
	D2D1_ROUNDED_RECT roundedRect;

	void BreakShared();
};



class _ANAFACE_DLL TControl : public TObject
{
	friend class TComboBox;
	friend class TControl_Edit;    // CDialog version of TControl editor
	friend class TControl_TDialog; // TDialog version of TControl editor
	friend class TBorder;
	friend class TControl;
public:

	TControl(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> styles, bool base = true);
	TControl(TControl&);
	TControl();
	virtual ~TControl();

	void SetSelf(TrecPointer<TControl> self);

	virtual void SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget>);

	//virtual int loadFromTML(CArchive* ar);
	virtual int loadFromHTML(TFile* ar);
	virtual void storeInTML(TFile* ar, int childLevel, bool overrideChildren = false);
	virtual void storeInHTML(TFile* ar);

	// Normal running messages
	afx_msg virtual void OnRButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDown(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr, TDataArray<TControl*>& clickedButtons);
	afx_msg virtual void OnRButtonDown(UINT nFlags, TPoint, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnMouseMove(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	//afx_msg void OnContextMenu(CWnd* pWnd, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual void OnLButtonUp(UINT nFlags, TPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg virtual bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput *mOut, TDataArray<EventID_Cred>& eventAr);

	// Builder Messages
	afx_msg void Builder_OnLButtonUp(UINT flags, TPoint point, TControl** mOut);
	afx_msg void Builder_OnLButtonDown(UINT flags, TPoint point, TControl** mOut, messageOutput* o);
	afx_msg void Builder_OnMouseMove(UINT flags, TPoint, TControl** mOut,const RECT&, messageOutput* o);
	afx_msg void Remove_Builder_Click_Focus();

	void setActive(bool act);
	EventArgs getEventArgs();

	virtual bool onCreate(RECT, TrecPointer<TWindowEngine> d3d);
	virtual void Resize(RECT);
	bool setEventHandler(EventTarget& eh);
	void updateArrayID(int aid);

	virtual void onDraw(TObject* obj = nullptr);
	RECT getLocation();
	RECT getMargin();
	TrecComPointer<ID2D1RenderTarget> getRenderTarget();
	TrecPointer<TControl> getParent();
	void setExternalBounds(RECT);
	bool getLayoutStatus();
	void offsetLocation(TPoint);

	void ShiftHorizontal(int degrees);
	void ShiftVertical(int degrees);

	void setMinHeight(int h);
	void setMaxHeight(int h);
	void setMinWidth(int w);
	void setMaxWidth(int w);
	void setWidth(int w);
	void setHeight(int h);

	void setLocation(RECT r);

	void setMBottom(int b);
	void setMTop(int t);
	void setMRight(int r);
	void setMLeft(int l);

	virtual UINT determineMinHeightNeeded();
	virtual void SetNewLocation(const RECT& r);
	virtual void ShrinkHeight();

	bool addAttribute(const TString&, TrecPointer<TString>);
	bool addChild(TrecPointer<TControl>);

	void setParent(TrecPointer<TControl> tcp);
	TrecPointer<TText> getText(int n);
	TrecPointer<TContent> getContent(int n);
	TrecPointer<TBorder> getBorder(int n);
	void setNewText(int n);
	//void setWrapperPointer(TrecPointer<TControl> tcp);


	// Methods to support Scrolling
	bool onScroll(int x, int y);
	bool onBeingScrolled(int x, int y);
	void scroll(RECT& loc);
	bool SetContextMenu(TrecPointer<TControl> cm);
	void BreakShared();
	void AddClass(const TString& t);
	TString GetID();
	void resetArgs();
	virtual UCHAR* GetAnaGameType()override;

	void SetNormalMouseState();

protected:
	//CMap<CString, CString, CString, CString> styles;
	bool resistFocusRemoval;
	bool boundsPreset;
	TString className;
	TString ID;
	RECT location, margin;
	TScrollBar *vScroll, *hScroll;
	TrecComPointer<ID2D1Factory> factory;
	TrecComPointer<ID2D1RenderTarget> renderTarget;
	
	TrecPointerSoft<TControl> parent;
	TrecPointerSoft<TControl> tThis;

	//TrecPointer<TControl> PointerCase;
	BYTE treeLevel;
	TrecPointer<TBorder> border1, border2, border3;
	TrecPointer<TText> text1 , text2, text3;
	TrecPointer<TContent> content1 , content2, content3;

	D2D1_ELLIPSE ellipse;
	D2D1_ROUNDED_RECT roundedRect;
	TShape shape;

	bool onCreate(TMap<TString>*,RECT);
	bool onCreate2(TMap<TString>*, RECT);
	bool onCreate3(TMap<TString>*, RECT);

	void updateComponentLocation();
	void CheckScroll();

	int generateImage(TrecPointer<TContent> tcon, TrecPointer<TString> p,RECT loc);

	void checkMargin(RECT);
	void checkHeightWidth(RECT);
	void generateSizeControl();
	//void updateComponentLocations();

	sizeControl* dimensions;

	bool isLayout, isTextControl;


	bool marginPriority,marginSet,fixHeight,fixWidth;

	TMap<TString> attributes;
	TrecPointer<TArray<styleTable>> styles;
	TArray<TControl> children;
	TArray<TArray<TString>> holder;

	// data for dealing with messages
	bool overrideParent;          // should response to message stop parent from doing the same?
	messageState mState;          // mechanism as to which version of the TControl should be drawn based off of user interaction
	TFlyout* flyout;              // Generic flyout, which can appear if attached
	TContextMenu* contextMenu;    // specific flyout primed for dealing with right clicks, as is traditional
	EventArgs args;               // Information about the arguements set
	TDataArray<EventTypeID> eventList;// 
	bool isActive;                // Controls can be disabled
	bool hasEvent(R_Message_Type type);

	int getEventID(R_Message_Type type);

	void SetToRenderTarget();

	// and Builder Messages
	bool rightBorder, leftBorder, topBorder, bottomBorder, // if the border is being moved
		onFocus,
		onClickFocus;

	EventTarget* eventHandler;
	int arrayID;
	public:

	void addEventID(R_Message_Type rmt, int e_id);
	
};

/*
* Function: resetAttributeString
* Purpose: Resets an attribute string for when saving to a TML String attribute
* Parameters: TString* st - The String to Adjust
*				int cou - the generation count
* Returns: void
*/
void _ANAFACE_DLL resetAttributeString(TString* st, int cou);

/*
* Function: convertD2DColorToString
* Purpose: Creates a string that represents a D2D color
* Parameters: D2D1::ColorF - the color to convert
* Returns: CString - the Color in string form
*/
TString _ANAFACE_DLL convertD2DColorToString(D2D1_COLOR_F);

/*
* Function: convertRectToString
* Purpose: Creates a string that represents a rectangle
* Parameters: RECT - the Rectangle to convert
* Returns: CString - the Representation of a rectangle in string form
*/
TString _ANAFACE_DLL convertRectToString(RECT);

/*
* Function: convertStringToRECT
* Purpose: Determines the Rectangle to use
* Parameters: TString* the String to read
* Returns: RECT - the resulting rectangle (Zero Rect if string is invalid)
*/
RECT _ANAFACE_DLL convertStringToRECT(TString*);

/*
* Function: convertStringToD2DColor
* Purpose: Converts a String to it's Color Representation (Black if invalid)
* Parameters: TString* - the string to read
* Returns: D2D1::ColorF - the color the string represents (Black if invalid)
*/
D2D1::ColorF _ANAFACE_DLL convertStringToD2DColor(TString*);

/*
* Function: convertRECTToD2DRectF
* Purpose: Converts an MFC Rectangle to a D2D Rectangle
* Parameters: RECT - the Rectangle to Convert
* Returns: D2D1_RECT_F - the D2D Rectangle sought
*/
D2D1_RECT_F _ANAFACE_DLL convertRECTToD2DRectF(RECT);

/*
* Function: convertD2DRectToRECT
* Purpose: Returns an MFC Rectangle from a Direct2D rectangle
* Parameters: D2D1_RECT_F f - the D2D Rectangle to convert
* Returns: RECT - the MFC rectangle created
*/
RECT _ANAFACE_DLL convertD2DRectToRECT(D2D1_RECT_F f);

/*
* Function: convertCRectToD2DRect
* Purpose: Turns an MFC RECT into a Direct2D Rect for compatibility purposes
* Parameters: RECT * - the source rectangle
*				D2D1_RECT_F* - the destination rectangle
* Returns: false if either parameter is null
*/
bool _ANAFACE_DLL convertCRectToD2DRect(RECT*, D2D1_RECT_F*);

/*
* Function: isContained
* Purpose: Checks of a point is within a given MFC Rectangle
* Parameters: const TPoint* - the point to check
*				const RECT* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint*, const RECT*);

/*
* Function: isContained
* Purpose: Checks of a point is within a given Direct2D Rectangle
* Parameters: const TPoint* - the point to check
*				const D2D1_RECT_F* - the rectangle to check
* Returns: bool - whether the point is withing the bounds
*/
bool _ANAFACE_DLL isContained(const TPoint*, const D2D1_RECT_F*);

/*
* Function: isContained
* Purpose: Checks if a point is in a certain ellipse
* Parameters: const TPoint* - the point to check
*				const D2D1_ELLIPSE* - the ellipse to check
* Returns: bool - whether the point is within the specified bounds
* Note: Algorithm for function found on https://math.stackexchange.com/questions/76457/check-if-a-point-is-within-an-ellipse
*/
bool isContained(const TPoint&, const D2D1_ELLIPSE&);

/*
* Function: convertStringToTextAlignment
* Purpose: Reads the String and returns whether the Text should be right, left, center, or justified
* Parameters: TString* t - the string to parse
* Returns: DWRITE_TEXT_ALIGNMENT - the Text position (Center if string is invalid)
*/
DWRITE_TEXT_ALIGNMENT convertStringToTextAlignment(TString* t);

/*
* Function: convertStringToParagraphAlignment
* Purpose: Reads the String and returns whether the Text should be top, bottom, or center
* Parameters: TString* t - the string to parse
* Returns: DWRITE_PARAGRAPH_ALIGNMENT - the text alignment (Center if string is invalid)
*/
DWRITE_PARAGRAPH_ALIGNMENT convertStringToParagraphAlignment(TString* t);

/*
* Function: ZeroMemory_
* Purpose: "Zero's an Event Args, (Using Microsoft's ZeroMemory macro currupts the TString in the EVent Args)
* Parameters: EventArgs& ea - the Event arguement structure to reset
* Returns: void
*/
void _ANAFACE_DLL ZeroMemory_(EventArgs& ea);

/*
* Function: switchLongs
* Purpose: Swaps longs
* Parameters: LONG& l1 - the first long to switch
*				LONG& l2 - the second long to switch
* Returns: void
*/
void switchLongs(LONG& l1, LONG& l2);

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const RECT& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool _ANAFACE_DLL isSnipZero(const RECT& snip);

/*
* Function: isSnipZero
* Purpose: Checks if a Rectangle is a "Zero" rectangle
* Parameters: const D2D1_RECT_F& snip - the Rectangle to check
* Returns: bool - whether the RECT is "Zero" or not
*/
bool _ANAFACE_DLL isSnipZero(const D2D1_RECT_F& snip);

/*
* Function: zeroSnip
* Purpose: Sets the Rectangle to 0, used to mark a control as off screen 
* Parameters: RECT& snip - the Rectangle to Nullify
* Returns: void
*/
void zeroSnip(RECT& snip);

bool operator==(D2D1_RECT_F& op1, D2D1_RECT_F& op2);

bool operator!=(D2D1_RECT_F& op1, D2D1_RECT_F& op2);

//void DrawArc(const D2D1_ELLIPSE& e, const D2D1_RECT_F& loc, const D2D1_RECT_F& snip, const D2D1_COLOR_F color);