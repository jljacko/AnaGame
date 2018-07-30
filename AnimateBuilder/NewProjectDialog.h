#pragma once


// NewProjectDialog dialog

class NewProjectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NewProjectDialog)

public:
	NewProjectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewProjectDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
