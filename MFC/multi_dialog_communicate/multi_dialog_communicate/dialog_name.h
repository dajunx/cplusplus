#pragma once


// dialog_name dialog

class dialog_name : public CDialogEx
{
	DECLARE_DYNAMIC(dialog_name)

public:
	dialog_name(CWnd* pParent = NULL);   // standard constructor
	virtual ~dialog_name();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
