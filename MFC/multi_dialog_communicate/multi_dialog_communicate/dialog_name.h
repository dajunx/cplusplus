#pragma once

#include "message.h"
// dialog_name dialog

class dialog_name : public CDialogEx
{
	DECLARE_DYNAMIC(dialog_name)

public:
	dialog_name(CWnd* pParent = NULL);   // standard constructor
	virtual ~dialog_name();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNotifyFatherMsg();

	HWND m_father_hwnd;
protected:
	afx_msg LRESULT OnFather2ChildMsg(WPARAM wParam, LPARAM lParam);
};
