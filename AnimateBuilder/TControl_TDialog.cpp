#include "stdafx.h"
#include "TControl_TDialog.h"
#include "MainFrm.h"

TString method1 = L"OnRadioNormal";
TString method2 = L"OnRadioHover";
TString method3 = L"OnRadioMouse";
TString method4 = L"OnCheckContent";
TString method5 = L"OnCheckBorder";
TString method6 = L"OnCheckText";

TString method_c_a = L"OnContentAlpha";
TString method_b_a = L"OnBorderAlpha";
TString method_t_a = L"OnTextAppha";

TString method_c_r = L"OnContentRed";
TString method_b_r = L"OnBorderRed";
TString method_t_r = L"OnTextRed";

TString method_c_g = L"OnContentGreen";
TString method_b_g = L"OnBorderGreen";
TString method_t_g = L"OnTextGreen";

TString method_c_b = L"OnContentBlue";
TString method_b_b = L"OnBorderBlue";
TString method_t_b = L"OnTextBlue";

TString method_text_t = L"OnControlText";
TString method_text_va = L"OnVerticalAlignment";
TString method_text_ha = L"OnHorizontalAlignment";
TString method_text_f = L"OnFontUpdate";
TString method_text_fs = L"OnFontSizeUpdate";

TControl_TDialog::TControl_TDialog(int width, int height):TDialog(width,height)
{
	CMainFrame* window = (CMainFrame*)AfxGetMainWnd();


	TString direct = window->getSpecialDirectory(bdi_user_appdata);
	
	for (UINT c = 0; c < direct.GetLength();c++)
	{
		filePath += direct[c];
	}

	// Prepare array
	for (int c = 0; c < TCONTROL_DIALOG_METHOD_COUNT; c++)
		dialogMethods[c] = nullptr;

	dialogMethods[0] = &TControl_TDialog::OnRadioNormal;
	dialogMethods[1] = &TControl_TDialog::OnRadioHover;
	dialogMethods[2] = &TControl_TDialog::OnRadioMouse;
	dialogMethods[3] = &TControl_TDialog::OnCheckContent;
	dialogMethods[4] = &TControl_TDialog::OnCheckBorder;
	dialogMethods[5] = &TControl_TDialog::OnCheckText;

	dialogMethods[6] = &TControl_TDialog::OnContentAlpha;
	dialogMethods[7] = &TControl_TDialog::OnBorderAlpha;
	dialogMethods[8] = &TControl_TDialog::OnTextAlpha;

	dialogMethods[9] = &TControl_TDialog::OnContentRed;
	dialogMethods[10] = &TControl_TDialog::OnBorderRed;
	dialogMethods[11] = &TControl_TDialog::OnTextRed;

	dialogMethods[12] = &TControl_TDialog::OnContentGreen;
	dialogMethods[13] = &TControl_TDialog::OnBorderGreen;
	dialogMethods[14] = &TControl_TDialog::OnTextGreen;

	dialogMethods[15] = &TControl_TDialog::OnContentBlue;
	dialogMethods[16] = &TControl_TDialog::OnBorderBlue;
	dialogMethods[17] = &TControl_TDialog::OnTextBlue;

	dialogMethods[18] = &TControl_TDialog::OnControlText;
	dialogMethods[19] = &TControl_TDialog::OnVerticalText;
	dialogMethods[20] = &TControl_TDialog::OnHorizontalText;
	
	dialogMethods[21] = &TControl_TDialog::OnFontText;
	dialogMethods[22] = &TControl_TDialog::OnFontSizeText;

	eventNameID enid;

	enid.eventID = 0;
	enid.name = method1;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 1;
	enid.name = method2;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 2;
	enid.name = method3;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 3;
	enid.name = method4;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 4;
	enid.name = method5;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);


	enid.eventID = 5;
	enid.name = method6;
	//enid = TrecPointer<eventNameID>(&enid);
	idMatch.push_back(enid);

	// Alpha Color events
	enid.eventID = 6;
	enid.name = method_c_a;
	idMatch.push_back(enid);

	enid.eventID = 7;
	enid.name = method_b_a;
	idMatch.push_back(enid);

	enid.eventID = 8;
	enid.name = method_t_a;
	idMatch.push_back(enid);

	// Red Color Events
	enid.eventID = 9;
	enid.name = method_c_r;
	idMatch.push_back(enid);

	enid.eventID = 10;
	enid.name = method_b_r;
	idMatch.push_back(enid);

	enid.eventID = 11;
	enid.name = method_t_r;
	idMatch.push_back(enid);

	// Green Color Events
	enid.eventID = 12;
	enid.name = method_c_g;
	idMatch.push_back(enid);

	enid.eventID = 13;
	enid.name = method_b_g;
	idMatch.push_back(enid);

	enid.eventID = 14;
	enid.name = method_t_g;
	idMatch.push_back(enid);

	// Blue Color Events
	enid.eventID = 15;
	enid.name = method_c_b;
	idMatch.push_back(enid);

	enid.eventID = 16;
	enid.name = method_b_b;
	idMatch.push_back(enid);

	enid.eventID = 17;
	enid.name = method_t_b;
	idMatch.push_back(enid);

	enid.eventID = 18;
	enid.name = method_text_t;
	idMatch.push_back(enid);

	enid.eventID = 19;
	enid.name = method_text_va;
	idMatch.push_back(enid);

	enid.eventID = 20;
	enid.name = method_text_ha;
	idMatch.push_back(enid);

	enid.eventID = 21;
	enid.name = method_text_f;
	idMatch.push_back(enid);

	enid.eventID = 22;
	enid.name = method_text_fs;
	idMatch.push_back(enid);


	filePath += L"\\TControlDialog.txt";


	

	text1 = nullptr; text2 = nullptr; text3 = nullptr;
	content1 = nullptr; content2 = nullptr; content3 = nullptr;
	border1 = nullptr; border2 = nullptr; border3 = nullptr;
	

	controlPointer = new TControl();


	
	
	
}


TControl_TDialog::~TControl_TDialog()
{
}

bool TControl_TDialog::getValidConstruction()
{
	return constructionWorked;
}

void TControl_TDialog::OnRadioNormal(TControl * tc, EventArgs ea)
{
	setCheckboxes(true);
	if (!tc)
		return;
	if (ea.positive)
		stateMode = normal;
	enableDesableCheck();

}

void TControl_TDialog::OnRadioHover(TControl * tc, EventArgs ea)
{
	setCheckboxes(true);
	if (!tc)
		return;
	if (ea.positive)
		stateMode = mouseHover;
	enableDesableCheck();
}

void TControl_TDialog::OnRadioMouse(TControl * tc, EventArgs ea)
{
	setCheckboxes(true);
	if (!tc)
		return;
	if (ea.positive)
		stateMode = mouseLClick;
	enableDesableCheck();
}

void TControl_TDialog::OnCheckText(TControl * tc, EventArgs ea)
{
	bool enableCont = true;
	if(tc) // ea is reliable
	{
		enableCont = ea.positive;
	}
	else
	{
		if(specControls.sec3.get() && typeid(specControls.sec3.get()) == typeid(TCheckBox*))
			enableCont = dynamic_cast<TCheckBox*>(specControls.sec3.get())->IsClicked();
	}

	for (int c = 3; c < 7; c++)
	{
		try
		{
			TLayout* lay = dynamic_cast<TLayout*>(pointer.get());
			TrecPointer<TControl> tcp =  lay->GetLayoutChild(2, c);
			if (!tcp.get())
				continue;
			tcp->setActive(enableCont);
		}
		catch (std::bad_cast b)
		{
			continue;
		}
	}
	setTextInControl(enableCont);
}

void TControl_TDialog::OnCheckBorder(TControl * tc, EventArgs ea)
{
	bool enableCont = true;
	if (tc) // ea is reliable
	{
		enableCont = ea.positive;
	}
	else
	{
		if (specControls.sec2.get() && typeid(specControls.sec2.get()) == typeid(TCheckBox*))
			enableCont = dynamic_cast<TCheckBox*>(specControls.sec2.get())->IsClicked();
	}

	for (int c = 3; c < 7; c++)
	{
		try
		{
			TLayout* lay = dynamic_cast<TLayout*>(pointer.get());
			TrecPointer<TControl> tcp = lay->GetLayoutChild(2, c);
			if (!tcp.get())
				continue;
			tcp->setActive(enableCont);
		}
		catch (std::bad_cast b)
		{
			continue;
		}
	}
	setBorderInControl(enableCont);
}

void TControl_TDialog::OnCheckContent(TControl * tc, EventArgs ea)
{
	bool enableCont = true;
	if (tc) // ea is reliable
	{
		enableCont = ea.positive;
	}
	else
	{
		if (specControls.sec1.get() && typeid(specControls.sec1.get()) == typeid(TCheckBox*))
			enableCont = dynamic_cast<TCheckBox*>(specControls.sec1.get())->IsClicked();
	}

	for (int c = 3; c < 7; c++)
	{
		try
		{
			TLayout* lay = dynamic_cast<TLayout*>(pointer.get());
			TrecPointer<TControl> tcp = lay->GetLayoutChild(2, c);
			if (!tcp.get())
				continue;
			tcp->setActive(enableCont);
		}
		catch (std::bad_cast b)
		{
			continue;
		}
	}

	setContentInControl(enableCont);
}

void TControl_TDialog::OnContentAlpha(TControl * tc, EventArgs ea)
{
	assert(tc);
	if (!ea.text)
		return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_content))
		{
			color.a = value;
			setTargetColor(color, get_content);
		}
	}
}

void TControl_TDialog::OnBorderAlpha(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_border))
		{
			color.a = value;
			setTargetColor(color, get_border);
		}
	}
}

void TControl_TDialog::OnTextAlpha(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_text))
		{
			color.a = value;
			setTargetColor(color, get_text);
		}
	}
}

void TControl_TDialog::OnContentRed(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_content))
		{
			color.r = value;
			setTargetColor(color, get_content);
		}
	}
}

void TControl_TDialog::OnBorderRed(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_border))
		{
			color.r = value;
			setTargetColor(color, get_border);
		}
	}
}

void TControl_TDialog::OnTextRed(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_text))
		{
			color.r = value;
			setTargetColor(color, get_text);
		}
	}
}

void TControl_TDialog::OnContentGreen(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_content))
		{
			color.g = value;
			setTargetColor(color, get_content);
		}
	}
}

void TControl_TDialog::OnBorderGreen(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_border))
		{
			color.g = value;
			setTargetColor(color, get_border);
		}
	}
}

void TControl_TDialog::OnTextGreen(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_text))
		{
			color.g = value;
			setTargetColor(color, get_text);
		}
	}
}

void TControl_TDialog::OnContentBlue(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_content))
		{
			color.b = value;
			setTargetColor(color, get_content);
		}
	}
}

void TControl_TDialog::OnBorderBlue(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_border))
		{
			color.b = value;
			setTargetColor(color, get_border);
		}
	}
}

void TControl_TDialog::OnTextBlue(TControl * tc, EventArgs ea)
{
	assert(tc);
	if(!ea.text) return;

	TString text = ea.text;
	float value = 0.0f;
	if (!text.ConvertToFloat(&value) && value >= 0.0f && value <= 1.0f)
	{
		D2D1_COLOR_F color = { 0.0f,0.0f,0.0f,0.0f };
		if (getTargetColor(color, get_text))
		{
			color.b = value;
			setTargetColor(color, get_text);
		}
	}
}

void TControl_TDialog::OnControlText(TControl * tc, EventArgs ea)
{
	assert(tc);
	TrecPointer<TText> text = tc->getText(1);
	if (text.get())
	{
		TString tText = text->getCaption();
		updateText(tText);
	}
}

void TControl_TDialog::OnVerticalText(TControl * tc, EventArgs ea)
{
	TrecPointer<TText> curText = getCurrentText();
	if (!curText.get() || !tc)
		return;
	if (!ea.text)
		return;

	if (!StrCmpW(L"Top", ea.text))
	{
		curText->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}
	else if (!StrCmpW(L"Center", ea.text))
	{
		curText->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	else if (!StrCmpW(L"Bottom", ea.text))
	{
		curText->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	}
}

void TControl_TDialog::OnHorizontalText(TControl * tc, EventArgs ea)
{
	TrecPointer<TText> curText = getCurrentText();
	if (!curText.get() || !tc)
		return;
	if (!ea.text)
		return;

	if (!StrCmpW(L"Left", ea.text))
	{
		curText->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}
	else if (!StrCmpW(L"Center", ea.text))
	{
		curText->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	else if (!StrCmpW(L"Right", ea.text))
	{
		curText->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	}
	else if (!StrCmpW(L"Justified", ea.text))
		curText->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
}

void TControl_TDialog::OnFontText(TControl * tc, EventArgs ea)
{
	TrecPointer<TText> curText = getCurrentText();
	if (!curText.get() || !tc)
		return;
	if (!ea.text)
		return;

	curText->setFont(ea.text);
}

void TControl_TDialog::OnFontSizeText(TControl * tc, EventArgs ea)
{
	TrecPointer<TText> curText = getCurrentText();
	if (!curText.get() || !tc)
		return;
	if (!ea.text)
		return;

	float size = curText->getFontSize();
	TString sizeInText = ea.text;
	sizeInText.ConvertToFloat(&size);
	curText->setNewFontSize(size);
}

bool TControl_TDialog::getTargetColor(D2D1_COLOR_F & color, targetComponent target)
{
	switch (stateMode)
	{
	case messageState::normal:
		switch (target)
		{
		case targetComponent::get_border:
			if (border1.get())
			{
				color = border1->getColor();
				return true;
			}
			break;
		case targetComponent::get_content:
			if (content1.get())
			{
				color = content1->getColor();
				return true;
			}
			break;
		case targetComponent::get_text:
			if (text1.get())
			{
				color = text1->getColor();
				return true;
			}
		}
		break;
	case messageState::mouseHover:
		switch (target)
		{
		case targetComponent::get_border:
			if (border2.get())
			{
				color = border2->getColor();
				return true;
			}
			break;
		case targetComponent::get_content:
			if (content2.get())
			{
				color = content2->getColor();
				return true;
			}
			break;
		case targetComponent::get_text:
			if (text2.get())
			{
				color = text2->getColor();
				return true;
			}
		}
		break;
	case messageState::mouseLClick:
		switch (target)
		{
		case targetComponent::get_border:
			if (border3.get())
			{
				color = border3->getColor();
				return true;
			}
			break;
		case targetComponent::get_content:
			if (content3.get())
			{
				color = content3->getColor();
				return true;
			}
			break;
		case targetComponent::get_text:
			if (text3.get())
			{
				color = text3->getColor();
				return true;
			}
		}
		break;
	}
	return false;
}

void TControl_TDialog::setTargetColor(D2D1_COLOR_F color, targetComponent target)
{
	switch (stateMode)
	{
	case normal:
		switch (target)
		{
		case get_content:
			if (content1.get())
			{
				content1->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				RECT loc = content1->getLocation();
				content1->onCreate(loc, loc);
			}
			return;
		case get_border:
			if (border1.get())
			{
				border1->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				border1->onCreate(border1->getLocation());
			}
			return;
		case get_text:
			if (text1.get())
			{
				text1->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				text1->onCreate(text1->getLocation());
			}
		}
		return;
	case mouseHover:
		switch (target)
		{
		case get_content:
			if (content2.get())
			{
				content2->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				RECT loc = content2->getLocation();
				content2->onCreate(loc, loc);
			}
			return;
		case get_border:
			if (border2.get())
			{
				border2->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				border2->onCreate(border2->getLocation());
			}
			return;
		case get_text:
			if (text2.get())
			{
				text2->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				text2->onCreate(text2->getLocation());
			}
		}
		return;
	case mouseLClick:
		switch (target)
		{
		case get_content:
			if (content3.get())
			{
				content3->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				RECT loc = content3->getLocation();
				content3->onCreate(loc, loc);
			}
			return;
		case get_border:
			if (border3.get())
			{
				border3->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				border3->onCreate(border3->getLocation());
			}
			return;
		case get_text:
			if (text3.get())
			{
				text3->setColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
				text3->onCreate(text3->getLocation());
			}
		}
		return;
	}


}

void TControl_TDialog::updateText(TString & newText)
{
	switch (stateMode)
	{
	case normal:
		if (text1.get())
			text1->setCaption(newText);
		break;
	case mouseHover:
		if (text2.get())
			text2->setCaption(newText);
		break;
	case mouseLClick:
		if (text3.get())
			text3->setCaption(newText);
		break;
	}
}

TrecPointer<TControl> TControl_TDialog::GetControl()
{
	return controlPointer;
}

void TControl_TDialog::MessageHandler()
{

	TControl* tc = nullptr;
	int e_id = -1;
	EventArgs ea;
	for (UINT c = 0; c < EventCred.Size();c++)
	{
		tc = EventCred.at(c).control;
		if (!tc)
			continue;
		ea = tc->getEventArgs();
		e_id = ea.methodID;
		// At this point, call the appropriate method
		if (e_id > -1 && e_id < TCONTROL_DIALOG_METHOD_COUNT)
		{
			// call method
			if (dialogMethods[e_id])
				(this->*dialogMethods[e_id])(tc, ea);
		}
	}

	onDraw();
	EventCred.RemoveAll();
}

void TControl_TDialog::setCheckboxes(bool b)
{
	if (specControls.sec1.get())
		specControls.sec1->setActive(b);
	if (specControls.sec2.get())
		specControls.sec2->setActive(b);
	if (specControls.sec3.get())
		specControls.sec3->setActive(b);
}

void TControl_TDialog::enableDesableCheck()
{
	EventArgs ea;
	ZeroMemory_(ea);


	OnCheckBorder(nullptr, ea);
	OnCheckContent(nullptr, ea);
	OnCheckText(nullptr, ea);
}

void TControl_TDialog::setTextInControl(bool set)
{
	if (!controlPointer.get())
		return;

	switch (stateMode)
	{
	case normal:
		if (set && controlPointer->text1.get())
			return;
		if (!set && !controlPointer->text1.get())
			return;
		if (set)
		{
			if (!text1.get())
			{
				text1 = new TText(controlPointer->renderTarget, controlPointer.get());
			}
			controlPointer->text1 = text1;
			if (specControls.control.get())
			{
				specControls.control->text1 = text1;
				text1->onCreate(specControls.control->location);
			}
		}
		else
		{
			if(!text1.get())
				text1 = controlPointer->text1;
			controlPointer->text1 = NULL;
			if (specControls.control.get())
				specControls.control->text1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->text2.get())
			return;
		if (!set && !controlPointer->text2.get())
			return;
		if (set)
		{
			if (!text2.get())
				text2 = new TText(controlPointer->renderTarget, controlPointer.get());
			controlPointer->text2 = text2;
			if (specControls.control.get())
			{
				specControls.control->text2 = text2;
				text2->onCreate(specControls.control->location);
			}
		}
		else
		{
			if(!text2.get())
				text2 = controlPointer->text2;
			controlPointer->text2 = NULL;
			if (specControls.control.get())
				specControls.control->text2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->text3.get())
			return;
		if (!set && !controlPointer->text3.get())
			return;
		if (set)
		{
			if (!text3.get())
				text3 =new  TText(controlPointer->renderTarget, controlPointer.get());
			controlPointer->text3 = text3;
			if (specControls.control.get())
			{
				specControls.control->text3 = text3;
				text3->onCreate(specControls.control->location);
			}
		}
		else
		{
			if(!text3.get())
				text3 = controlPointer->text3;
			controlPointer->text3 = NULL;
			if (specControls.control.get())
				specControls.control->text3 = NULL;
		}
	}
	if (specControls.control.get())
		specControls.control->SetToRenderTarget();
}

void TControl_TDialog::setBorderInControl(bool set)
{
	if (!controlPointer.get())
		return;

	switch (stateMode)
	{
	case normal:
		if (set && controlPointer->border1.get())
			return;
		if (!set && !controlPointer->border1.get())
			return;
		if (set)
		{
			if (!border1.get())
				border1 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border1 = border1;
			if (specControls.control.get())
			{
				specControls.control->border1 = border1;
				border1->onCreate(specControls.control->location);
			}
		}
		else
		{
			if(!border1.get())
				border1 = controlPointer->border1;
			controlPointer->border1 = NULL;
			if (specControls.control.get())
				specControls.control->border1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->border2.get())
			return;
		if (!set && !controlPointer->border2.get())
			return;
		if (set)
		{
			if (!border2.get())
				border2 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border2 = border2;
			if (specControls.control.get())
			{
				specControls.control->border2 = border2;
				border2->onCreate(specControls.control->location);
			}
		}
		else 
		{
			if(!border2.get())
				border2 = controlPointer->border2;
			controlPointer->border2 = NULL;
			if (specControls.control.get())
				specControls.control->border2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->border3.get())
			return;
		if (!set && !controlPointer->border3.get())
			return;
		if (set)
		{
			if (!border3.get())
				border3 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border3 = border3;
			if (specControls.control.get())
			{
				specControls.control->border3 = border3;
				border3->onCreate(specControls.control->location);
			}
		}
		else
		{
			if(!border3.get())
				border3 = controlPointer->border3;
			controlPointer->border3 = NULL;
			if (specControls.control.get())
				specControls.control->border3 = NULL;
		}
	}
	if (specControls.control.get())
		specControls.control->SetToRenderTarget();
}

void TControl_TDialog::setContentInControl(bool set)
{
	if (!controlPointer.get())
		return;

	switch (stateMode)
	{
	case normal:
		if (set && controlPointer->content1.get())
			return;
		if (!set && !controlPointer->content1.get())
			return;
		if (set)
		{
			if (!content1.get())
				content1 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content1 = content1;
			if (specControls.control.get())
			{
				specControls.control->content1 = content1;
				content1->onCreate(specControls.control->location,
								specControls.control->location);
			}
		}
		else
		{
			if(!content1.get())
				content1 = controlPointer->content1;
			controlPointer->content1 = NULL;
			if (specControls.control.get())
				specControls.control->content1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->content2.get())
			return;
		if (!set && !controlPointer->content2.get())
			return;
		if (set)
		{
			if (!content2.get())
				content2 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content2 = content2;
			if (specControls.control.get())
			{
				specControls.control->content2 = content2;
				content2->onCreate(specControls.control->location,
					specControls.control->location);
			}
		}
		else
		{
			if(!content2.get())
				content2 = controlPointer->content2;
			controlPointer->content2 = NULL;
			if (specControls.control.get())
				specControls.control->content2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->content3.get())
			return;
		if (!set && !controlPointer->content3.get())
			return;
		if (set)
		{
			if (!content3.get())
				content3 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content3 = content3;
			if (specControls.control.get())
			{
				specControls.control->content3 = content3;
				content3->onCreate(specControls.control->location,
					specControls.control->location);
			}
		}
		else
		{
			if(!content3.get())
				content3 = controlPointer->content3;
			controlPointer->content3 = NULL;
			if (specControls.control.get())
				specControls.control->content3 = NULL;
		}
	}
	if (specControls.control.get())
		specControls.control->SetToRenderTarget();
}

TrecPointer<TBorder> TControl_TDialog::getCurrentBorder()
{
	switch (stateMode)
	{
	case normal:
		return border1;
	case mouseHover:
		return border2;
	case mouseLClick:
		return border3;
	}
	return nullptr;
}

TrecPointer<TContent> TControl_TDialog::getCurrentContent()
{
	switch (stateMode)
	{
	case normal:
		return content1;
	case mouseHover:
		return content2;
	case mouseLClick:
		return content3;
	}
	return nullptr;
}

TrecPointer<TText> TControl_TDialog::getCurrentText()
{
	switch (stateMode)
	{
	case normal:
		return text1;
	case mouseHover:
		return text2;
	case mouseLClick:
		return text3;
	}
	return nullptr;
}

void TControl_TDialog::initializeControls()
{
	if (!pointer.get())
		return;
	TLayout* curLay = nullptr;
	try
	{
		curLay = dynamic_cast<TLayout*>(pointer.get());
	}
	catch (std::bad_cast& e)
	{
		return;
	}
	catch (...)
	{
		return;
	}

	specControls.mode1 = curLay->GetLayoutChild(0, 0);
	specControls.mode2 = curLay->GetLayoutChild(1, 0);
	specControls.mode3 = curLay->GetLayoutChild(2, 0);

	specControls.sec1 = curLay->GetLayoutChild(0, 1);
	specControls.sec2 = curLay->GetLayoutChild(1, 1);
	specControls.sec3 = curLay->GetLayoutChild(2, 1);

	specControls.op1 = curLay->GetLayoutChild(0, 2);
	specControls.op2 = curLay->GetLayoutChild(1, 2);
	specControls.op3 = curLay->GetLayoutChild(2, 2);

	specControls.opV1 = curLay->GetLayoutChild(0, 3);
	specControls.opV2 = curLay->GetLayoutChild(1, 3);
	specControls.opV3 = curLay->GetLayoutChild(2, 3);

	specControls.red1 = curLay->GetLayoutChild(0, 4);
	specControls.red2 = curLay->GetLayoutChild(1, 4);
	specControls.red3 = curLay->GetLayoutChild(2, 4);

	specControls.green1 = curLay->GetLayoutChild(0, 5);
	specControls.green2 = curLay->GetLayoutChild(1, 5);
	specControls.green3 = curLay->GetLayoutChild(2, 5);

	specControls.blue1 = curLay->GetLayoutChild(0, 6);
	specControls.blue2 = curLay->GetLayoutChild(1, 6);
	specControls.blue3 = curLay->GetLayoutChild(2, 6);

	specControls.control = curLay->GetLayoutChild(1, 10);

	if (specControls.control.get())
	{
		RECT basicRect = specControls.control->getLocation();

		
	}

	EventArgs ea;
	ZeroMemory_(ea);
	OnCheckBorder(nullptr, ea);
	OnCheckContent(nullptr, ea);
	OnCheckText(nullptr, ea);

	setCheckboxes(false);
}

bool TControl_TDialog::Initialize(int& error)
{
	bool ret = TDialog::InitializeWindow(error);
	if (ret)
		ret = TDialog::Initialize2D(error);
	if (ret)
		ret = TDialog::InitializeAnaface(error);
	if (!controlPointer.get() && ret)
	{
		controlPointer = new TControl(regRenderTarget, table);
		initializeControls();
	}
	//control = controlPointer;
	return ret;
}
