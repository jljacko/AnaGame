#pragma once
#include "stdafx.h"
#include <AnafaceUI.h>
#include <TString.h>

// TLayoutLocDialog dialog

class TLayoutLocDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TLayoutLocDialog)

public:
	TLayoutLocDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TLayoutLocDialog();

	void setPointers(int* r, int* c);
	void setMaximum(int r, int c);

	INT_PTR DoModal()override;
	void OnOK()override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int *row, *colunm, rowMax,colunmMax;

	DECLARE_MESSAGE_MAP()
};
