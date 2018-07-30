// NewProjectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "NewProjectDialog.h"
#include "afxdialogex.h"


// NewProjectDialog dialog

IMPLEMENT_DYNAMIC(NewProjectDialog, CDialogEx)

NewProjectDialog::NewProjectDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

NewProjectDialog::~NewProjectDialog()
{
}

void NewProjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NewProjectDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &NewProjectDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// NewProjectDialog message handlers


void NewProjectDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}
