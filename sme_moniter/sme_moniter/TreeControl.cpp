// TreeControl.cpp : implementation file
//

#include "stdafx.h"
#include "sme_moniter.h"
#include "TreeControl.h"
#include "afxdialogex.h"

/**
 * ²Î¿¼£ºhttps://www.tutorialspoint.com/mfc/mfc_tree_control.htm
 */

// CTreeControl dialog

IMPLEMENT_DYNAMIC(CTreeControl, CDialog)

CTreeControl::CTreeControl(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeControl::IDD, pParent)
{

}

CTreeControl::~CTreeControl()
{
}

void CTreeControl::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE_NODE_LIST, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CTreeControl, CDialog)
END_MESSAGE_MAP()


// CTreeControl message handlers


BOOL CTreeControl::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  //SetIcon(m_hIcon, TRUE);            // Set big icon
  //SetIcon(m_hIcon, FALSE);            // Set small icon

  // TODO: Add extra initialization here
  HTREEITEM hItem, hCar;
  hItem = m_treeCtrl.InsertItem(L"Car Listing", TVI_ROOT);
  hCar = m_treeCtrl.InsertItem(L"Economy", hItem);
  m_treeCtrl.InsertItem(L"BH-733", hCar);
  m_treeCtrl.InsertItem(L"SD-397", hCar);
  m_treeCtrl.InsertItem(L"JU-538", hCar);
  m_treeCtrl.InsertItem(L"DI-285", hCar);
  m_treeCtrl.InsertItem(L"AK-830", hCar);
  hCar = m_treeCtrl.InsertItem(L"Compact", hItem);
  m_treeCtrl.InsertItem(L"HG-490", hCar);
  m_treeCtrl.InsertItem(L"PE-473", hCar);
  hCar = m_treeCtrl.InsertItem(L"Standard", hItem);
  m_treeCtrl.InsertItem(L"SO-398", hCar);
  m_treeCtrl.InsertItem(L"DF-438", hCar);
  m_treeCtrl.InsertItem(L"IS-833", hCar);
  hCar = m_treeCtrl.InsertItem(L"Full Size", hItem);
  m_treeCtrl.InsertItem(L"PD-304", hCar);
  hCar = m_treeCtrl.InsertItem(L"Mini Van", hItem);
  m_treeCtrl.InsertItem(L"ID-497", hCar);
  m_treeCtrl.InsertItem(L"RU-304", hCar);
  m_treeCtrl.InsertItem(L"DK-905", hCar);
  hCar = m_treeCtrl.InsertItem(L"SUV", hItem);
  m_treeCtrl.InsertItem(L"FE-948", hCar);
  m_treeCtrl.InsertItem(L"AD-940", hCar);
  hCar = m_treeCtrl.InsertItem(L"Truck", hItem);
  m_treeCtrl.InsertItem(L"HD-394", hCar);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
