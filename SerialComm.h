// SerialComm.h : main header file for the SerialComm application
//
#pragma once

#ifdef _MSC_VER
#pragma comment (linker, "/DEFAULTLIB:Setupapi.lib")
#endif

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSerialCommApp:
// See SerialComm.cpp for the implementation of this class
//

class CSerialCommApp : public CWinApp
{
public:
	CSerialCommApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSerialCommApp theApp;