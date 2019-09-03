// dialog_pwd.cpp : implementation file
//

#include "stdafx.h"
#include "multi_dialog_communicate.h"
#include "dialog_pwd.h"
#include "afxdialogex.h"


// dialog_pwd dialog

IMPLEMENT_DYNAMIC(dialog_pwd, CDialogEx)

dialog_pwd::dialog_pwd(CWnd* pParent /*=NULL*/)
	: CDialogEx(dialog_pwd::IDD, pParent)
{

}

dialog_pwd::~dialog_pwd()
{
}

void dialog_pwd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dialog_pwd, CDialogEx)
END_MESSAGE_MAP()


// dialog_pwd message handlers
