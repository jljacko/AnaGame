// TControl_Edit.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "TControl_Edit.h"
#include "TLayoutEdit.h"
#include "afxdialogex.h"

// implement nullify functions
namespace {
	void nullifyBorderBox(borderBox* bb)
	{
		if (!bb)
			return;
		bb->color = NULL;
		bb->opaque = NULL;
		bb->style = NULL;
		bb->thickBox = NULL;
	}

	void nullifyContentBox(contentBox* cb)
	{
		if (!cb)
			return;
		cb->button = NULL;
		cb->color = NULL;
		cb->edit = NULL;
		cb->opaque = NULL;
		cb->style = NULL;
	}

	void nullifyTextBox(textBox* tb)
	{
		if (!tb)
			return;
		tb->color = NULL;
		tb->font = NULL;
		tb->fontSize = NULL;
		tb->horizSpace = NULL;
		tb->opaque = NULL;
		tb->VerticalSpace = NULL;
	}

	void nullifySizeBox(sizeBox* sb)
	{
		if (!sb)
			return;
		sb->fixHeight = NULL;
		sb->fixWidth = NULL;
		sb->four_side = NULL;
		sb->height = NULL;
		sb->heightWidth = NULL;
		sb->margin = NULL;
		sb->marginBottom = NULL;
		sb->marginLeft = NULL;
		sb->marginRight = NULL;
		sb->marginTop = NULL;
		sb->maxHeight = NULL;
		sb->maxWidth = NULL;
		sb->minHeight = NULL;
		sb->minWidth = NULL;
		sb->pointPadding = NULL;
		sb->width = NULL;

	}

	void nullifyMouseBox(mouseBox* mb)
	{
		if (!mb)
			return;
		mb->click = NULL;
		mb->hover = NULL;
		mb->normal = NULL;

	}

	void nullifyScrollBox(scrollBox* sb)
	{
		if (!sb)
			return;
		sb->hScrolling = NULL;
		sb->vScrolling = NULL;
		sb->multiScroll = NULL;
	}

	void nullifyShapeBox(shapeBox* sb)
	{
		if (!sb)
			return;
		sb->eclipse = NULL;
		sb->other = NULL;
		sb->otherButton = NULL;
		sb->rectangle = NULL;
		sb->roundedRectangle = NULL;
	}
}


// TControl_Edit dialog

IMPLEMENT_DYNAMIC(TControl_Edit, CDialogEx)

TControl_Edit::TControl_Edit(TrecPointer<TControl> contPoint,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	if (!contPoint.get())
		return;
	controlPointer = contPoint; // This dialog just expects a TControl, it doesn't care what type
	//control = contPoint->rootType;

	nullifyBorderBox(&borderGroup);
	nullifyContentBox(&contentGroup);
	nullifyMouseBox(&mouseGroup);
	nullifyScrollBox(&scrollGroup);
	nullifyShapeBox(&shapeGroup);
	nullifySizeBox(&sizeGroup);
	nullifyTextBox(&textGroup);

	borderCheck = NULL;
	contentCheck = NULL;
	textCheck = NULL;
	subClass = NULL;
	isCreated = false;
	
}

TControl_Edit::~TControl_Edit()
{
}

INT_PTR TControl_Edit::DoModal()
{
	INT_PTR returnable = CDialogEx::DoModal();
	// Set the attributes of controls that correlate to the edit controls



	return returnable;
}

TrecPointer<TControl> TControl_Edit::getControl()
{
	return controlPointer;
}

void TControl_Edit::initialize()
{
	mouseGroup.click = (CButton*)GetDlgItem(IDC_RADIO11);
	mouseGroup.hover = (CButton*)GetDlgItem(IDC_RADIO10);
	mouseGroup.normal = (CButton*)GetDlgItem(IDC_RADIO9);

	borderGroup.color = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON1);
	borderGroup.opaque = (CComboBox*)GetDlgItem(IDC_COMBO3);
	borderGroup.style = (CComboBox*)GetDlgItem(IDC_COMBO2);
	borderGroup.thickBox = (CComboBox*)GetDlgItem(IDC_COMBO1);

	contentGroup.button = (CButton*)GetDlgItem(IDC_BUTTON2);
	contentGroup.color = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON2);
	contentGroup.edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	contentGroup.opaque = (CComboBox*)GetDlgItem(IDC_COMBO5);
	contentGroup.style = (CComboBox*)GetDlgItem(IDC_COMBO4);

	scrollGroup.hScrolling = (CButton*)GetDlgItem(IDC_CHECK2);
	scrollGroup.multiScroll = (CButton*)GetDlgItem(IDC_CHECK9);
	scrollGroup.vScrolling = (CButton*)GetDlgItem(IDC_CHECK3);

	textGroup.color = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON3);
	textGroup.font = (CComboBox*)GetDlgItem(IDC_COMBO7);
	textGroup.fontSize = (CComboBox*)GetDlgItem(IDC_COMBO8);
	textGroup.horizSpace = (CComboBox*)GetDlgItem(IDC_COMBO9);
	textGroup.opaque = (CComboBox*)GetDlgItem(IDC_COMBO11);
	textGroup.VerticalSpace = (CComboBox*)GetDlgItem(IDC_COMBO10);
	textGroup.edit = (CEdit*)GetDlgItem(IDC_EDIT12);

	shapeGroup.eclipse = (CButton*)GetDlgItem(IDC_RADIO2);
	shapeGroup.other = (CButton*)GetDlgItem(IDC_RADIO4);
	shapeGroup.otherButton = (CButton*)GetDlgItem(IDC_BUTTON3);
	shapeGroup.rectangle = (CButton*)GetDlgItem(IDC_RADIO1);
	shapeGroup.roundedRectangle=(CButton*)GetDlgItem(IDC_RADIO3);

	sizeGroup.heightWidth = (CButton*)GetDlgItem(IDC_RADIO5);
	sizeGroup.margin = (CButton*)GetDlgItem(IDC_RADIO6);
	sizeGroup.four_side = (CButton*)GetDlgItem(IDC_RADIO7);
	sizeGroup.pointPadding = (CButton*)GetDlgItem(IDC_RADIO8);
	sizeGroup.fixHeight = (CButton*)GetDlgItem(IDC_CHECK7);
	sizeGroup.fixWidth = (CButton*)GetDlgItem(IDC_CHECK8);

	sizeGroup.height = (CEdit*)GetDlgItem(IDC_EDIT2);
	sizeGroup.width = (CEdit*)GetDlgItem(IDC_EDIT3);
	sizeGroup.maxHeight = (CEdit*)GetDlgItem(IDC_EDIT4);
	sizeGroup.minHeight = (CEdit*)GetDlgItem(IDC_EDIT5);
	sizeGroup.maxWidth = (CEdit*)GetDlgItem(IDC_EDIT6);
	sizeGroup.minWidth = (CEdit*)GetDlgItem(IDC_EDIT7);
	sizeGroup.marginTop = (CEdit*)GetDlgItem(IDC_EDIT8);
	sizeGroup.marginLeft = (CEdit*)GetDlgItem(IDC_EDIT9);
	sizeGroup.marginBottom = (CEdit*)GetDlgItem(IDC_EDIT10);
	sizeGroup.marginRight = (CEdit*)GetDlgItem(IDC_EDIT11);

	borderCheck = (CButton*)GetDlgItem(IDC_CHECK4); // Border
	contentCheck = (CButton*)GetDlgItem(IDC_CHECK5); // Content
	textCheck = (CButton*)GetDlgItem(IDC_CHECK6); // text
	subClass = (CButton*)GetDlgItem(IDC_BUTTON1); // subclass

	text1 = controlPointer->text1;
	text2 = controlPointer->text2;
	text3 = controlPointer->text3;
	border1 = controlPointer->border1;
	border2 = controlPointer->border2;
	border3 = controlPointer->border3;
	content1 = controlPointer->content1;
	content2 = controlPointer->content2;
	content3 = controlPointer->content3;

	// Get information about margin and size and apply it to the text boxes
	// in the margin section
	if (controlPointer.get() && controlPointer->dimensions)
	{
		sizeControl* sc = controlPointer->dimensions;
		CString string;
		int values[] = { sc->height, sc->width, sc->maxHeight, sc->minHeight, sc->maxWidth, sc->minWidth };

		UINT boxes[] = { IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6, IDC_EDIT7 };

		for (int c = 0; c < 6;c++)
		{
			string.Format(_T("%d"), values[c]);
			SetDlgItemTextW(boxes[c], string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
	}
	RECT* mar = &(controlPointer->margin);
	int values[] = { mar->top, mar->left, mar->bottom, mar->right };

	UINT boxes[] = { IDC_EDIT8, IDC_EDIT9, IDC_EDIT10, IDC_EDIT11 };
	CString string;
	for (int c = 0; c < 6;c++)
	{
		string.Format(_T("%d"), values[c]);
		SetDlgItemTextW(boxes[c], string.GetBuffer());
		string.ReleaseBuffer();
		string.Empty();
	}



	PrepControls();
}

void TControl_Edit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL TControl_Edit::OnInitDialog()
{
	BOOL returnable = CDialogEx::OnInitDialog();
	initialize();
	return returnable;
}


BEGIN_MESSAGE_MAP(TControl_Edit, CDialogEx)
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &TControl_Edit::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_CHECK4, &TControl_Edit::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &TControl_Edit::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &TControl_Edit::OnBnClickedMfccolorbutton1)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &TControl_Edit::OnBnClickedMfccolorbutton2)
	ON_BN_CLICKED(IDC_RADIO9, &TControl_Edit::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &TControl_Edit::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &TControl_Edit::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_CHECK6, &TControl_Edit::OnBnClickedCheck6)
	ON_CBN_SELCHANGE(IDC_COMBO1, &TControl_Edit::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &TControl_Edit::OnCbnSelchangeOpaqBorder)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &TControl_Edit::OnBnClickedMfccolorbutton3)
	ON_CBN_SELCHANGE(IDC_COMBO7, &TControl_Edit::OnCbnSelchangeCombo7)
	ON_CBN_SELCHANGE(IDC_COMBO8, &TControl_Edit::OnCbnSelchangeCombo8)
	ON_CBN_SELCHANGE(IDC_COMBO9, &TControl_Edit::OnCbnSelchangeCombo9)
	ON_CBN_SELCHANGE(IDC_COMBO10, &TControl_Edit::OnCbnSelchangeCombo10)
	ON_CBN_SELCHANGE(IDC_COMBO11, &TControl_Edit::OnCbnSelchangeCombo11)
	ON_EN_CHANGE(IDC_EDIT12, &TControl_Edit::OnEnChangeEdit12)
	ON_CBN_SELCHANGE(IDC_COMBO5, &TControl_Edit::OnCbnSelchangeContentOpaque)
	ON_BN_CLICKED(IDC_CHECK7, &TControl_Edit::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &TControl_Edit::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_RADIO5, &TControl_Edit::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &TControl_Edit::OnBnClickedRadio6)
	ON_BN_CLICKED(IDOK, &TControl_Edit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &TControl_Edit::OnBnClickedButton1)
END_MESSAGE_MAP()


// TControl_Edit message handlers

void TControl_Edit::enableDesableCheck()
{
	OnBnClickedCheck4();
	OnBnClickedCheck5();
	OnBnClickedCheck6();
}

void TControl_Edit::setTextInControl(bool set)
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
		if (set && text1.get())
			controlPointer->text1 = text1;
		else if (set && !text1.get())
		{
			text1 = new TText(controlPointer->renderTarget, controlPointer.get());
			controlPointer->text1 = text1;
		}
		else if (!set &&text1.get())
			controlPointer->text1 = NULL;
		else if (!set && !text1.get())
		{
			text1 = controlPointer->text1;
			controlPointer->text1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->text2.get())
			return;
		if (!set && !controlPointer->text2.get())
			return;
		if (set && text2.get())
			controlPointer->text2 = text2;
		else if (set && !text2.get())
		{
			text2 = new TText(controlPointer->renderTarget, controlPointer.get());
			controlPointer->text2 = text2;
		}
		else if (!set &&text2.get())
			controlPointer->text2 = NULL;
		else if (!set && !text2.get())
		{
			text2 = controlPointer->text2;
			controlPointer->text2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->text3.get())
			return;
		if (!set && !controlPointer->text3.get())
			return;
		if (set && text3.get())
			controlPointer->text3 = text3;
		else if (set && !text3.get())
		{
			text3 = new TText(controlPointer->renderTarget, controlPointer.get());
			controlPointer->text3 = text3;
		}
		else if (!set &&text3.get())
			controlPointer->text3 = NULL;
		else if (!set && !text3.get())
		{
			text3 = controlPointer->text3;
			controlPointer->text3 = NULL;
		}
	}
}

void TControl_Edit::setBorderInControl(bool set)
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
		if (set && border1.get())
			controlPointer->border1 = border1;
		else if (set && !border1.get())
		{
			border1 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border1 = border1;
		}
		else if (!set &&border1.get())
			controlPointer->border1 = NULL;
		else if (!set && !border1.get())
		{
			border1 = controlPointer->border1;
			controlPointer->border1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->border2.get())
			return;
		if (!set && !controlPointer->border2.get())
			return;
		if (set && border2.get())
			controlPointer->border2 = border2;
		else if (set && !border2.get())
		{
			border2 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border2 = border2;
		}
		else if (!set &&border2.get())
			controlPointer->border2 = NULL;
		else if (!set && !border2.get())
		{
			border2 = controlPointer->border2;
			controlPointer->border2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->border3.get())
			return;
		if (!set && !controlPointer->border3.get())
			return;
		if (set && border3.get())
			controlPointer->border3 = border3;
		else if (set && !border3.get())
		{
			border3 = new TBorder(controlPointer->renderTarget, controlPointer.get());
			controlPointer->border3 = border3;
		}
		else if (!set &&border3.get())
			controlPointer->border3 = NULL;
		else if (!set && !border3.get())
		{
			border3 = controlPointer->border3;
			controlPointer->border3 = NULL;
		}
	}
}

void TControl_Edit::setContentInControl(bool set)
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
		if (set && content1.get())
			controlPointer->content1 = content1;
		else if (set && !content1.get())
		{
			content1 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content1 = content1;
		}
		else if (!set &&content1.get())
			controlPointer->content1 = NULL;
		else if (!set && !content1.get())
		{
			content1 = controlPointer->content1;
			controlPointer->content1 = NULL;
		}
		break;
	case mouseHover:
		if (set && controlPointer->content2.get())
			return;
		if (!set && !controlPointer->content2.get())
			return;
		if (set && content2.get())
			controlPointer->content2 = content2;
		else if (set && !content2.get())
		{
			content2 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content2 = content2;
		}
		else if (!set &&content2.get())
			controlPointer->content2 = NULL;
		else if (!set && !content2.get())
		{
			content2 = controlPointer->content2;
			controlPointer->content2 = NULL;
		}
		break;

	case mouseLClick:
		if (set && controlPointer->content3.get())
			return;
		if (!set && !controlPointer->content3.get())
			return;
		if (set && content3.get())
			controlPointer->content3 = content3;
		else if (set && !content3.get())
		{
			content3 = new TContent(controlPointer->renderTarget, controlPointer.get());
			controlPointer->content3 = content3;
		}
		else if (!set &&content3.get())
			controlPointer->content3 = NULL;
		else if (!set && !content3.get())
		{
			content3 = controlPointer->content3;
			controlPointer->content3 = NULL;
		}
	}
}

TrecPointer<TBorder> TControl_Edit::getCurrentBorder()
{
	if (!controlPointer.get())
		return nullptr;
	switch (stateMode)
	{
	case normal:
	case mouseRClick:
		return controlPointer->border1;
	case mouseHover:
		return controlPointer->border2;
	case mouseLClick:
		return controlPointer->border3;
	}
	return nullptr;
}

TrecPointer<TContent> TControl_Edit::getCurrentContent()
{
	if (!controlPointer.get())
		return nullptr;
	switch (stateMode)
	{
	case normal:
	case mouseRClick:
		return controlPointer->content1;
	case mouseHover:
		return controlPointer->content2;
	case mouseLClick:
		return controlPointer->content3;
	}
	return nullptr;
}

TrecPointer<TText> TControl_Edit::getCurrentText()
{
	if (!controlPointer.get())
		return nullptr;
	switch (stateMode)
	{
	case normal:
	case mouseRClick:
		return controlPointer->text1;
	case mouseHover:
		return controlPointer->text2;
	case mouseLClick:
		return controlPointer->text3;
	}
	return nullptr;
}

void TControl_Edit::setCurrentText()
{
	if (IsDlgButtonChecked(IDC_CHECK6) && textGroup.edit && textGroup.edit->IsWindowEnabled())
	{
		TString string;
		textGroup.edit->GetWindowTextW(string);
		switch (stateMode)
		{
		case normal:
			if (text1.get())
				text1->setCaption(string);
			break;
		case mouseHover:
			if (text2.get())
				text2->setCaption(string);
			break;
		case mouseLClick:
			if (text3.get())
				text3->setCaption(string);
		}
	}
}

void TControl_Edit::PrepControls()
{
	switch (stateMode)
	{
	case normal:
	case mouseRClick:
		if (text1.get())
		{
			CheckDlgButton(IDC_CHECK6, true);
			D2D1::ColorF d2Col = text1->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			textGroup.color->SetColor(col);

			float op = text1->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO11, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			switch (text1->getHorizontalAlignment())
			{
			case DWRITE_TEXT_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO9, L"Center");
				break;
			case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
				SetDlgItemTextW(IDC_COMBO9, L"Justified");
				break;
			case DWRITE_TEXT_ALIGNMENT_LEADING:
				SetDlgItemTextW(IDC_COMBO9, L"Right");
				break;
			case DWRITE_TEXT_ALIGNMENT_TRAILING:
				SetDlgItemTextW(IDC_COMBO9, L"Left");
			}
			switch (text1->getVerticalAlignment())
			{
			case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO10, L"Center");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
				SetDlgItemTextW(IDC_COMBO10, L"Bottom");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
				SetDlgItemTextW(IDC_COMBO10, L"Top");
			}

			int font = static_cast<int>(text1->getFontSize());
			string.Format(_T("%d"), font);
			SetDlgItemTextW(IDC_COMBO8, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
			TString wfont = text1->getFont();
			SetDlgItemTextW(IDC_COMBO7, wfont);
			TString newText = text1->getCaption();
			textGroup.edit->SetWindowTextW(newText);

		}
		else
			CheckDlgButton(IDC_CHECK6, false);

		if (content1.get())
		{
			CheckDlgButton(IDC_CHECK5, true);
			D2D1::ColorF d2Col = content1->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			contentGroup.color->SetColor(col);

			float op = content1->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO5, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();


		}
		else
			CheckDlgButton(IDC_CHECK5, false);

		if (border1.get())
		{

			CheckDlgButton(IDC_CHECK4, true);
			D2D1::ColorF d2Col = border1->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			borderGroup.color->SetColor(col);

			float op = border1->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO3, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			op = border1->getThickness();
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO1, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
		else
			CheckDlgButton(IDC_CHECK4, false);

		break;
	case mouseHover:
		if (text2.get())
		{
			CheckDlgButton(IDC_CHECK6, true);
			D2D1::ColorF d2Col = text2->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			textGroup.color->SetColor(col);

			float op = text2->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO11, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			switch (text2->getHorizontalAlignment())
			{
			case DWRITE_TEXT_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO9, L"Center");
				break;
			case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
				SetDlgItemTextW(IDC_COMBO9, L"Justified");
				break;
			case DWRITE_TEXT_ALIGNMENT_LEADING:
				SetDlgItemTextW(IDC_COMBO9, L"Right");
				break;
			case DWRITE_TEXT_ALIGNMENT_TRAILING:
				SetDlgItemTextW(IDC_COMBO9, L"Left");
			}
			switch (text2->getVerticalAlignment())
			{
			case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO10, L"Center");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
				SetDlgItemTextW(IDC_COMBO10, L"Bottom");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
				SetDlgItemTextW(IDC_COMBO10, L"Top");
			}

			int font = static_cast<int>(text2->getFontSize());
			string.Format(_T("%d"), font);
			SetDlgItemTextW(IDC_COMBO8, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
			TString wfont = text2->getFont();
			SetDlgItemTextW(IDC_COMBO7, wfont);
			TString newText = text2->getCaption();
			textGroup.edit->SetWindowTextW(newText);

		}
		else
			CheckDlgButton(IDC_CHECK6, false);

		if (content2.get())
		{
			CheckDlgButton(IDC_CHECK5, true);
			D2D1::ColorF d2Col = content2->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			contentGroup.color->SetColor(col);

			float op = content2->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO5, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
		else
			CheckDlgButton(IDC_CHECK5, false);

		if (border2.get())
		{
			CheckDlgButton(IDC_CHECK4, true);
			D2D1::ColorF d2Col = border2->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			borderGroup.color->SetColor(col);

			float op = border2->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO3, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			op = border2->getThickness();
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO1, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
		else
			CheckDlgButton(IDC_CHECK4, false);
		break;
	case mouseLClick:
		if (text3.get())
		{
			CheckDlgButton(IDC_CHECK6, true);
			D2D1::ColorF d2Col = text3->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			textGroup.color->SetColor(col);

			float op = text3->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO11, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			switch (text3->getHorizontalAlignment())
			{
			case DWRITE_TEXT_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO9, L"Center");
				break;
			case DWRITE_TEXT_ALIGNMENT_JUSTIFIED:
				SetDlgItemTextW(IDC_COMBO9, L"Justified");
				break;
			case DWRITE_TEXT_ALIGNMENT_LEADING:
				SetDlgItemTextW(IDC_COMBO9, L"Right");
				break;
			case DWRITE_TEXT_ALIGNMENT_TRAILING:
				SetDlgItemTextW(IDC_COMBO9, L"Left");
			}
			switch (text3->getVerticalAlignment())
			{
			case DWRITE_PARAGRAPH_ALIGNMENT_CENTER:
				SetDlgItemTextW(IDC_COMBO10, L"Center");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_FAR:
				SetDlgItemTextW(IDC_COMBO10, L"Bottom");
				break;
			case DWRITE_PARAGRAPH_ALIGNMENT_NEAR:
				SetDlgItemTextW(IDC_COMBO10, L"Top");
			}

			int font = static_cast<int>(text3->getFontSize());
			string.Format(_T("%d"), font);
			SetDlgItemTextW(IDC_COMBO8, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
			TString wfont = text3->getFont();
			SetDlgItemTextW(IDC_COMBO7, wfont);
			TString newText = text3->getCaption();
			textGroup.edit->SetWindowTextW(newText);

		}
		else
			CheckDlgButton(IDC_CHECK6, false);

		if (content3.get())
		{
			CheckDlgButton(IDC_CHECK5, true);
			D2D1::ColorF d2Col = content3->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			contentGroup.color->SetColor(col);

			float op = content3->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO5, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
		else
			CheckDlgButton(IDC_CHECK5, false);

		if (border3.get())
		{
			CheckDlgButton(IDC_CHECK4, true);
			D2D1::ColorF d2Col = border3->getColor();
			COLORREF col = RGB(d2Col.r, d2Col.g, d2Col.b);
			borderGroup.color->SetColor(col);

			float op = border3->getColor().a;
			CString string;
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO3, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();

			op = border3->getThickness();
			string.Format(_T("%f"), op);
			SetDlgItemTextW(IDC_COMBO1, string.GetBuffer());
			string.ReleaseBuffer();
			string.Empty();
		}
		else
			CheckDlgButton(IDC_CHECK4, false);
	}
	enableDesableCheck();
}

void TControl_Edit::OnBnClickedCheck4()
{
	// Border Properties
	if (IsDlgButtonChecked(IDC_CHECK4))
	{
		if (borderGroup.color)
			borderGroup.color->EnableWindow(TRUE);
		if (borderGroup.opaque)
			borderGroup.opaque->EnableWindow(TRUE);
		if (borderGroup.style)
			borderGroup.style->EnableWindow(TRUE);
		if (borderGroup.thickBox)
			borderGroup.thickBox->EnableWindow(TRUE);
		setBorderInControl(true);
	}
	else
	{
		if (borderGroup.color)
			borderGroup.color->EnableWindow(FALSE);
		if (borderGroup.opaque)
			borderGroup.opaque->EnableWindow(FALSE);
		if (borderGroup.style)
			borderGroup.style->EnableWindow(FALSE);
		if (borderGroup.thickBox)
			borderGroup.thickBox->EnableWindow(FALSE);
		setBorderInControl(false);
	}

}

void TControl_Edit::OnBnClickedCheck5()
{
	// Content Propeties
	if (IsDlgButtonChecked(IDC_CHECK5))
	{
		if (contentGroup.style)
			contentGroup.style->EnableWindow(TRUE);
		if (contentGroup.button)
			contentGroup.button->EnableWindow(TRUE);
		if (contentGroup.color)
			contentGroup.color->EnableWindow(TRUE);
		if (contentGroup.edit)
			contentGroup.edit->EnableWindow(TRUE);
		if (contentGroup.opaque)
			contentGroup.opaque->EnableWindow(TRUE);

		setContentInControl(true);
	}
	else
	{
		if (contentGroup.style)
			contentGroup.style->EnableWindow(FALSE);
		if (contentGroup.button)
			contentGroup.button->EnableWindow(FALSE);
		if (contentGroup.color)
			contentGroup.color->EnableWindow(FALSE);
		if (contentGroup.edit)
			contentGroup.edit->EnableWindow(FALSE);
		if (contentGroup.opaque)
			contentGroup.opaque->EnableWindow(FALSE);

		setContentInControl(false);
	}

}

void TControl_Edit::OnBnClickedMfccolorbutton1()
{
	if (!controlPointer.get())
		return;
	if (borderGroup.color)
	{
		COLORREF cf = borderGroup.color->GetColor();
		D2D1::ColorF d2Color = D2D1::ColorF(GetRValue(cf), GetGValue(cf), GetBValue(cf), 1.0);
		TrecPointer<TBorder> border = getCurrentBorder();

		

		if (!border.get())
			return;

		// Chance the color, not the opaquency
		d2Color.a = border->getColor().a;

		border->setColor(d2Color);
	}
}

void TControl_Edit::OnBnClickedMfccolorbutton2()
{
	if (!controlPointer.get())
		return;
	if (contentGroup.color)
	{
		COLORREF cf = contentGroup.color->GetColor();
		D2D1::ColorF d2Color = D2D1::ColorF(GetRValue(cf), GetGValue(cf), GetBValue(cf), 1.0);
		TrecPointer<TContent> content = getCurrentContent();



		if (!content.get())
			return;

		// Chance the color, not the opaquency
		d2Color.a = content->getColor().a;

		content->setColor(d2Color);
	}
}

void TControl_Edit::OnBnClickedRadio9()
{
	setCurrentText();

	// TODO: Add your control notification handler code here
	stateMode = normal;
	if (controlPointer.get())
	{
		if (controlPointer->text1.get())
			CheckDlgButton(IDC_CHECK6, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK6, BST_UNCHECKED);

		if (controlPointer->border1.get())
			CheckDlgButton(IDC_CHECK4, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK4, BST_UNCHECKED);

		if (controlPointer->content1.get())
			CheckDlgButton(IDC_CHECK5, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK5, BST_UNCHECKED);

		PrepControls();
	}
}

void TControl_Edit::OnBnClickedRadio10()
{
	setCurrentText();
	stateMode = mouseHover;
	if (controlPointer.get())
	{
		
		if (controlPointer->text2.get())
			CheckDlgButton(IDC_CHECK6, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK6, BST_UNCHECKED);

		if (controlPointer->border2.get())
			CheckDlgButton(IDC_CHECK4, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK4, BST_UNCHECKED);

		if (controlPointer->content2.get())
			CheckDlgButton(IDC_CHECK5, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK5, BST_UNCHECKED);

		PrepControls();
	}
}

void TControl_Edit::OnBnClickedRadio11()
{
	setCurrentText();
	stateMode = mouseLClick;
	if (controlPointer.get())
	{
		if (controlPointer->text3.get())
			CheckDlgButton(IDC_CHECK6, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK6, BST_UNCHECKED);

		if (controlPointer->border3.get())
			CheckDlgButton(IDC_CHECK4, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK4, BST_UNCHECKED);

		if (controlPointer->content3.get())
			CheckDlgButton(IDC_CHECK5, BST_CHECKED);
		else
			CheckDlgButton(IDC_CHECK5, BST_UNCHECKED);

		PrepControls();
	}
}

void TControl_Edit::OnBnClickedCheck6()
{
	// Text Propeties
	if (IsDlgButtonChecked(IDC_CHECK6))
	{
		if (textGroup.color)
			textGroup.color->EnableWindow(TRUE);
		if (textGroup.font)
			textGroup.font->EnableWindow(TRUE);
		if (textGroup.fontSize)
			textGroup.fontSize->EnableWindow(TRUE);
		if (textGroup.horizSpace)
			textGroup.horizSpace->EnableWindow(TRUE);
		if (textGroup.opaque)
			textGroup.opaque->EnableWindow(TRUE);
		if (textGroup.VerticalSpace)
			textGroup.VerticalSpace->EnableWindow(TRUE);

		setTextInControl(true);
	}
	else
	{
		if (textGroup.color)
			textGroup.color->EnableWindow(FALSE);
		if (textGroup.font)
			textGroup.font->EnableWindow(FALSE);
		if (textGroup.fontSize)
			textGroup.fontSize->EnableWindow(FALSE);
		if (textGroup.horizSpace)
			textGroup.horizSpace->EnableWindow(FALSE);
		if (textGroup.opaque)
			textGroup.opaque->EnableWindow(FALSE);
		if (textGroup.VerticalSpace)
			textGroup.VerticalSpace->EnableWindow(FALSE);

		setTextInControl(false);
	}
}

void TControl_Edit::OnCbnSelchangeCombo1()
{
	if (!controlPointer.get())
		return;
	if (!borderGroup.thickBox)
		return;
	//borderGroup.thickBox->G
	TString selectionText;
	int current = borderGroup.thickBox->GetCurSel();
	int len = borderGroup.thickBox->GetLBTextLen(current);
	borderGroup.thickBox->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	int i = 0;
	float f = 0.0;

	TrecPointer<TBorder> border = getCurrentBorder();
	if (!selectionText.ConvertToInt(&i))
	{
		f = static_cast<float>(i);
	}
	else if (selectionText.ConvertToFloat(&f))
		return;
	border->setThickness(f);
}

void TControl_Edit::OnCbnSelchangeOpaqBorder()
{
	TString selectionText;
	int current = borderGroup.opaque->GetCurSel();
	if (current < 0)
		return;
	int len = borderGroup.opaque->GetLBTextLen(current);
	borderGroup.opaque->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	int i = 0;
	float f = 0.0;

	TrecPointer<TBorder> border = getCurrentBorder();
	if (!selectionText.ConvertToInt(&i))
	{
		f = static_cast<float>(i);
	}
	else if (selectionText.ConvertToFloat(&f))
		return;

	border->setOpaquency(f);
}

void TControl_Edit::OnBnClickedMfccolorbutton3()
{
	// TODO: Add your control notification handler code here
	if (!controlPointer.get())
		return;
	if (borderGroup.color)
	{
		COLORREF cf = borderGroup.color->GetColor();
		D2D1::ColorF d2Color = D2D1::ColorF(GetRValue(cf), GetGValue(cf), GetBValue(cf), 1.0);
		TrecPointer<TText> text = getCurrentText();



		if (!text.get())
			return;

		// Chance the color, not the opaquency
		d2Color.a = text->getColor().a;

		text->setColor(d2Color);
	}
}

void TControl_Edit::OnCbnSelchangeCombo7()
{
	// TODO: Add your control notification handler code here
	TString selectionText;
	int current = textGroup.font->GetCurSel();
	if (current < 0)
		return;
	int len = textGroup.font->GetLBTextLen(current);
	textGroup.font->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	TrecPointer<TText> text = getCurrentText();
	if (!text.get())
		return;

	text->setNewFont(selectionText);
	selectionText.ReleaseBuffer();
}

void TControl_Edit::OnCbnSelchangeCombo8()
{
	TString selectionText;
	int current = textGroup.fontSize->GetCurSel();
	if (current < 0)
		return;
	int len = textGroup.fontSize->GetLBTextLen(current);
	textGroup.fontSize->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	int i = 0;
	float f = 0.0;

	TrecPointer<TText> text = getCurrentText();
	if (!selectionText.ConvertToInt(&i))
	{
		f = static_cast<float>(i);
	}
	else if (selectionText.ConvertToFloat(&f))
		return;

	text->setNewFontSize(f);
}

void TControl_Edit::OnCbnSelchangeCombo9()
{
	TString selectionText;
	int current = textGroup.horizSpace->GetCurSel();
	if (current < 0)
		return;
	int len = textGroup.horizSpace->GetLBTextLen(current);
	textGroup.horizSpace->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();
	TrecPointer<TText> text = getCurrentText();
	if (!text.get())
		return;

	if (!selectionText.Compare(L"Left"))
	{
		text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	}
	else if (!selectionText.Compare(L"Right"))
	{
		text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	}
	else if (!selectionText.Compare(L"Justified"))
	{
		text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	}
	else
	{
		text->setNewHorizontalAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}
}

void TControl_Edit::OnCbnSelchangeCombo10()
{
	TString selectionText;
	int current = textGroup.VerticalSpace->GetCurSel();
	if (current < 0)
		return;
	int len = textGroup.VerticalSpace->GetLBTextLen(current);
	textGroup.VerticalSpace->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();
	TrecPointer<TText> text = getCurrentText();
	if (!text.get())
		return;

	if (!selectionText.Compare(L"Top"))
	{
		text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}
	else if (!selectionText.Compare(L"Bottom"))
	{
		text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	}
	else
	{
		text->setNewVerticalAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void TControl_Edit::OnCbnSelchangeCombo11()
{
	// TODO: Add your control notification handler code here
	TString selectionText;
	int current = textGroup.opaque->GetCurSel();
	if (current < 0)
		return;
	int len = textGroup.opaque->GetLBTextLen(current);
	textGroup.opaque->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	int i = 0;
	float f = 0.0;

	TrecPointer<TText> text = getCurrentText();
	if (!selectionText.ConvertToInt(&i))
	{
		f = static_cast<float>(i);
	}
	else if (selectionText.ConvertToFloat(&f))
		return;

	text->setOpaquency(f);
}

void TControl_Edit::OnEnChangeEdit12()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void TControl_Edit::OnCbnSelchangeContentOpaque()
{
	// TODO: Add your control notification handler code here
	TString selectionText;
	int current = contentGroup.opaque->GetCurSel();
	if (current < 0)
		return;
	int len = contentGroup.opaque->GetLBTextLen(current);
	contentGroup.opaque->GetLBText(current, selectionText.GetBuffer(len));
	selectionText.ReleaseBuffer();

	int i = 0;
	float f = 0.0;

	TrecPointer<TContent> content = getCurrentContent();
	if (!selectionText.ConvertToInt(&i))
	{
		f = static_cast<float>(i);
	}
	else if (selectionText.ConvertToFloat(&f))
		return;

	content->setOpaquency(f);
}


void TControl_Edit::OnBnClickedCheck7()
{
	if (IsDlgButtonChecked(IDC_CHECK7))
	{
		controlPointer->fixHeight = true;
	}
	else
	{
		controlPointer->fixHeight = false;
	}
}


void TControl_Edit::OnBnClickedCheck8()
{
	if (IsDlgButtonChecked(IDC_CHECK8))
	{
		controlPointer->fixWidth = true;
	}
	else
	{
		controlPointer->fixWidth = false;
	}
}




void TControl_Edit::OnBnClickedRadio5()
{
	if (IsDlgButtonChecked(IDC_RADIO5))
	{
		controlPointer->marginPriority = false;
	}
	else
	{
		controlPointer->marginPriority = true;
	}
}


void TControl_Edit::OnBnClickedRadio6()
{
	if (!IsDlgButtonChecked(IDC_RADIO6))
	{
		controlPointer->marginPriority = false;
	}
	else
	{
		controlPointer->marginPriority = true;
	}
}


void TControl_Edit::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	TString string;

	setCurrentText();
	// Check fot the caption


	// Height
	sizeGroup.height->GetWindowTextW(string);
	int attSize = 0;
	if (!string.ConvertToInt(&attSize))
		controlPointer->setHeight(attSize);
	string.Empty();
	
	// Width
	sizeGroup.width->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setWidth(attSize);
	string.Empty();

	// MinHeight
	sizeGroup.minHeight->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMinHeight(attSize);
	string.Empty();

	// MinWidth
	sizeGroup.minWidth->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMinWidth(attSize);
	string.Empty();

	// MaxHeight
	sizeGroup.maxHeight->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMaxHeight(attSize);
	string.Empty();

	// MaxWidth
	sizeGroup.maxWidth->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMaxWidth(attSize);
	string.Empty();

	// Margin
	sizeGroup.marginBottom->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMBottom(attSize);
	string.Empty();

	sizeGroup.marginTop->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMTop(attSize);
	string.Empty();

	sizeGroup.marginLeft->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMLeft(attSize);
	string.Empty();
	sizeGroup.marginRight->GetWindowTextW(string);
	if (!string.ConvertToInt(&attSize))
		controlPointer->setMRight(attSize);
	string.Empty();


	CDialogEx::OnOK();
}

/* For moving to another Control Editor*/
void TControl_Edit::OnBnClickedButton1()
{
	if (!controlPointer.get())
		return;

	OnBnClickedOk();

	if (typeid(controlPointer.get()) == typeid(TLayout*))
	{/*
		TLayoutEdit* tle = new TLayoutEdit(controlPointer);

		tle->DoModal();
		delete tle;
		tle = nullptr;*/
	}
	// TODO: Add your control notification handler code here
}
