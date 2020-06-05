// SerialCommView.h : interface of the CSerialCommView class
//


#pragma once
#include "afxwin.h"

#include "SerialPort.h"
#include "afxcmn.h"


class CSerialCommView : public CFormView
{
protected: // create from serialization only
	CSerialCommView();
	DECLARE_DYNCREATE(CSerialCommView)

public:
	enum{ IDD = IDD_SERIALCOMM_FORM };

// Attributes
public:
	CSerialCommDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CSerialCommView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSerialPort    m_Ports[1];

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_strPortNum;
	int           m_nPort;

	afx_msg void OnBnClickedBtnconfig();
	afx_msg void OnBnClickedBtnsend();
	afx_msg LRESULT OnCommRx(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCheck1loop();
	afx_msg void OnBnClickedCheck1hex();
	afx_msg void OnBnClickedBtnclear();
	afx_msg void OnFileSaveAs();
	afx_msg void OnAppExit();
	afx_msg void OnBnClickedBtnsave();

public:
	char *Hex_Str(unsigned char hex, char *str);
	static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

	afx_msg void OnFileSave();
	afx_msg void OnMnurecord();
};

#ifndef _DEBUG  // debug version in SerialCommView.cpp
inline CSerialCommDoc* CSerialCommView::GetDocument() const
   { return reinterpret_cast<CSerialCommDoc*>(m_pDocument); }
#endif

