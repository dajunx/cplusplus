
// sme_moniterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sme_moniter.h"
#include "sme_moniterDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <iostream>
//#include <process.h> // _beginthreadex head file
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_D_BTN 10000

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


// Csme_moniterDlg dialog




Csme_moniterDlg::Csme_moniterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Csme_moniterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csme_moniterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Csme_moniterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDOK, &Csme_moniterDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_OPEN_TREE_DIALOG, &Csme_moniterDlg::OnBnClickedOpenTreeDialog)
END_MESSAGE_MAP()


// Csme_moniterDlg message handlers

BOOL Csme_moniterDlg::OnInitDialog()
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

  m_static[0].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 100, 150, 120), this, IDC_D_BTN + 1);
  m_static[1].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 120, 150, 140), this, IDC_D_BTN + 2);
  m_static[2].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 140, 150, 160), this, IDC_D_BTN + 3);
  m_static[3].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 160, 150, 180), this, IDC_D_BTN + 4);
  m_static[4].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 180, 150, 200), this, IDC_D_BTN + 5);
  m_static[5].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 200, 150, 220), this, IDC_D_BTN + 6);
  m_static[6].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 220, 150, 240), this, IDC_D_BTN + 7);
  m_static[7].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 240, 150, 260), this, IDC_D_BTN + 8);
  m_static[8].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 260, 150, 280), this, IDC_D_BTN + 9);
  m_static[9].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 280, 150, 300), this, IDC_D_BTN + 10);

  HANDLE thread1 = CreateThread(NULL, 0, Csme_moniterDlg::ThreadFun, NULL, 0, NULL);
  //WaitForSingleObject(thread3, INFINITE);

  //static[10].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 200, 150, 220), this, IDC_D_BTN + 11);
  //static[11].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 210, 150, 230), this, IDC_D_BTN + 12);
  //static[12].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 220, 150, 240), this, IDC_D_BTN + 13);
  //static[13].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 230, 150, 250), this, IDC_D_BTN + 14);
  //static[14].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 240, 150, 260), this, IDC_D_BTN + 15);
  //static[15].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 250, 150, 270), this, IDC_D_BTN + 16);
  //static[16].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 260, 150, 280), this, IDC_D_BTN + 17);
  //static[17].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 270, 150, 290), this, IDC_D_BTN + 18);
  //static[18].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 280, 150, 300), this, IDC_D_BTN + 19);
  //static[19].Create(_T("test"), WS_CHILD|WS_VISIBLE, CRect(100, 290, 150, 310), this, IDC_D_BTN + 20);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

unsigned int __stdcall Csme_moniterDlg::ThreadFun(PVOID pm)
{
  std::stringstream ss_content;
  
  int i = 0;
  while(true)
  {
    ss_content << "linjianjun" << i;
    CString cstr_temp = CT2A(ss_content.str().c_str());
    m_static[0].SetWindowText(cstr_temp);
    m_static[1].SetWindowText(cstr_temp);
    m_static[2].SetWindowText(cstr_temp);
    m_static[3].SetWindowText(cstr_temp);
    m_static[4].SetWindowText(cstr_temp);
    m_static[5].SetWindowText(cstr_temp);
    m_static[6].SetWindowText(cstr_temp);
    m_static[7].SetWindowText(cstr_temp);
    m_static[8].SetWindowText(cstr_temp);
    m_static[9].SetWindowText(cstr_temp);
  }
  
  return 0;
}

void Csme_moniterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Csme_moniterDlg::OnPaint()
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
HCURSOR Csme_moniterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// �������ݿ�
void Csme_moniterDlg::test_mysql_functions()
{
  //��ʼ��
  try {
    conn = mysql_init(NULL);
  } catch (int e) {
    int i = 0;
  }
  // conn = mysql_init(0);

  if (conn == NULL) {
    return;
  }

  std::string str_ip("119.29.36.228");
  std::string str_username("kaka"), str_pwd("kaka");
  std::string str_db_name("test");
  int login_port = 3306;

  //�������ݿ�
  if (!mysql_real_connect(conn, str_ip.c_str(), str_username.c_str(), str_pwd.c_str(),
    str_db_name.c_str(), login_port, NULL, 0)) {
      std::stringstream ss_err;
      ss_err << "Failed to connect to database: Error: " << mysql_error(conn);
      std::string str_err = ss_err.str();

      return;
  }


  //ִ��sql
  std::string str_sql("select * from test;");
  if (mysql_real_query(conn, str_sql.c_str(), str_sql.size())) {
    return;
  }

  res = mysql_store_result(conn);

  std::stringstream data;
  int column = mysql_num_fields(res);
  while (row = mysql_fetch_row(res)) {
    for (int i = 0; i < column; ++i) {
      data << row[i] << " ";
    }
    data << std::endl;
  }

  std::string str_query_result = data.str();
}

void Csme_moniterDlg::test_socket()
{
  WORD sockVersion = MAKEWORD(2, 2);
  WSADATA data;
  if (WSAStartup(sockVersion, &data) != 0)
  {
    return;
  }

  SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sclient == INVALID_SOCKET)
  {
    printf("invalid socket !");
    return;
  }

  sockaddr_in serAddr;
  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(6888);
  serAddr.sin_addr.S_un.S_addr = inet_addr("119.29.36.228");

  while (true)
  {
    int ret_conn = connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr));
    if (ret_conn == SOCKET_ERROR) {
      std::cout << "connect server err:" << WSAGetLastError() << std::endl;
      break;
    }

    // �Ż����ӷ���ˣ���û�����Ϸ������sleep 1�룬ֱ�������Ϸ���ˣ�
    bool continueConn = false;
    showSubNetErr(continueConn);
    if (continueConn) {
      Sleep(1000);
      continue;
    }

    //���տͻ��˷��͵����ݲ���ӡ
    while (true) {

      std::string str_send_data("hello server,i'm client.");
      int ret  = send(sclient, str_send_data.c_str(), str_send_data.size(), 0);
      if (ret > 0)
      {
        int i = 0; // succ.
      } else {
        std::stringstream ss_err;
        ss_err << "recv server data err:" << WSAGetLastError();
        std::string str_err = ss_err.str();
        break;
      }
    }
  }

  closesocket(sclient);
  WSACleanup();
}

void Csme_moniterDlg::showSubNetErr(bool& continueConn)
{
  switch (WSAGetLastError()) {
  case WSAECONNREFUSED:
    {
      std::cout << "Connection refused." << std::endl;
      continueConn = true;
      break;
    }
  default:
    ;
    //std::cout<<"unknow err."<<std::endl;
  }
}

void Csme_moniterDlg::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  CDialogEx::OnOK();

  //test_mysql_functions();
  test_socket();
}


void Csme_moniterDlg::OnBnClickedOpenTreeDialog()
{
  // TODO: Add your control notification handler code here
  CTreeControl tree_control;
  tree_control.DoModal();
}