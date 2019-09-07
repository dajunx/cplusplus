// dialog_name.cpp : implementation file
//

#include "stdafx.h"
#include "multi_dialog_communicate.h"
#include "dialog_name.h"
#include "afxdialogex.h"


// dialog_name dialog

IMPLEMENT_DYNAMIC(dialog_name, CDialogEx)

dialog_name::dialog_name(CWnd* pParent /*=NULL*/)
	: CDialogEx(dialog_name::IDD, pParent)
{

}

dialog_name::~dialog_name()
{
}

void dialog_name::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dialog_name, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_NOTIFY_FATHER_MSG, &dialog_name::OnBnClickedButtonNotifyFatherMsg)
	ON_MESSAGE(WM_FATHER_2_CHILD_MSG, &dialog_name::OnFather2ChildMsg)
END_MESSAGE_MAP()


// dialog_name message handlers


BOOL dialog_name::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void dialog_name::OnBnClickedButtonNotifyFatherMsg()
{
	// WM_CHILD_USER_DEFINE_MSG 消息要在父对话框 class wiz 中添加到自定义消息中
	if (m_father_hwnd != NULL)
	{
		::SendNotifyMessage(m_father_hwnd, WM_CHILD_USER_DEFINE_MSG, 0, 0);
	}
	
}


afx_msg LRESULT dialog_name::OnFather2ChildMsg(WPARAM wParam, LPARAM lParam)
{
	// 接收来自父窗体的自定义消息
	return 0;
}
