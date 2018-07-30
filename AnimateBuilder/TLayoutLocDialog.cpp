// TLayoutLocDialog.cpp : implementation file
//
#include <TString.h>


#include "stdafx.h"
#include "AnimateBuilder.h"
#include "TLayoutLocDialog.h"
#include "afxdialogex.h"


// TLayoutLocDialog dialog

IMPLEMENT_DYNAMIC(TLayoutLocDialog, CDialogEx)

TLayoutLocDialog::TLayoutLocDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{
	row = colunm = nullptr;
	rowMax = colunmMax = 0;
}

TLayoutLocDialog::~TLayoutLocDialog()
{
}

void TLayoutLocDialog::setPointers(int * r, int * c)
{
	row = r;
	colunm = c;
}

void TLayoutLocDialog::setMaximum(int r, int c)
{
	rowMax = r;
	colunmMax = c;
}

INT_PTR TLayoutLocDialog::DoModal()
{

	
	CWnd* string = GetDlgItem(IDC_EDIT3);
	TString numString;
	numString.Format(L"%d",rowMax);
	string->SetWindowTextW(numString.GetBuffer());
	numString.ReleaseBuffer();


	string = GetDlgItem(IDC_EDIT4);
	numString.Empty();
	numString.Format(L"%d", colunmMax);

	string->SetWindowTextW(numString.GetBuffer());
	numString.ReleaseBuffer();


	return CDialogEx::DoModal();
}

void TLayoutLocDialog::OnOK()
{
	if (row && colunm)
	{
		TString value;
		CWnd* box = GetDlgItem(IDC_EDIT1);

		box->GetWindowTextW(value);

		value.ConvertToInt(row);

		box = GetDlgItem(IDC_EDIT2);
		box->GetWindowTextW(value);

		value.ConvertToInt(colunm);

		if (*row > rowMax)
			*row = rowMax;

		if (*colunm > colunmMax)
			*colunm = colunmMax;
	}
	CDialogEx::OnOK();
}

void TLayoutLocDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TLayoutLocDialog, CDialogEx)
END_MESSAGE_MAP()


// TLayoutLocDialog message handlers
