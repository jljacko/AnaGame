#include "stdafx.h"
#include <TDialog.h>
#pragma once

// Always update this when you add methods to dialogMethods
#define TCONTROL_DIALOG_METHOD_COUNT 23

class TControl_TDialog;

typedef void (TControl_TDialog::*methodArray)(TControl* tc, EventArgs ea);

typedef enum targetComponent
{
	get_border,
	get_content,
	get_text
}targetComponent;

typedef struct DialogAtts
{
	TrecPointer<TControl> mode1, mode2, mode3;
	TrecPointer<TControl> sec1, sec2, sec3;
	TrecPointer<TControl> op1, op2, op3;
	TrecPointer<TControl> opV1, opV2, opV3;
	TrecPointer<TControl> red1, red2, red3;
	TrecPointer<TControl> green1, green2, green3;
	TrecPointer<TControl> blue1, blue2, blue3;
	TrecPointer<TControl> control;
}DialogAtts;


class TControl_TDialog :
	public TDialog
{
public:
	TControl_TDialog(int width, int height);
	~TControl_TDialog();

	bool getValidConstruction();

	void initializeControls();
	bool Initialize(int& error);
	void OnRadioNormal(TControl* tc, EventArgs ea);
	void OnRadioHover(TControl* tc, EventArgs ea);
	void OnRadioMouse(TControl* tc, EventArgs ea);

	void OnCheckText(TControl*tc, EventArgs ea);
	void OnCheckBorder(TControl* tc, EventArgs ea);
	void OnCheckContent(TControl* tc, EventArgs ea);

	void OnContentAlpha(TControl*tc, EventArgs ea);
	void OnBorderAlpha(TControl*tc, EventArgs ea);
	void OnTextAlpha(TControl*tc, EventArgs ea);

	void OnContentRed(TControl*tc, EventArgs ea);
	void OnBorderRed(TControl*tc, EventArgs ea);
	void OnTextRed(TControl*tc, EventArgs ea);

	void OnContentGreen(TControl*tc, EventArgs ea);
	void OnBorderGreen(TControl*tc, EventArgs ea);
	void OnTextGreen(TControl*tc, EventArgs ea);

	void OnContentBlue(TControl*tc, EventArgs ea);
	void OnBorderBlue(TControl*tc, EventArgs ea);
	void OnTextBlue(TControl*tc, EventArgs ea);

	void OnControlText(TControl*tc, EventArgs ea);
	void OnVerticalText(TControl* tc, EventArgs ea);
	void OnHorizontalText(TControl* tc, EventArgs ea);
	void OnFontText(TControl* tc, EventArgs ea);
	void OnFontSizeText(TControl* tc, EventArgs ea);

	bool getTargetColor(D2D1_COLOR_F& color, targetComponent target);
	void setTargetColor(D2D1_COLOR_F color, targetComponent target);

	void updateText(TString& newText);
	TrecPointer<TControl> GetControl();
private:
	void MessageHandler()override;

	void setCheckboxes(bool b);


	TString fileHolder;
	bool constructionWorked;

	// Methods holding the Event Handlers
	methodArray dialogMethods[TCONTROL_DIALOG_METHOD_COUNT];

	// Imported from CDialog Version
	messageState stateMode;
	
	TrecPointer<TControl> controlPointer;
	TrecPointer<TArray<styleTable>> table;
	//TControl* control;

	// Pointers to hold onto
	TrecPointer<TText> text1, text2, text3;
	TrecPointer<TContent> content1, content2, content3;
	TrecPointer<TBorder> border1, border2, border3;

	void enableDesableCheck();
	void setTextInControl(bool);
	void setBorderInControl(bool set);
	void setContentInControl(bool);

	TrecPointer<TBorder> getCurrentBorder();
	TrecPointer<TContent> getCurrentContent();
	TrecPointer<TText> getCurrentText();



	//void setCurrentText();
	DialogAtts specControls;

};

