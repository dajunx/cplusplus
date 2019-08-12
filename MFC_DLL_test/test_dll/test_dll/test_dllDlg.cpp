
// test_dllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test_dll.h"
#include "test_dllDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define  DllImport extern "C" _declspec(dllimport)
//DllImport int Add(int d1,int d2);
//DllImport int Sub(int d1,int d2);
//DllImport int Mul(int d1,int d2);
//DllImport int Div(int d1,int d2);
//DllImport int Mod(int d1,int d2);

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


// Ctest_dllDlg dialog




Ctest_dllDlg::Ctest_dllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest_dllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_dllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_dllDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON_USE_DLL, &Ctest_dllDlg::OnBnClickedButtonUseDll)
  ON_BN_CLICKED(IDC_BUTTON_LOAD, &Ctest_dllDlg::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// Ctest_dllDlg message handlers

BOOL Ctest_dllDlg::OnInitDialog()
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

void Ctest_dllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ctest_dllDlg::OnPaint()
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
HCURSOR Ctest_dllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctest_dllDlg::OnBnClickedButtonUseDll()
{
  /*
  int result;

  int nID = 1;
  int data1 = 2, data2 = 1;
  double dr = 0;

  switch(nID)
  {
  case 1: 
    {
      result = Add(data1, data2);
    }    
    break;
  case 2: 
    {
      result = Sub(data1, data2);
    }
    break;
  case 3: 
    {
      result = Mul(data1, data2);
    }
    break;
  case 4: 
    {
      dr = Div(data1, data2);
    }
    break;
  case 5: 
    {
      result = Mod(data1, data2);
    }
    break; 
  }
  */

}

DWORD WINAPI Ctest_dllDlg::ThreadProc(LPVOID lpParameter)
{
  typedef VOID (CALLBACK *Add)(int, int);
  Add add;
  HINSTANCE g_hInstanceDll = NULL;

  g_hInstanceDll = LoadLibrary(_T("static_dll.dll"));

  if ( g_hInstanceDll == NULL)
  {
    AfxMessageBox(_T("加载DLL失败"));

    return -1L;
  }

  add = (Add) ::GetProcAddress(g_hInstanceDll, "Add");
  if (add == NULL)
  {
    int i = 0;
    ///TODO 调用 add 函数有问题
  }
  else
  {
    int i = 0;
  }

  Sleep(1000000);

  return 0L;
}

void Ctest_dllDlg::OnBnClickedButtonLoad()
{
  int ThreadInputData = 0;
  HANDLE thread1 = CreateThread(NULL, 0, Ctest_dllDlg::ThreadProc, &ThreadInputData, 0, NULL);
}
