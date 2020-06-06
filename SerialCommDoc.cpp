// SerialCommDoc.cpp : implementation of the CSerialCommDoc class
//

#include "stdafx.h"
#include "SerialComm.h"

#include "SerialCommDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerialCommDoc

IMPLEMENT_DYNCREATE(CSerialCommDoc, CDocument)

BEGIN_MESSAGE_MAP(CSerialCommDoc, CDocument)
END_MESSAGE_MAP()


// CSerialCommDoc construction/destruction

CSerialCommDoc::CSerialCommDoc()
{
	// TODO: check SQLite3 database and table
	try
	{

	}
	catch (const std::exception&)
	{

	}
}

CSerialCommDoc::~CSerialCommDoc()
{
}

BOOL CSerialCommDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSerialCommDoc serialization

void CSerialCommDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSerialCommDoc diagnostics

#ifdef _DEBUG
void CSerialCommDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSerialCommDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSerialCommDoc commands
boost::thread 
CSerialCommDoc::workerLogRecord(CString str, int flag, int lineNum) {
	// insert sql
}
