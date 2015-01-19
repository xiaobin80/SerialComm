// SerialCommView.cpp : implementation of the CSerialCommView class
//

#include "stdafx.h"
#include "SerialComm.h"

#include "SerialCommDoc.h"
#include "SerialCommView.h"

#include "ConfigDlg.h"
#include "SerialPort.h"
#include "EnumSerial.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerialCommView

IMPLEMENT_DYNCREATE(CSerialCommView, CFormView)

BEGIN_MESSAGE_MAP(CSerialCommView, CFormView)
	ON_BN_CLICKED(IDC_BTNCONFIG, &CSerialCommView::OnBnClickedBtnconfig)
	ON_BN_CLICKED(IDC_BTNSEND, &CSerialCommView::OnBnClickedBtnsend)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommRx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1LOOP, &CSerialCommView::OnBnClickedCheck1loop)
	ON_BN_CLICKED(IDC_CHECK1HEX, &CSerialCommView::OnBnClickedCheck1hex)
	ON_BN_CLICKED(IDC_BTNCLEAR, &CSerialCommView::OnBnClickedBtnclear)
	ON_COMMAND(ID_FILE_SAVE_AS, &CSerialCommView::OnFileSaveAs)
	ON_COMMAND(ID_APP_EXIT, &CSerialCommView::OnAppExit)
	ON_BN_CLICKED(IDC_BTNSAVE, &CSerialCommView::OnBnClickedBtnsave)
	ON_COMMAND(ID_FILE_SAVE, &CSerialCommView::OnFileSave)
END_MESSAGE_MAP()

// CSerialCommView construction/destruction

CSerialCommView::CSerialCommView()
	: CFormView(CSerialCommView::IDD)
{
	// TODO: add construction code here
	m_nPort = 0;
}

CSerialCommView::~CSerialCommView()
{
	// CloseHandle(m_Ports[0].m_hComm);
}

void CSerialCommView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOPORTNUM, m_strPortNum);
}

BOOL CSerialCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSerialCommView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// Add String
	CArray<SSerInfo,SSerInfo&> asi;

	// Populate the list of serial ports.
	EnumSerialPorts(asi,FALSE/*include all*/);

	for (int ii = 0; ii< asi.GetSize(); ii++) {
		m_strPortNum.AddString(asi[ii].strFriendlyName);
	}
	
	if (asi.IsEmpty()) {
		((CEdit *)GetDlgItem(IDC_EDTSEND))->SetWindowTextA("NOT FOUND");
		((CEdit *)GetDlgItem(IDC_EDTSEND))->EnableWindow(FALSE);

		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->ReplaceSel("NOT FOUND");

		((CButton *)GetDlgItem(IDC_CHECK1LOOP))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BTNSEND))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_BTNCONFIG))->EnableWindow(FALSE);
	}

	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->ShowWindow(SW_HIDE);
}


// CSerialCommView diagnostics

#ifdef _DEBUG
void CSerialCommView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSerialCommView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSerialCommDoc* CSerialCommView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSerialCommDoc)));
	return (CSerialCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CSerialCommView message handlers

void CSerialCommView::OnBnClickedBtnconfig()
{
	// TODO: Add your control notification handler code here

	int nCurSel, nIndex, nIndexLast, nLen, nPortNum = 0;
	CString strTemp, strNum;

	nCurSel = m_strPortNum.GetCurSel();
	m_strPortNum.GetLBText(nCurSel, strTemp);
	nIndex = strTemp.Find("COM");
	nIndexLast = strTemp.Find(")");
	if (nIndex == -1) 
		nLen = 1;
	nLen = (nIndexLast) - (nIndex + 3);
	strNum = strTemp.Mid(nIndex + 3, nLen);
	if (!strNum.IsEmpty()) 
		nPortNum = atoi(strNum) - 1;

	m_nPort = nPortNum + 1;
	// init the ports

	if (m_Ports[0].InitPort(this, nPortNum + 1, nPortNum == 0 ? 19200 : 9600))
		m_Ports[0].StartMonitoring();
	else
	{
		// port not found
		/*
		m_Edit[i].SetWindowText("NOT FOUND");
		m_Edit[i].EnableWindow(FALSE);
		m_ListBox[i].EnableWindow(FALSE);
		*/
		CString strTemp2;
		strTemp2.Format("%d", nPortNum + 1);
		strTemp2 = "COM" + strTemp2 + " NOT FOUND" + "\n";
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->SetSel(sizeof(strTemp2), sizeof(strTemp2));
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->ReplaceSel(strTemp2);
	}
			

    // 
	CConfigDlg *dlg = new CConfigDlg(this, m_Ports[0].GetDCB());

	dlg->m_strSendBuffer.Format("%d", m_Ports[0].GetWriteBufferSize());

	DWORD dwCommEvents = m_Ports[0].GetCommEvents();
	dlg->m_CommBreakDetected = (dwCommEvents & EV_BREAK) > 0 ? TRUE : FALSE;
	dlg->m_CommCTSDetected   = (dwCommEvents & EV_CTS) > 0 ? TRUE : FALSE;
	dlg->m_CommDSRDetected   = (dwCommEvents & EV_DSR) > 0 ? TRUE : FALSE;
	dlg->m_CommERRDetected   = (dwCommEvents & EV_ERR) > 0 ? TRUE : FALSE;
	dlg->m_CommRingDetected  = (dwCommEvents & EV_RING) > 0 ? TRUE : FALSE;
	dlg->m_CommRLSDDetected  = (dwCommEvents & EV_RLSD) > 0 ? TRUE : FALSE;
	dlg->m_CommRxChar        = (dwCommEvents & EV_RXCHAR) > 0 ? TRUE : FALSE;
	dlg->m_CommRxCharFlag    = (dwCommEvents & EV_RXFLAG) > 0 ? TRUE :FALSE;
	dlg->m_CommTxEmpty       = (dwCommEvents & EV_TXEMPTY) > 0 ? TRUE : FALSE;

	if (dlg->DoModal() == IDOK) {
		DWORD dwCommEvents = 0;
		if (dlg->m_CommBreakDetected)
			dwCommEvents |= EV_BREAK;
		if (dlg->m_CommCTSDetected)
			dwCommEvents |= EV_CTS;
		if (dlg->m_CommDSRDetected)
			dwCommEvents |= EV_DSR;
		if (dlg->m_CommERRDetected)
			dwCommEvents |= EV_ERR;
		if (dlg->m_CommRingDetected)
			dwCommEvents |= EV_RING;
		if (dlg->m_CommRLSDDetected)
			dwCommEvents |= EV_RLSD;
		if (dlg->m_CommRxChar)
			dwCommEvents |= EV_RXCHAR;
		if (dlg->m_CommRxCharFlag)
			dwCommEvents |= EV_RXFLAG;
		if (dlg->m_CommTxEmpty)
			dwCommEvents |= EV_TXEMPTY;

		m_Ports[0].InitPort(this, nPortNum + 1, 
			atoi(dlg->m_strBaudRate),
			dlg->m_strParity[0],
			atoi(dlg->m_strDataBits),
			atoi(dlg->m_strStopBit),
			dwCommEvents,
			atoi(dlg->m_strSendBuffer));

		m_Ports[0].StartMonitoring();

	}

	delete dlg;
}

void CSerialCommView::OnBnClickedBtnsend()
{
	// TODO: Add your control notification handler code here
	char buff[100];
	GetDlgItemText(IDC_EDTSEND, buff, sizeof(buff));
	if (strcmp("NOT FOUND", buff) == 0)
		return;
	m_Ports[0].WriteToPort(buff);

}

LRESULT CSerialCommView::OnCommRx(WPARAM ch, LPARAM port)
{
	if (port <= 0 || port > 14) 
		return -1;

	CString strTemp2;
	int nLen1, nLen2;
	char *pTemp = (char *)malloc(4);
	
    
    // LOG1
	Hex_Str(ch, pTemp);

    nLen1 = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->GetWindowTextLengthA();
    ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->SetSel(nLen1, nLen1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->ReplaceSel(pTemp);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);

	// free(pTemp);
	delete [] pTemp;
	

    // LOG2
    strTemp2 += (char)ch;

    nLen2 = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->GetWindowTextLengthA();
    ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->SetSel(nLen2, nLen2);
    ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->ReplaceSel(strTemp2);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->PostMessageA(WM_VSCROLL, SB_BOTTOM, 0);

    
    return 0;

}


void CSerialCommView::OnTimer(UINT nIDEvent)
{
	char buff[100];
	CString strTemp;
	GetDlgItemText(IDC_EDTSEND, buff, sizeof(buff));
	if (strcmp("NOT FOUND", buff) == 0)
		return;

	m_Ports[0].WriteToPort(buff);

	CFormView::OnTimer(nIDEvent);
}
void CSerialCommView::OnBnClickedCheck1loop()
{
	// TODO: Add your control notification handler code here
    CString strTemp;
	int nTimer, nCurSel;
	DWORD dwCheck;

	assert(m_nPort > 1);
	dwCheck = ((CButton *)GetDlgItem(IDC_CHECK1LOOP))->GetCheck();
	//
	

	if (!dwCheck)
		KillTimer(1);
	else {
		nCurSel = ((CComboBox *) GetDlgItem(IDC_COMBO1Timer))->GetCurSel();
		((CComboBox *) GetDlgItem(IDC_COMBO1Timer))->GetLBText(nCurSel, strTemp);
		if (!strTemp.IsEmpty())
			nTimer = atoi(strTemp);
		else
			nTimer = 1000;
		SetTimer(1, nTimer,NULL);
	}
}

void CSerialCommView::OnBnClickedCheck1hex()
{
	// TODO: Add your control notification handler code here
	DWORD dwCheck;
	int nLineCount, nLineIndex, nLineLen;
	CString strText;
	dwCheck = ((CButton *) GetDlgItem(IDC_CHECK1HEX))->GetCheck();	

	nLineCount = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->GetLineCount();

	if (dwCheck) {
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->ShowWindow(SW_NORMAL);
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->ShowWindow(SW_HIDE);

#ifdef _DEBUG
		for (int j = 0; j < nLineCount; j++) {
			nLineIndex = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->LineIndex(j);
			nLineLen = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->LineLength(nLineIndex);

			((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->GetLine(j, strText.GetBufferSetLength(nLineLen + 1));
			strText.SetAt(nLineLen, _T('\0'));
			strText.ReleaseBuffer(nLineLen + 1);

			AfxMessageBox(strText);
		}
#endif

	} else {
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->ShowWindow(SW_HIDE);
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->ShowWindow(SW_NORMAL);
	}
}

char *CSerialCommView::Hex_Str(unsigned char hex, char *str)
{
    char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	// *str++ = '0';
    // *str++ = 'x';
    *str++ = hex_chars[hex >>  4];
    *str++ = hex_chars[hex & 0xF];
    *str++ = 0x20;
	*str++ = '\0';

	return str;
}
void CSerialCommView::OnBnClickedBtnclear()
{
	// TODO: Add your control notification handler code here
	// LOG1
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->SetReadOnly(FALSE);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->SetSel(0, -1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->Clear();
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->SetReadOnly(TRUE);

	// LOG2
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->SetReadOnly(FALSE);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->SetSel(0, -1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->Clear();
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->SetReadOnly(TRUE);
}

void CSerialCommView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
    CString strPathName;
	CString strFileName;
	CTime   myTime;
	int     nYear;
	int     nMonth;
	int     nDay;
	int     nHour;
	int     nMinute;
	int     nSecond;

	myTime = CTime::GetCurrentTime();

	nYear   = myTime.GetYear();
	nMonth  = myTime.GetMonth();
	nDay    = myTime.GetDay();
	nHour   = myTime.GetHour();
	nMinute = myTime.GetMinute();
	nSecond = myTime.GetSecond();

	strFileName.Format("%d%d%d%d%d%d", nYear, nMonth, nDay, nHour, nMinute, nSecond);
	strFileName = "D" + strFileName + ".rtf";


	CFileDialog fSaveDialog(FALSE,                  // Set to TRUE to construct a File Open dialog box or 
		                                            // FALSE to construct a File Save As dialog box.
		".rtf", 
		strFileName, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		"Serial Comm Data (*.rtf)|*.rtf|Text (*.txt)|*.txt||", 
		this);
	fSaveDialog.m_pOFN->lpstrTitle = "Serial Comm Data File (Save As...)";
	fSaveDialog.m_pOFN->lpstrInitialDir = "D:";

	if (fSaveDialog.DoModal() == IDOK)
		strPathName = fSaveDialog.GetPathName();
	else
		return;

	CFile cFile(TEXT(strPathName), CFile::modeCreate|CFile::modeWrite);
	EDITSTREAM es;

	if (((CButton *)GetDlgItem(IDC_CHECK1HEX))->GetCheck()) {
		es.dwCookie = (DWORD) &cFile;
	    es.pfnCallback = MyStreamOutCallback;

		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG2))->StreamOut(SF_RTF, es);
	} else {
		es.dwCookie = (DWORD) &cFile;
	    es.pfnCallback = MyStreamOutCallback;

	    ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2LOG1))->StreamOut(SF_RTF, es);
	}



	
	
}

void CSerialCommView::OnAppExit()
{
	// TODO: Add your command handler code here
	// DestroyWindow();
    // exit(EXIT_SUCCESS);  
    // AfxGetMainWnd()->SendMessage(WM_CLOSE);
	m_Ports[0].StopMonitoring();
	PostQuitMessage(0);
}

void CSerialCommView::OnBnClickedBtnsave()
{
	// TODO: Add your control notification handler code here
	CSerialCommView::OnFileSaveAs();
}

DWORD CALLBACK CSerialCommView::MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CFile *pFile = (CFile *)dwCookie;

	pFile->Write(pbBuff, cb);
	*pcb = cb;

	return 0;
}
void CSerialCommView::OnFileSave()
{
	// TODO: Add your command handler code here
	CSerialCommView::OnFileSaveAs();
}
