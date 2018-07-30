// ErrorMesDial.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateCentral.h"
#include "ErrorMesDial.h"
#include "afxdialogex.h"
#include "resource.h"


// ErrorMesDial dialog

IMPLEMENT_DYNAMIC(ErrorMesDial, CDialogEx)

ErrorMesDial::ErrorMesDial(int lineError,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ERRORMESDIAL, pParent)
{
	number = lineError;
}

ErrorMesDial::~ErrorMesDial()
{
}

INT_PTR ErrorMesDial::DoModal()
{

	return CDialogEx::DoModal();
}

void ErrorMesDial::DoDataExchange(CDataExchange* pDX)
{
	this->GetDlgItem(IDC_EDIT1)->Set
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ErrorMesDial, CDialogEx)
END_MESSAGE_MAP()


// ErrorMesDial message handlers
