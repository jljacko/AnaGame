#pragma once
#include "TControl.h"
#include "TGadgetControl.h"


// Allows TTextControls to keep track of numbers whether they are int or float
typedef enum _ANAFACE_DLL iControlType
{
	t_none,  // the type has not been determined
	t_float, // the type is a float
	t_int    // the type is an int
}iControlType;

// Holds either a float or an integer
typedef union _ANAFACE_DLL incrimentControlValue
{
	float f;
	int i;
}incrimentControlValue;

// Holds a value of type int or float and a marker indicating the type
typedef struct _ANAFACE_DLL incrimentControl
{
	iControlType type;
	incrimentControlValue value;
	void operator=(incrimentControl&);
	void operator=(int i);
	void operator=(float f);
}incrimentControl;

// Allows Anaface to format the text at different sections, used by Web-Tuors
// To support the <b> and <i> tags in HTML
typedef struct formattingDetails
{
	DWRITE_FONT_WEIGHT weight; // For handling boldness
	DWRITE_FONT_STYLE style;   // For handling italics
	DWRITE_TEXT_RANGE range;   // the Range through which it should be done
}formattingDetails;

bool operator>(incrimentControl& ic1, incrimentControl& ic2);
bool operator>(incrimentControl& ic, int i);
bool operator>(incrimentControl& ic, float f);
bool operator>(int i, incrimentControl& ic);
bool operator>(float f, incrimentControl& ic);

bool operator<(incrimentControl& ic1, incrimentControl& ic2);
bool operator<(incrimentControl& ic, int i);
bool operator<(incrimentControl& ic, float f);
bool operator<(int i, incrimentControl& ic);
bool operator<(float f, incrimentControl& ic);

bool operator>=(incrimentControl& ic1, incrimentControl& ic2);
bool operator>=(incrimentControl& ic, int i);
bool operator>=(incrimentControl& ic, float f);
bool operator>=(int i, incrimentControl& ic);
bool operator>=(float f, incrimentControl& ic);

bool operator<=(incrimentControl& ic1, incrimentControl& ic2);
bool operator<=(incrimentControl& ic, int i);
bool operator<=(incrimentControl& ic, float f);
bool operator<=(int i, incrimentControl& ic);
bool operator<=(float f, incrimentControl& ic);

bool operator==(incrimentControl& ic1, incrimentControl& ic2);
bool operator==(incrimentControl& ic, int i);
bool operator==(incrimentControl& ic, float f);
bool operator==(int i, incrimentControl& ic);
bool operator==(float f, incrimentControl& ic);

bool operator==(D2D1_COLOR_F& c1, D2D1_COLOR_F& c2);

typedef struct ColorEffect
{
	ID2D1SolidColorBrush* colBrush;
	D2D1_COLOR_F color;
};


/*
* Class: TTextField
* Purpose: Supports Anaface's ability to have more complex text layouts, including
*	Password boxes, number fields, or complex formatted strings.
* SuperClass: TGadgetControl - allows the combobox to have a feture within it
*/
class _ANAFACE_DLL TTextField :	public TGadgetControl
{
	friend class AnafaceParser;
public:
	TTextField(TrecComPointer<ID2D1RenderTarget>, TrecPointer<TArray<styleTable>> ta, HWND);
	~TTextField();


	bool onClick(CPoint);

	//virtual int loadFromTML(CArchive* ar) override;
	virtual int loadFromHTML(TFile* ar) override;
	virtual void storeInTML(TFile* ar, int childLevel, bool ov = false) override;
	virtual void storeInHTML(TFile* ar) override;

	virtual bool onCreate(RECT) override;

	virtual void onDraw(TObject* obj = nullptr) override;
	UINT determineMinHeightNeeded()override;
	void SetNewLocation(RECT& r)override;

	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg bool OnChar(bool fromChar,UINT nChar, UINT nRepCnt, UINT nFlags, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point, messageOutput* mOut, TDataArray<EventID_Cred>& eventAr);

	void AppendBoldText(TString& t);
	void AppendItalicText(TString& t);
	void AppendBoldItalicText(TString& t);
	void AppendNormalText(TString& t);

	bool isOnFocus();
	TString GetText();
	void SetText(TString);
	virtual UCHAR* GetAnaGameType()override;

	void Resize(RECT r) override;
	void setNumericText(int);
	void setNumericText(float);

	void AddColorEffect(D2D1_COLOR_F col, UINT start, UINT length);
private:
	bool isPassword, offerPasswordPeek;
	bool isEditable;
	TString text;
	int caretLoc;
	bool onFocus;
	bool isNumber, drawNumBoxes;
	HWND windowHandle;
	incrimentControl incriment;

	// extra resources
	D2D1_ELLIPSE passwordPeek_outer, passwordPeek_inner;
	bool showPassword;
	CPoint circleCenter;
	int radius;

	D2D1_RECT_F botBut;
	D2D1_RECT_F topBut;


	void updateNumber(bool);

	void setToZero();

	void InputChar(wchar_t, int);
	void updateTextString();
	void moveCaretLeft(CPoint point);
	void moveCaretRight(CPoint point);
	void moveCaretUp(CPoint point);
	void moveCaretDown(CPoint point);

	// For Numbers, handle minumum and maximum
	bool hasMin, hasMax;
	incrimentControl minumum, maximum;

	TDataArray<formattingDetails> details;
	TDataArray<ColorEffect> colors;
};

