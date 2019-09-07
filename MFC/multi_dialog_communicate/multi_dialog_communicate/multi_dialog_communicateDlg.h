
// multi_dialog_communicateDlg.h : header file
//

#pragma once
#include "dialog_name.h"
#include "dialog_pwd.h"


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

public:
	CTabCtrl m_tab_main;
	CRect m_ctrl_rc;

	dialog_name m_dialog_name;
	dialog_pwd m_dialog_pwd;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCtrlMessage();
//	afx_msg void OnBnClickedButtonSendButtonSefmsg();
//	afx_msg void OnBnClickedButtonTest();
//	CButton m_button;
protected:
	afx_msg LRESULT OnUserDefineMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonUserDefineMsg();
	afx_msg void OnBnClickedButtonTest();
protected:
	afx_msg LRESULT OnChildUserDefineMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonMsg2Child();
};
