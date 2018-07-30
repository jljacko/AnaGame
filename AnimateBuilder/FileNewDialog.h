#pragma once
#include "stdafx.h"

// FileNewDialog dialog

class FileNewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FileNewDialog)

public:
	FileNewDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~FileNewDialog();

	void givePointer(int *point);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILENEWDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int* pointer;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
};
