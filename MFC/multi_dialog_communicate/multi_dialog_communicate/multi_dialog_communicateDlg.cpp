
// multi_dialog_communicateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "multi_dialog_communicate.h"
#include "multi_dialog_communicateDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmulti_dialog_communicateDlg dialog




Cmulti_dialog_communicateDlg::Cmulti_dialog_communicateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmulti_dialog_communicateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmulti_dialog_communicateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tab_main);

	m_dialog_name.Create(IDD_DIALOG_NAME, &m_tab_main);
	m_dialog_pwd.Create(IDD_DIALOG_PWD, &m_tab_main);

	// tab �ؼ��ⲿ ���Ի����ȡ �ӶԻ���ؼ�����
	DDX_Control(pDX, IDD_DIALOG_NAME, m_dialog_name);
	DDX_Control(pDX, IDD_DIALOG_PWD, m_dialog_pwd);
	//  DDX_Control(pDX, IDC_BUTTON_TEST, m_button);
}

BEGIN_MESSAGE_MAP(Cmulti_dialog_communicateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &Cmulti_dialog_communicateDlg::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDC_BUTTON_CTRL_MESSAGE, &Cmulti_dialog_communicateDlg::OnBnClickedButtonCtrlMessage)

	ON_MESSAGE(WM_USER_DEFINE_MSG, &Cmulti_dialog_communicateDlg::OnUserDefineMsg)
	ON_BN_CLICKED(IDC_BUTTON_USER_DEFINE_MSG, &Cmulti_dialog_communicateDlg::OnBnClickedButtonUserDefineMsg)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &Cmulti_dialog_communicateDlg::OnBnClickedButtonTest)
	ON_MESSAGE(WM_CHILD_USER_DEFINE_MSG, &Cmulti_dialog_communicateDlg::OnChildUserDefineMsg)
	ON_BN_CLICKED(IDC_BUTTON_MSG_2_CHILD, &Cmulti_dialog_communicateDlg::OnBnClickedButtonMsg2Child)
END_MESSAGE_MAP()


// Cmulti_dialog_communicateDlg message handlers

BOOL Cmulti_dialog_communicateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// �򴰿� tab ctrl �ؼ������ǩ
	m_tab_main.InsertItem(0, _T("�û���"));
	m_tab_main.InsertItem(1, _T("����"));

	// �趨��Tab ����ʾ�ķ�Χ
	m_tab_main.GetClientRect(m_ctrl_rc);
	m_ctrl_rc.top += 20;
	m_ctrl_rc.bottom -= 0;
	m_ctrl_rc.left += 0;
	m_ctrl_rc.right -= 0;

	m_dialog_name.MoveWindow(&m_ctrl_rc);
	m_dialog_pwd.MoveWindow(&m_ctrl_rc);
	
	m_dialog_name.ShowWindow(SW_SHOW);
	m_dialog_pwd.ShowWindow(SW_HIDE);

	m_dialog_name.m_father_hwnd = GetSafeHwnd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cmulti_dialog_communicateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cmulti_dialog_communicateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cmulti_dialog_communicateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmulti_dialog_communicateDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// ͬ�� tab ctrl �ؼ�ҳǩ�仯����ʾ�������ض�Ӧ �ӶԻ���
	int curr_sel = m_tab_main.GetCurSel();
	switch(curr_sel)
	{
	case 0:
		{
			m_dialog_name.ShowWindow(SW_SHOW);
			m_dialog_pwd.ShowWindow(SW_HIDE);
		}
		break;
	case 1:
		{
			m_dialog_name.ShowWindow(SW_HIDE);
			m_dialog_pwd.ShowWindow(SW_SHOW);
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}


void Cmulti_dialog_communicateDlg::OnBnClickedButtonCtrlMessage()
{
	// �� tab ctrl ���ͱ�ǩѡ����Ϣ�������л����Ǹ���ǩ������ͨ�� SetCurSel ����
	NMHDR nmhdr;
	nmhdr.code = TCN_SELCHANGE;
	nmhdr.hwndFrom = m_tab_main.GetSafeHwnd();
	nmhdr.idFrom = m_tab_main.GetDlgCtrlID();
	::SendDlgItemMessage(this->m_hWnd, IDC_TAB_MAIN, WM_NOTIFY, MAKELONG(TCN_SELCHANGE, 0), (LPARAM)(&nmhdr));
}

afx_msg LRESULT Cmulti_dialog_communicateDlg::OnUserDefineMsg(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = LOWORD(wParam);

	return 0;
}


void Cmulti_dialog_communicateDlg::OnBnClickedButtonUserDefineMsg()
{
	// �����Զ�����Ϣ
	//::SendNotifyMessage(this->m_hWnd, WM_USER_DEFINE_MSG, 1, NULL); // ����������߼� һ����Ч��
	::SendNotifyMessage(this->GetSafeHwnd(), WM_USER_DEFINE_MSG, 1, NULL);
}


void Cmulti_dialog_communicateDlg::OnBnClickedButtonTest()
{
	// ��ؼ� ϵͳ�������Ϣ��Ӧ���� ������Ϣ
	//  OnBnClickedButtonUserDefineMsg ��Ӧ
	// �ο����ӣ�https://blog.csdn.net/igolang/article/details/41775123
	// �򴰿��пؼ�ID������Ϣ ==>  �ο� http://msdn.microsoft.com/en-us/library/windows/desktop/ms645515(v=vs.85).aspx
	::SendDlgItemMessage(this->m_hWnd, IDC_BUTTON_USER_DEFINE_MSG, BM_CLICK, 0, 0);
}


afx_msg LRESULT Cmulti_dialog_communicateDlg::OnChildUserDefineMsg(WPARAM wParam, LPARAM lParam)
{
	// ���������ӶԻ��� IDD_DIALOG_NAME ���͹�������Ϣ
	return 0;
}


void Cmulti_dialog_communicateDlg::OnBnClickedButtonMsg2Child()
{
	// ���Ӵ��巢���Զ�����Ϣ
	HWND Hwnd = m_dialog_name.GetSafeHwnd();
	if (Hwnd != NULL)
	{
		::SendNotifyMessage(Hwnd, WM_FATHER_2_CHILD_MSG, 0, 0);
	}	
}
