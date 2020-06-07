// SerialCommDoc.cpp : implementation of the CSerialCommDoc class
//

#include "stdafx.h"
#include "SerialComm.h"

/// Nested reference
///#include "SerialCommView.h"

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
	m_nPort = 0;
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
char *getLocalTime(char *dateSeparator, char *timeSeparator, char *dateTimeSeparator)
{
	errno_t err;

	struct tm newtime;
	time_t long_time;

	char *result;
	char year[5];
	char month[3];
	char chMonth[2];
	char day[3];
	char chDay[2];
	char hour[3];
	char chHour[2];
	char minut[3];
	char chMinut[2];

	char *pZero = "0";

	result = (char *)malloc(20);
	memset(result, 0, sizeof(result));
	memset(month, 0, sizeof(month));
	memset(day, 0, sizeof(day));
	memset(hour, 0, sizeof(hour));
	memset(minut, 0, sizeof(minut));

	time(&long_time);
	err = localtime_s(&newtime, &long_time);

	int iYear = newtime.tm_year;
	int iMonth = newtime.tm_mon;
	int iDay = newtime.tm_mday;
	int iHour = newtime.tm_hour;
	int iMinut = newtime.tm_min;

	int iLenZero = strlen(pZero);
	int iLenDateSeparator = strlen(dateSeparator);
	int iLenTimeSeparator = strlen(timeSeparator);


	/// Use zero padding
	iYear += 1900;
	err = _itoa_s(iYear, year, sizeof(year), 10);

	iMonth += 1;
	if (iMonth > 9)
		err = _itoa_s(iMonth, month, 3, 10);
	else {
		strcat_s(month, iLenZero + 1, pZero);
		err = _itoa_s(iMonth, chMonth, sizeof(chMonth), 10);
		strcat_s(month, strlen(month) + strlen(chMonth) + 1, chMonth);
	}

	if (iDay > 9)
		err = _itoa_s(iDay, day, 3, 10);
	else {
		strcat_s(day, iLenZero + 1, pZero);
		err = _itoa_s(iDay, chDay, sizeof(chDay), 10);
		strcat_s(day, strlen(day) + strlen(chDay) + 1, chDay);
	}

	if (iHour > 9) {
		err = _itoa_s(iHour, hour, 3, 10);
	}
	else {
		strcat_s(hour, iLenZero + 1, pZero);
		err = _itoa_s(iHour, chHour, sizeof(chHour), 10);
		strcat_s(hour, strlen(hour) + strlen(chHour) + 1, chHour);
	}

	if (iMinut > 9)
		err = _itoa_s(iMinut, minut, 3, 10);
	else {
		strcat_s(minut, iLenZero + 1, pZero);
		err = _itoa_s(iMinut, chMinut, sizeof(chMinut), 10);
		strcat_s(minut, strlen(minut) + strlen(chMinut) + 1, chMinut);
	}


	/// year - month - day Hour : Minut
	// year
	strcat_s(result, strlen(year) + 1, year);
	strcat_s(result, strlen(result) + iLenDateSeparator + 1, dateSeparator);

	// month
	strcat_s(result, strlen(result) + strlen(month) + 1, month);
	strcat_s(result, strlen(result) + iLenDateSeparator + 1, dateSeparator);

	// day
	strcat_s(result, strlen(result) + strlen(day) + 1, day);
	strcat_s(result, strlen(result) + sizeof(dateTimeSeparator) + 1, dateTimeSeparator);

	// hour
	strcat_s(result, strlen(result) + strlen(hour) + 1, hour);
	strcat_s(result, strlen(result) + iLenTimeSeparator + 1, timeSeparator);

	// minut
	strcat_s(result, strlen(result) + strlen(minut) + 1, minut);

	return result;
}

/// Generate sql string of values
char *genStringSql(int iSize, int iArgc, char *pValues[]) {
	char *result;
	int len = 0;

	result = (char *)malloc(iSize);
	memset(result, 0, sizeof(result));

	for (int i = 0; i < iArgc; i++) {
		len = strlen(result);
		strcat_s(result, len + strlen(pValues[i]) + 1, pValues[i]);
	}

	return result;
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

boost::thread 
CSerialCommDoc::workerLogRecord(CStringA strA, int flag, int lineNum) {
	errno_t err;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	int   memSize = 512;
	char *pStrSql;
	char *pArgv[18];

	char *strInsert = "INSERT INTO ";
	char *columns = "(com_num, line_data, line_num, date, log_flag)";
	char *strVal_b = " VALUES(";
	char *strVal_e = ");";
	char *strPortNum;
	char *sqlSeparator = ",";
	char *sqlApostrophe = "'";
	char *strLn;
	char *strFlag;
	char *strLocalTime = getLocalTime("-", ":", " ");
	char *str;

	rc = sqlite3_open(m_dbName, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		// insert sql
		pStrSql = (char *)malloc(memSize);
		memset(pStrSql, 0, sizeof(pStrSql));
		
		// port number
		strPortNum = (char *)malloc(2);
		memset(strPortNum, 0, sizeof(strPortNum));
		/// Nested reference
		/// m_nPort was originally a member variable of CSerialCommView.
		//POSITION posView = GetFirstViewPosition();
		//CSerialCommView *pView = (CSerialCommView *)GetNextView(posView);
		err = _itoa_s(m_nPort, strPortNum, sizeof(strPortNum), 10);
		
		// line number
		strLn = (char *)malloc(2);
		memset(strLn, 0, sizeof(strLn));
		err = _itoa_s(lineNum, strLn, sizeof(strLn), 10);
		
		// log1 or log2
		strFlag = (char *)malloc(2);
		memset(strFlag, 0, sizeof(strFlag));
		err = _itoa_s(flag, strFlag, sizeof(strFlag), 10);
		
		// Converting from CString
		str = (char *)malloc(128);
		memset(str, 0, sizeof(str));
		/// https://docs.microsoft.com/en-us/cpp/text/how-to-convert-between-various-string-types?view=vs-2019#converting-from-cstring
		int len = strA.GetLength() + 1;
		strcpy_s(str, len, strA.GetString());

		// Assignment pArgv
		pArgv[0] = strInsert;
		pArgv[1] = m_tableName;
		pArgv[2] = columns;
		pArgv[3] = strVal_b;
		pArgv[4] = strPortNum;
		pArgv[5] = sqlSeparator;
		pArgv[6] = sqlApostrophe;
		pArgv[7] = str;
		pArgv[8] = sqlApostrophe;
		pArgv[9] = sqlSeparator;
		pArgv[10] = strLn;
		pArgv[11] = sqlSeparator;
		pArgv[12] = sqlApostrophe;
		pArgv[13] = strLocalTime;
		pArgv[14] = sqlApostrophe;
		pArgv[15] = sqlSeparator;
		pArgv[16] = strFlag;
		pArgv[17] = strVal_e;

		char *pSql = genStringSql(memSize, 18, pArgv);

		/// Execute SQL statement
		rc = sqlite3_exec(db, pSql, sqlite_callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
	}

	sqlite3_close(db);

	return boost::thread();
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

int CSerialCommDoc::checkDB(char *dbName, char *tableName) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	int len_db, len_table = 0;

	rc = sqlite3_open(dbName, &db);

	if (rc) {
		fprintf_s(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
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
