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
END_MESSAGE_MAP()


// dialog_name message handlers
