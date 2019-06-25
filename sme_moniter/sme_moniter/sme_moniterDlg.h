
// sme_moniterDlg.h : header file
//

#pragma once

#include <mysql.h>

#include <windows.h>
#include <WINSOCK2.H>
#include <STDIO.H>
#include <Ws2tcpip.h>

#include "TreeControl.h"

#pragma  comment(lib,"ws2_32.lib")

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
  
  // ≤‚ ‘Õ¯¬Á ¡¥Ω”
  void test_socket();
  void showSubNetErr(bool& continueConn);

private:
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
public:
  afx_msg void OnBnClickedOpenTreeDialog();

  CStatic m_static[20];

  static unsigned int __stdcall ThreadFun(PVOID pm);
};
