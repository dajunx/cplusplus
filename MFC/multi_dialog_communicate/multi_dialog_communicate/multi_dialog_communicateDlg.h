
// multi_dialog_communicateDlg.h : header file
//

#pragma once


// Cmulti_dialog_communicateDlg dialog
class Cmulti_dialog_communicateDlg : public CDialogEx
{
// Construction
public:
	Cmulti_dialog_communicateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MULTI_DIALOG_COMMUNICATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
