#pragma once
#include "afxwin.h"


// CConfigDlg dialog

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	CConfigDlg(CWnd* pParent, DCB dcb); // custom constructor

	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_DLGCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBaudRate;
	CString m_strParity;
	CString m_strDataBits;
	CString m_strStopBit;
	CString m_strSendBuffer;
	BOOL m_CommBreakDetected;
	BOOL m_CommCTSDetected;
	BOOL m_CommDSRDetected;
	BOOL m_CommERRDetected;
	BOOL m_CommRingDetected;
	BOOL m_CommRLSDDetected;
	BOOL m_CommRxChar;
	BOOL m_CommRxCharFlag;
	BOOL m_CommTxEmpty;

protected:
	DCB     m_dcb;

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedOk();
};
