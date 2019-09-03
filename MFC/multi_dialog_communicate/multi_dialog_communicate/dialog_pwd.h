#pragma once


// dialog_pwd dialog

class dialog_pwd : public CDialogEx
{
	DECLARE_DYNAMIC(dialog_pwd)

public:
	dialog_pwd(CWnd* pParent = NULL);   // standard constructor
	virtual ~dialog_pwd();

// Dialog Data
	enum { IDD = IDD_DIALOG_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
