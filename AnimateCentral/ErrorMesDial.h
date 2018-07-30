#pragma once
#include "stdafx.h"


// ErrorMesDial dialog

class ErrorMesDial : public CDialogEx
{
	DECLARE_DYNAMIC(ErrorMesDial)

public:
	ErrorMesDial(int lineError, CWnd* pParent = NULL);   
	virtual ~ErrorMesDial();
	INT_PTR DoModal() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ERRORMESDIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int number;
	DECLARE_MESSAGE_MAP()
};
