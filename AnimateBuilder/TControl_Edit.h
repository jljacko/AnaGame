#pragma once
#include "stdafx.h"
#include <AnafaceUI.h>
// TControl_Edit dialog

namespace
{
	typedef struct borderBox
	{
		CComboBox* thickBox;
		CComboBox* style;
		CComboBox* opaque;
		CMFCColorButton* color;
	}borderBox;

	void nullifyBorderBox(borderBox*);

	typedef struct contentBox
	{
		//CComboBox* thickBox;
		CComboBox* style;
		CComboBox* opaque;
		CButton* button;
		CEdit* edit;
		CMFCColorButton* color;
	}contentBox;

	void nullifyContentBox(contentBox*);

	typedef struct textBox
	{
		CComboBox* font;
		CComboBox* fontSize;
		CComboBox* VerticalSpace;
		CComboBox* horizSpace;
		CComboBox* opaque;
		CMFCColorButton* color;
		CEdit* edit;
	}textBox;

	void nullifyTextBox(textBox*);

	typedef struct mouseBox
	{
		CButton* normal;
		CButton* hover;
		CButton* click;
	}mouseBox;

	void nullifyMouseBox(mouseBox*);

	typedef struct scrollBox
	{
		CButton* vScrolling;
		CButton* hScrolling;
		CButton* multiScroll;
	}scrollBox;

	void nullifyScrollBox(scrollBox*);

	typedef struct sizeBox
	{
		CButton* heightWidth;
		CButton* margin;
		CButton* four_side;
		CButton* pointPadding;
		CButton* fixHeight;
		CButton* fixWidth;
		CEdit* height;
		CEdit* width;
		CEdit* minHeight;
		CEdit* minWidth;
		CEdit* maxHeight;
		CEdit* maxWidth;
		CEdit* marginTop;
		CEdit* marginBottom;
		CEdit* marginLeft;
		CEdit* marginRight;
	}sizeBox;

	void nullifySizeBox(sizeBox*);

	typedef struct shapeBox
	{
		CButton* rectangle;
		CButton* roundedRectangle;
		CButton* eclipse;
		CButton* other;
		CButton* otherButton;
	}shapeBox;

	void nullifyShapeBox(shapeBox*);
}



class TControl_Edit : public CDialogEx
{
	DECLARE_DYNAMIC(TControl_Edit)

public:
	TControl_Edit(TrecPointer<TControl>,CWnd* pParent = NULL);   // standard constructor
	virtual ~TControl_Edit();

	INT_PTR DoModal() override;
	TrecPointer<TControl> getControl();



// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void initialize();
	TrecPointer<TControl> controlPointer;
	//TControl* control;
	BOOL OnInitDialog() override;
	// Various controls
	textBox textGroup;
	contentBox contentGroup;
	borderBox borderGroup;
	sizeBox sizeGroup;
	mouseBox mouseGroup;
	scrollBox scrollGroup;
	shapeBox shapeGroup;
	CButton* borderCheck;
	CButton* contentCheck;
	CButton* textCheck;
	CButton* subClass;
	bool isCreated;

	messageState stateMode;


	// Pointers to hold onto
	TrecPointer<TText> text1,text2,text3;
	TrecPointer<TContent> content1, content2, content3;
	TrecPointer<TBorder> border1, border2, border3;

	void enableDesableCheck();
	void setTextInControl(bool);
	void setBorderInControl(bool set);
	void setContentInControl(bool);

	TrecPointer<TBorder> getCurrentBorder();
	TrecPointer<TContent> getCurrentContent();
	TrecPointer<TText> getCurrentText();

	void setCurrentText();

	void PrepControls();

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedMfccolorbutton1();
	afx_msg void OnBnClickedMfccolorbutton2();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeOpaqBorder();
	afx_msg void OnBnClickedMfccolorbutton3();
	afx_msg void OnCbnSelchangeCombo7();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnCbnSelchangeCombo10();
	afx_msg void OnCbnSelchangeCombo11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnCbnSelchangeContentOpaque();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
