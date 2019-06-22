
// sme_moniterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sme_moniter.h"
#include "sme_moniterDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <string>
#include <sstream>

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

	return TRUE;  // return TRUE  unless you set the focus to a control
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

// 链接数据库
void Csme_moniterDlg::test_mysql_functions()
{
  //初始化
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

  //连接数据库
  if (!mysql_real_connect(conn, str_ip.c_str(), str_username.c_str(), str_pwd.c_str(),
    str_db_name.c_str(), login_port, NULL, 0)) {
      std::stringstream ss_err;
      ss_err << "Failed to connect to database: Error: " << mysql_error(conn);
      std::string str_err = ss_err.str();

      return;
  }


  //执行sql
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

    // 优化连接服务端：若没连接上服务端则sleep 1秒，直到连接上服务端；
    bool continueConn = false;
    showSubNetErr(continueConn);
    if (continueConn) {
      Sleep(1000);
      continue;
    }

    //接收客户端发送的数据并打印
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
