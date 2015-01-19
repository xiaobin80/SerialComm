// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialComm.h"
#include "ConfigDlg.h"


// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	m_strBaudRate           = _T("");
	m_strParity             = _T("");
	m_strDataBits           = _T("");
	m_strStopBit            = _T("");
	m_CommBreakDetected     = FALSE;
    m_CommCTSDetected       = FALSE;
	m_CommDSRDetected       = FALSE;
	m_CommERRDetected       = FALSE;
	m_CommRingDetected      = FALSE;
	m_CommRLSDDetected      = FALSE;
	m_CommRxChar            = TRUE;          // changed value
	m_CommRxCharFlag        = FALSE;
	m_CommTxEmpty           = FALSE;
	m_strSendBuffer         = _T("");    
}

CConfigDlg::CConfigDlg(CWnd* pParent, DCB dcb)
:CDialog(CConfigDlg::IDD, pParent)
{
	m_dcb = dcb;
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_BAUDRATECOMBO, m_strBaudRate);
	DDX_CBString(pDX, IDC_PARITYCOMBO, m_strParity);
	DDX_CBString(pDX, IDC_DATABITSCOMBO, m_strDataBits);
	DDX_CBString(pDX, IDC_STOPBITCOMBO, m_strStopBit);
	DDX_CBString(pDX, IDC_SENDBUFFERCOMBO, m_strSendBuffer);
	DDX_Check(pDX, IDC_CHECK1, m_CommBreakDetected);
	DDX_Check(pDX, IDC_CHECK2, m_CommCTSDetected);
	DDX_Check(pDX, IDC_CHECK3, m_CommDSRDetected);
	DDX_Check(pDX, IDC_CHECK4, m_CommERRDetected);
	DDX_Check(pDX, IDC_CHECK5, m_CommRingDetected);
	DDX_Check(pDX, IDC_CHECK6, m_CommRLSDDetected);
	DDX_Check(pDX, IDC_CHECK7, m_CommRxChar);
	DDX_Check(pDX, IDC_CHECK8, m_CommRxCharFlag);
	DDX_Check(pDX, IDC_CHECK9, m_CommTxEmpty);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg message handlers
BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTemp;
	strTemp.Format("%d", m_dcb.BaudRate);

	((CComboBox*) GetDlgItem(IDC_BAUDRATECOMBO))->SelectString(0, strTemp);
	
	((CComboBox*) GetDlgItem(IDC_PARITYCOMBO))->SetCurSel(m_dcb.Parity);

	((CComboBox*) GetDlgItem(IDC_STOPBITCOMBO))->SetCurSel(m_dcb.StopBits);

	strTemp.Format("%d", m_dcb.ByteSize);
	((CComboBox*) GetDlgItem(IDC_DATABITSCOMBO))->SelectString(0, strTemp);
	
	return TRUE;
}

void CConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
