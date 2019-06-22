#pragma once


// CTreeControl dialog

class CTreeControl : public CDialog
{
	DECLARE_DYNAMIC(CTreeControl)

public:
	CTreeControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreeControl();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CTreeCtrl m_treeCtrl;
  virtual BOOL OnInitDialog();
};
