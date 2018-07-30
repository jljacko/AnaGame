// FileNewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateBuilder.h"
#include "FileNewDialog.h"
#include "afxdialogex.h"


// FileNewDialog dialog

IMPLEMENT_DYNAMIC(FileNewDialog, CDialogEx)

FileNewDialog::FileNewDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILENEWDIALOG, pParent)
{

}

FileNewDialog::~FileNewDialog()
{
}

void FileNewDialog::givePointer(int * point)
{
	pointer = point;
}

void FileNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FileNewDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &FileNewDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RADIO1, &FileNewDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &FileNewDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &FileNewDialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &FileNewDialog::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &FileNewDialog::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &FileNewDialog::OnBnClickedRadio6)
END_MESSAGE_MAP()


// FileNewDialog message handlers


void FileNewDialog::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}


void FileNewDialog::OnBnClickedRadio1()
{
	if (pointer)
		*pointer = 1;
	
}


void FileNewDialog::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	if (pointer)
		*pointer = 2;
}


void FileNewDialog::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	if (pointer)
		*pointer = 3;
}


void FileNewDialog::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	if (pointer)
		*pointer = 4;
}


void FileNewDialog::OnBnClickedRadio5()
{
	// TODO: Add your control notification handler code here
	if (pointer)
		*pointer = 5;
}


void FileNewDialog::OnBnClickedRadio6()
{
	// TODO: Add your control notification handler code here
	if (pointer)
		*pointer = 6;
}
