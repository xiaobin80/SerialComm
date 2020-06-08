// SerialCommDoc.h : interface of the CSerialCommDoc class
//


#pragma once

#include <boost/thread/thread.hpp>
#include "sqlite3.h"

class CSerialCommDoc : public CDocument
{
protected: // create from serialization only
	CSerialCommDoc();
	DECLARE_DYNCREATE(CSerialCommDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSerialCommDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	char *m_dbName, *m_tableName;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	int           m_nPort;

	boost::thread workerLogRecord(CStringA strA, int flag, int lineNum);

	int checkDB(char *dbName, char *tableName);
};


