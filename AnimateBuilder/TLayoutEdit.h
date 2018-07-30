#pragma once
#include "stdafx.h"
#include <AnafaceUI.h>

// TLayoutEdit dialog

class TLayoutEdit : public CDialogEx
{
	DECLARE_DYNAMIC(TLayoutEdit)

public:
	TLayoutEdit(TrecPointer<TLayout>, CWnd* pParent = NULL);   // standard constructor
	virtual ~TLayoutEdit();

	INT_PTR DoModal() override;
	void initialize();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;
	TrecPointer<TLayout> controlPointer;
	//TLayout* layout;

	// Member features, this dialog is less complex than the dialog for TControl
	// Safe to not gorup evry control here
	CComboBox* rowCombo;
	CComboBox* colunmCombo;
	CEdit* rowNumber;
	CEdit* colunmNumber;
	CEdit* rowSize;
	CEdit* colunmSize;

	CEdit* errorBox; // to alert users to errors without creating another dialog

	int xFocus, yFocus; // which colunm and row to focus on

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
