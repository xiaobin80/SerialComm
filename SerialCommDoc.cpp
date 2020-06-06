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
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(m_dbName, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		// insert sql
		
	}

	sqlite3_close(db);
}

static int sqlite_callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

int CSerialCommDoc::checkDB(char *dbName, char *tableName) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	int len_db, len_table = 0;

	rc = sqlite3_open(dbName, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(-1);
	}
	else {
		if (checkTable(db, tableName) != 0)
			return(-2);
		else {
			/// Assignment to member variables
			len_db = strlen(dbName) + 1;
			m_dbName = (char *)malloc(len_db);
			strcpy_s(m_dbName, len_db, dbName);
			len_table = strlen(tableName) + 1;
			m_tableName = (char *)malloc(len_table);
			strcpy_s(m_tableName, len_table, tableName);
		}
	}

	sqlite3_close(db);

	return 0;
}

int checkTable(sqlite3 *db, char *tableName) {
	int rc;
	char *zErrMsg = 0;
	const char* data = "Callback function called";

	char *sql;
	int len, len1 = 0;
	
	/// Create SQL statement
	char *sql1 = "SELECT * from ";
	len1 = strlen(sql1);

	sql = (char *)malloc(len1 + 128);
	memset(sql, 0, sizeof(sql));

	strcat_s(sql, len1 + 1, sql1);

	len = strlen(sql) + strlen(tableName) + 1;
	strcat_s(sql, len + 1, tableName);

	/// Execute SQL statement
	rc = sqlite3_exec(db, sql, sqlite_callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return(-1);
	}

	return 0;
}
