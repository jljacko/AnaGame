// TLayoutEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "TLayoutEdit.h"
#include "TControl_Edit.h"
#include "afxdialogex.h"


// TLayoutEdit dialog

IMPLEMENT_DYNAMIC(TLayoutEdit, CDialogEx)

TLayoutEdit::TLayoutEdit(TrecPointer<TLayout> contPoint,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	if (!contPoint.get())
	{
		return;
	}

	controlPointer = contPoint;
	//layout = controlPointer->pointer.tl;
	rowCombo = nullptr;
	rowNumber = nullptr;
	rowSize = nullptr;
	colunmCombo = nullptr;
	colunmNumber = nullptr;
	colunmSize = nullptr;
	errorBox = nullptr;
}

TLayoutEdit::~TLayoutEdit()
{

}

INT_PTR TLayoutEdit::DoModal()
{
	INT_PTR returnable = CDialogEx::DoModal();

	return returnable;
}

void TLayoutEdit::initialize()
{
	rowCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	rowNumber = (CEdit*)GetDlgItem(IDC_EDIT1);
	rowSize = (CEdit*)GetDlgItem(IDC_EDIT5);
	colunmCombo = (CComboBox*)GetDlgItem(IDC_COMBO2);
	colunmNumber = (CEdit*)GetDlgItem(IDC_EDIT2);
	colunmSize = (CEdit*)GetDlgItem(IDC_EDIT6);
	errorBox = (CEdit*)GetDlgItem(IDC_EDIT3);
	if (errorBox)
	{
		errorBox->SetWindowTextW(L"TLayout Generation");
	}
}

void TLayoutEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL TLayoutEdit::OnInitDialog()
{
	initialize();
	return CDialogEx::OnInitDialog();
}


BEGIN_MESSAGE_MAP(TLayoutEdit, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO3, &TLayoutEdit::OnBnClickedRadio3)
	ON_EN_CHANGE(IDC_EDIT1, &TLayoutEdit::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &TLayoutEdit::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_RADIO1, &TLayoutEdit::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &TLayoutEdit::OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &TLayoutEdit::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &TLayoutEdit::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &TLayoutEdit::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &TLayoutEdit::OnBnClickedOk)
END_MESSAGE_MAP()


// TLayoutEdit message handlers

// Set Layout to vertical stack with rows
void TLayoutEdit::OnBnClickedRadio3()
{

	// Disable the colunms
	if (colunmCombo)
		colunmCombo->EnableWindow(false);
	if (colunmNumber)
		colunmNumber->EnableWindow(false);
	if (colunmSize)
		colunmSize->EnableWindow(false);

	// Enable the rows
	if (rowCombo)
		rowCombo->EnableWindow(true);
	if (rowNumber)
		rowNumber->EnableWindow(true);
	if (rowSize)
		rowSize->EnableWindow(true);
}

// Rows (number of rows)
void TLayoutEdit::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	// Chnge Combo1
	if (!rowCombo || !rowNumber)
		return;

	// first, get number from box
	TString string;
	rowNumber->GetWindowTextW(string);
	int val = 0;
	if (!string.ConvertToInt(&val))
	{
		if (val < 1 || val > 100)
		{
			if (errorBox)
				errorBox->SetWindowTextW(L"ERROR: value must be inclusively between 1-100!");
			return;
		}
		rowCombo->ResetContent();

		for (int c = 1; c <= val; c++)
		{
			string.Format(_T("%d"), c);
			rowCombo->AddString(string);
		}
		if (errorBox)
			errorBox->SetWindowTextW(L"Row Number updated!");
	}
	else
	{
		if (errorBox)
			errorBox->SetWindowTextW(L"ERROR: Text must equate to an integer inclusively between 1-100!");
	}
}

// Columns (number of colunms)
void TLayoutEdit::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!colunmCombo || !colunmNumber)
		return;

	// first, get number from box
	TString string;
	colunmNumber->GetWindowTextW(string);
	int val = 0;
	if (!string.ConvertToInt(&val))
	{
		if (val < 1 || val > 100)
		{
			if (errorBox)
				errorBox->SetWindowTextW(L"ERROR: value must be inclusively between 1-100!");
			return;
		}

		colunmCombo->ResetContent();

		for (int c = 1; c <= val; c++)
		{
			string.Format(_T("%d"), c);
			colunmCombo->AddString(string);
		}
		if (errorBox)
			errorBox->SetWindowTextW(L"Colunm Number updated!");
	}
	else
	{
		if (errorBox)
			errorBox->SetWindowTextW(L"ERROR: Text must equate to an integer inclusively between 1-100!");
	}
}

// Grid
void TLayoutEdit::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here



	// Enable the colunms
	if (colunmCombo)
		colunmCombo->EnableWindow(true);
	if (colunmNumber)
		colunmNumber->EnableWindow(true);
	if (colunmSize)
		colunmSize->EnableWindow(true);

	// Enable the rows
	if (rowCombo)
		rowCombo->EnableWindow(true);
	if (rowNumber)
		rowNumber->EnableWindow(true);
	if (rowSize)
		rowSize->EnableWindow(true);
}

// Set to a horizontal stack with colunms
void TLayoutEdit::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here


	// enable the colunms
	if (colunmCombo)
		colunmCombo->EnableWindow();
	if (colunmNumber)
		colunmNumber->EnableWindow();
	if (colunmSize)
		colunmSize->EnableWindow();

	// Disable the rows
	if (rowCombo)
		rowCombo->EnableWindow(0);
	if (rowNumber)
		rowNumber->EnableWindow(0);
	if (rowSize)
		rowSize->EnableWindow(0);
}

// Change the focus row
void TLayoutEdit::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	if (!rowCombo || !rowSize || !controlPointer.get())
		return;
	TString string;
	rowCombo->GetWindowTextW(string);
	int val = 0;
	if (!string.ConvertToInt(&val))
	{
		string.Format(_T("%d"), controlPointer->getRowHeight(val-1));
		rowSize->SetWindowTextW(string);
	}
	else
		if (errorBox)
			errorBox->SetWindowTextW(L"Internal Error: Value captured by Combo Box is not a number. Update failed");
}

// Change the focus colunm
void TLayoutEdit::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	if (!colunmCombo || !colunmSize || !controlPointer.get())
		return;
	TString string;
	colunmCombo->GetWindowTextW(string);
	int val = 0;
	if (!string.ConvertToInt(&val))
	{
		string.Format(_T("%d"), controlPointer->getColunmWidth(val - 1));
		colunmSize->SetWindowTextW(string);
	}
	else
		if (errorBox)
			errorBox->SetWindowTextW(L"Internal Error: Value captured by Combo Box is not a number. Update failed");
}


void TLayoutEdit::OnBnClickedButton1()
{

	OnBnClickedOk();

	TControl_Edit *tce = new TControl_Edit(controlPointer.get());
	tce->DoModal();
	delete tce;
	tce = nullptr;
	// TODO: Add your control notification handler code here
}


void TLayoutEdit::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
