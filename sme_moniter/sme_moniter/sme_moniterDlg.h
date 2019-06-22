
// sme_moniterDlg.h : header file
//

#pragma once

#include <mysql.h>

// Csme_moniterDlg dialog
class Csme_moniterDlg : public CDialogEx
{
// Construction
public:
	Csme_moniterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SME_MONITER_DIALOG };

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
  afx_msg void OnBnClickedOk();

private:
  // ≤‚ ‘mysql ¡¥Ω”
  void test_mysql_functions();
  void test_socket();

private:
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
};
