/*$T Share/DBSystem/DataBase/KLODBCInterface.cpp GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "DBCore.h"


/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::ErrorDiag()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ int32 ii = 1;
	SQLINTEGER	NativeError;
	SQLCHAR		SqlState[6];
	SQLSMALLINT	MsgLen;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	memset(m_szSQLErrorMsg, 0, MAX_SQL_ERROR_MSG);
	while
	(
		m_SQLReturn = SQLGetDiagRec
			(
				SQL_HANDLE_DBC,
				m_hSQLDbc,
				ii,
				SqlState,
				&NativeError,
				m_szSQLErrorMsg,
				sizeof(m_szSQLErrorMsg),
				&MsgLen
			) != SQL_NO_DATA
	)
	{
		ii++;
	}

	m_szSQLErrorMsg[MAX_SQL_ERROR_MSG - 1] = '\0';

	if(strlen((const char *) m_szSQLErrorMsg) == 0)
	{
		m_SQLReturn = SQLError
			(
				m_hSQLEnv,
				m_hSQLDbc,
				m_hSQLStmt,
				SqlState,
				&NativeError,
				m_szSQLErrorMsg,
				sizeof(m_szSQLErrorMsg),
				&MsgLen
			);
	}

	m_RetCode = NativeError;

	switch(m_RetCode)
	{
	case 2601:	break;
	case 3621:	break;
	default:	{ Close(); }
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	ErrorBuffer[10240] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	tsnprintf( ErrorBuffer, 10240, "RetCode = %d,ErrorMessage = %s,Sql=%s", m_RetCode, m_szSQLErrorMsg, m_Query.m_SqlStr );
	__FlushMsg(ErrorBuffer);
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::__FlushMsg(const char *pLog)
{
	__GUARD__ if(strlen(pLog) == 0) return;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	szName[_MAX_PATH] = { 0 };
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	tsnprintf
	(
		szName,
		_MAX_PATH - 1,
		"./Log/database.log"
	);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	FILE	*f = fopen(szName, "a");
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	KCheck(NULL != f);

	char	szContext[MAX_SQL_LENGTH + QLogSystem::ciLog_Temp_Name] = {0};
	tsnprintf
		(
		szContext,
		strlen(szContext) - 1,
		"(%.2d:%.2d:%.2d %.3f %lu) %s",
		GET_TIME().hour(), 
		GET_TIME().minute(), 
		GET_TIME().second(), 
		(float) (GET_TIME().TickCount()) / 1000.0, 
		KGetCurrentTID(),
		pLog
		);

	fwrite(szContext, 1, strlen(szContext), f);
	fwrite("\r\n", 1, 2, f);

	fclose(f);
	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::Connect(const char *ConnectStr, const char *UserName, const char *Password)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Close();
	/*~~~~~~~~~~~~~~~~~~~~*/

	strncpy(m_szConnect, ConnectStr, DB_MAX_STR_LEN);
	strncpy(m_szUserName, UserName, DB_MAX_USER_LEN);
	strncpy(m_szPassWord, Password, DB_MAX_PASSWD_LEN);

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hSQLEnv);
	SQLSetEnvAttr(m_hSQLEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC, m_hSQLEnv, &m_hSQLDbc);
	m_SQLReturn = SQLConnect
		(
			m_hSQLDbc,
			(SQLCHAR *) m_szConnect,
			SQL_NTS,
			(SQLCHAR *) m_szUserName,
			SQL_NTS,
			(SQLCHAR *) m_szPassWord,
			SQL_NTS
		);

	if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
	{
		/*~~~~~~~~~~~~~~~~~*/
		char	LogBuff[512];
		/*~~~~~~~~~~~~~~~~~*/

		memset(LogBuff, 0, 512);
		sprintf(LogBuff, "Connect string: %s", m_szConnect);
		sprintf(LogBuff, "Connect string: %s", m_szUserName);
		ErrorDiag();
		return FALSE;
	}

//#ifdef _ATTR_AUTO_COMMIT_
	if( GetDBOpMode() == DBOPMODE_SAVE )
	{
		m_SQLReturn = SQLSetConnectAttr(m_hSQLDbc,SQL_ATTR_AUTOCOMMIT,0,SQL_IS_INTEGER);
		if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
		{
			ErrorDiag();
			return FALSE;
		}
	}
//#endif

	m_SQLReturn = SQLAllocHandle(SQL_HANDLE_STMT, m_hSQLDbc, &m_hSQLStmt);

	if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
	{
		m_hSQLStmt = 0;
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::Connect()
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Close();
	/*~~~~~~~~~~~~~~~~~~~~*/

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hSQLEnv);
	SQLSetEnvAttr(m_hSQLEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC, m_hSQLEnv, &m_hSQLDbc);

	m_SQLReturn = SQLConnect
		(
			m_hSQLDbc,
			(SQLCHAR *) m_szConnect,
			SQL_NTS,
			(SQLCHAR *) m_szUserName,
			SQL_NTS,
			(SQLCHAR *) m_szPassWord,
			SQL_NTS
		);

	if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
	{
		/*~~~~~~~~~~~~~~~~~*/
		char	LogBuff[512];
		/*~~~~~~~~~~~~~~~~~*/

		memset(LogBuff, 0, 512);
		sprintf(LogBuff, "Connect string: %s", m_szConnect);
		sprintf(LogBuff, "Connect string: %s", m_szUserName);
		ErrorDiag();
		return FALSE;
	}

//#ifdef _ATTR_AUTO_COMMIT_
	if( GetDBOpMode() == DBOPMODE_SAVE )
	{
		m_SQLReturn = SQLSetConnectAttr(m_hSQLDbc,SQL_ATTR_AUTOCOMMIT,0,SQL_IS_INTEGER);
		if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
		{
			ErrorDiag();
			return FALSE;
		}
	}
//#endif


	m_SQLReturn = SQLAllocHandle(SQL_HANDLE_STMT, m_hSQLDbc, &m_hSQLStmt);

	if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO))
	{
		m_hSQLStmt = 0;
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;

	__UNGUARD__ return FALSE;
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */

DBCore::DBCore()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_bConnected = FALSE;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_AffectCount = -1;
	m_SQLReturn = SQL_SUCCESS;
	m_hSQLEnv = 0;
	m_hSQLStmt = 0;
	m_hSQLDbc = 0;
	memset(m_szSQLErrorMsg, 0, MAX_SQL_ERROR_MSG);
	m_Query.Clear();

	m_nColCount = 0;
	m_papszColNames = NULL;
	m_panColType = NULL;
	m_papszColTypeNames = NULL;
	m_panColSize = NULL;
	m_panColPrecision = NULL;
	m_panColNullable = NULL;

	m_papszColValues = NULL;
	m_panColValueLengths = NULL;

	m_ColInfoAllocator.Init(m_szColInfoBuff, MAX_COLUMNINFO_BUFFER);
	m_ColValueAllocator.Init(m_szColValueBuff, MAX_COLUMNVALUE_BUFFER);
	m_uOpMode = DBOPMODE_LOAD;

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::Close()
{
	__GUARD__ if(m_hSQLStmt)
	{
		SQLCloseCursor(m_hSQLStmt);
		SQLFreeStmt(m_hSQLStmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hSQLStmt);
		m_hSQLStmt = NULL;
	}

	if(m_hSQLDbc)
	{
		SQLDisconnect(m_hSQLDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hSQLDbc);
		m_hSQLDbc = NULL;
	}

	if(m_hSQLEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hSQLEnv);
		m_hSQLEnv = NULL;
	}

	m_bConnected = FALSE;

	return TRUE;

	__UNGUARD__ return FALSE;
}



/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::Clear()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	SQLCloseCursor(m_hSQLStmt);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	SQLFreeStmt(m_hSQLStmt, SQL_UNBIND);

	ClearColumnInfo();
	ClearColumnData();

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::Failed(int32 nRetCode)
{
	if(nRetCode == SQL_SUCCESS || nRetCode == SQL_SUCCESS_WITH_INFO) return FALSE;
	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::ClearColumnInfo()
{
	m_ColInfoAllocator.Clear();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
DBCore::~DBCore()
{
	__GUARD__ if(m_hSQLStmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hSQLStmt);
	if(m_hSQLDbc) SQLDisconnect(m_hSQLDbc);
	if(m_hSQLDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hSQLDbc);
	if(m_hSQLEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hSQLEnv);

	__UNGUARD__
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::ClearColumnData()
{
	if(m_nColCount > 0)
	{
		for(int32 iCol = 0; iCol < m_nColCount; iCol++)
		{
			if(m_papszColValues[iCol] != NULL)
			{
				m_ColValueAllocator.Free(m_papszColValues[iCol]);
				m_papszColValues[iCol] = NULL;
			}
		}
	}

	m_ColValueAllocator.Clear();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::Fetch(int32 nOrientation, int32 nOffset)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	ClearColumnData();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(m_hSQLStmt == NULL || m_nColCount < 1) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~*/
	SQLRETURN	nRetCode;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	if(nOrientation == SQL_FETCH_NEXT && nOffset == 0)
	{
		nRetCode = SQLFetch(m_hSQLStmt);
		if(Failed(nRetCode))
		{
			if(nRetCode != SQL_NO_DATA)
			{
				ErrorDiag();
			}

			return FALSE;
		}
	}
	else
	{
		nRetCode = SQLFetchScroll(m_hSQLStmt, (SQLSMALLINT) nOrientation, nOffset);
		if(Failed(nRetCode))
		{
			if(nRetCode == SQL_NO_DATA)
			{
				ErrorDiag();
			}

			return FALSE;
		}
	}

	/*~~~~~~~~~~~~~~~~~*/
	SQLSMALLINT	iCol;
	/*~~~~~~~~~~~~~~~~~*/

	for(iCol = 0; iCol < m_nColCount; iCol++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		char		szWrkData[512];
		SQLLEN	cbDataLen;
		SQLSMALLINT	nFetchType = GetTypeMapping(m_panColType[iCol]);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if(nFetchType != SQL_C_BINARY) nFetchType = SQL_C_CHAR;

		szWrkData[0] = '\0';
		szWrkData[sizeof(szWrkData) - 1] = '\0';

		nRetCode = SQLGetData(m_hSQLStmt, iCol + 1, nFetchType, szWrkData, sizeof(szWrkData), &cbDataLen);
		if(Failed(nRetCode))
		{
			if(nRetCode == SQL_NO_DATA)
			{
				ErrorDiag();
			}

			return FALSE;
		}

		if(cbDataLen == SQL_NULL_DATA)
		{
			m_papszColValues[iCol] = NULL;
			m_panColValueLengths[iCol] = 0;
		}
		else if(nRetCode == SQL_SUCCESS_WITH_INFO)
		{
			if(cbDataLen >= (SQLUINTEGER) (sizeof(szWrkData) - 1))
			{
				cbDataLen = (SQLUINTEGER) (sizeof(szWrkData) - 1);
				if(nFetchType == SQL_C_CHAR)
					while((cbDataLen > 1) && (szWrkData[cbDataLen - 1] == 0)) --cbDataLen;
			}

			m_papszColValues[iCol] = (char *) m_ColValueAllocator.Malloc(cbDataLen + 1);
			memcpy(m_papszColValues[iCol], szWrkData, cbDataLen);
			m_papszColValues[iCol][cbDataLen] = '\0';
			m_panColValueLengths[iCol] = cbDataLen;

			while(TRUE)
			{
				/*~~~~~~~~~~~~~~~~~~~~~~*/
				SQLLEN	nChunkLen;
				/*~~~~~~~~~~~~~~~~~~~~~~*/

				nRetCode = SQLGetData
					(
						m_hSQLStmt,
						(SQLUSMALLINT) iCol + 1,
						nFetchType,
						szWrkData,
						sizeof(szWrkData),
						&cbDataLen
					);
				if(nRetCode == SQL_NO_DATA) break;

				if(Failed(nRetCode))
				{
					ErrorDiag();
					return FALSE;
				}

				if(cbDataLen > (int32) (sizeof(szWrkData) - 1) || cbDataLen == SQL_NO_TOTAL)
				{
					nChunkLen = sizeof(szWrkData) - 1;
					if(nFetchType == SQL_C_CHAR)
						while((nChunkLen > 1) && (szWrkData[nChunkLen - 1] == 0)) --nChunkLen;
				}
				else
					nChunkLen = cbDataLen;
				szWrkData[nChunkLen] = '\0';

				m_papszColValues[iCol] = (char *) m_ColValueAllocator.Realloc
					(
						m_papszColValues[iCol],
						m_panColValueLengths[iCol] + nChunkLen + 1
					);
				memcpy(m_papszColValues[iCol] + m_panColValueLengths[iCol], szWrkData, nChunkLen);
				m_panColValueLengths[iCol] += nChunkLen;
				m_papszColValues[iCol][m_panColValueLengths[iCol]] = '\0';
			}
		}
		else
		{
			m_panColValueLengths[iCol] = cbDataLen;
			m_papszColValues[iCol] = (char *) m_ColValueAllocator.Malloc(cbDataLen + 1);
			memcpy(m_papszColValues[iCol], szWrkData, cbDataLen);
			m_papszColValues[iCol][cbDataLen] = '\0';
		}

		if(nFetchType == SQL_C_CHAR && m_papszColValues[iCol] != NULL)
		{
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			char	*pszTarget = m_papszColValues[iCol];
			size_t	iEnd = strlen(pszTarget);
			/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

			while(iEnd > 0 && pszTarget[iEnd - 1] == ' ') pszTarget[--iEnd] = '\0';
		}
	}

	return TRUE;
	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::Execute()
{
	_NET_TRY
	{
		m_SQLReturn = SQLExecDirect(m_hSQLStmt, (SQLCHAR *) m_Query.m_SqlStr, SQL_NTS);
		if((m_SQLReturn != SQL_SUCCESS) && (m_SQLReturn != SQL_SUCCESS_WITH_INFO) && (m_SQLReturn != SQL_NO_DATA))
		{
			ErrorDiag();
			return FALSE;
		}

		return CollectResultsInfo();
	}

	_NET_CATCH
	{
		__FlushMsg((const char *) m_Query.m_SqlStr);
	}

	return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::Get_Property_Int(int32 ColIndex, int32 &RetCode)
{
	__GUARD__ if(ColIndex > m_nColCount)
	{
		RetCode = QUERY_RESULT_NOCOL;
		KCheck(FALSE);
		return QUERY_RESULT_NOCOL;
	}

	if(m_papszColValues[ColIndex - 1] == NULL)
	{
		RetCode = QUERY_RESULT_NULL;
		KCheck(FALSE);
		return QUERY_RESULT_NULL;
	}
	else
	{
		RetCode = QUERY_RESULT_OK;
		return atoi(GetColData(ColIndex - 1));
	}

	__UNGUARD__ return QUERY_RESULT_NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint64 DBCore::Get_Property_UInt(int32 ColIndex, int32 &RetCode)
{
	__GUARD__ if(ColIndex > m_nColCount)
	{
		RetCode = QUERY_RESULT_NOCOL;
		KCheck(FALSE);
		return QUERY_RESULT_NOCOL;
	}

	if(m_papszColValues[ColIndex - 1] == NULL)
	{
		RetCode = QUERY_RESULT_NULL;
		KCheck(FALSE);
		return QUERY_RESULT_NULL;
	}
	else
	{
		RetCode = QUERY_RESULT_OK;

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		const char	*str = GetColData(ColIndex - 1);
		char		*endp;
		return strtoull(GetColData(ColIndex - 1), &endp, 10);
	}

	__UNGUARD__ return QUERY_RESULT_NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
float DBCore::Get_Property_Float(int32 ColIndex, int32 &RetCode)
{
	__GUARD__ if(ColIndex > m_nColCount)
	{
		RetCode = QUERY_RESULT_NOCOL;
		KCheck(FALSE);
		return QUERY_RESULT_NOCOL;
	}

	if(m_papszColValues[ColIndex - 1] == NULL)
	{
		RetCode = QUERY_RESULT_NULL;
		KCheck(FALSE);
		return QUERY_RESULT_NULL;
	}
	else
	{
		RetCode = QUERY_RESULT_OK;
		return (float) atof(GetColData(ColIndex - 1));
	}

	__UNGUARD__ return QUERY_RESULT_NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::GetString(int32 ColIndex, char *buf, int32 BufLen, int32 &RetCode)
{
	__GUARD__ 

	if(ColIndex > m_nColCount)
	{
		RetCode = QUERY_RESULT_NOCOL;
		KCheck(FALSE);
	}

	if(m_papszColValues[ColIndex - 1] == NULL)
	{
		RetCode = QUERY_RESULT_NULL;
		KCheck(FALSE);
	}
	else
	{
		RetCode = QUERY_RESULT_OK;

		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		int32	nLen = GetColDataLength(ColIndex - 1);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if( nLen <= BufLen )
		{
			strncpy(buf, GetColData(ColIndex - 1), BufLen);
			// 加个保护，确保数据库中取出来的字符串是有结束符的 [yangzhen]
			buf[BufLen - 1] = 0;
		}
		else
		{
			char szMsg[8092] = {0};
			sprintf( szMsg, "buflen=%d,dblen=%d,dbstr=%s", BufLen, nLen, GetColData(ColIndex-1) );
			KCheckEx(FALSE,szMsg);
		}
		return nLen;
	}

	return 0;
	__UNGUARD__ return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::GetField(int32 ColIndex, char *buf, int32 BufLen, int32 &RetCode)
{
	__GUARD__ if(ColIndex > m_nColCount)
	{
		RetCode = QUERY_RESULT_NOCOL;
		KCheck(FALSE);
	}

	if(m_papszColValues[ColIndex - 1] == NULL)
	{
		RetCode = QUERY_RESULT_NULL;
		KCheck(FALSE);
	}
	else
	{
		/*~~~~~~~~~~~~~~~~~~*/
		uint32	OutLength = 0;
		/*~~~~~~~~~~~~~~~~~~*/

		Str2Binary(GetColData(ColIndex - 1), GetColDataLength(ColIndex - 1), buf, BufLen, OutLength);

		if( (int32) OutLength <= BufLen )
		{
			RetCode = QUERY_RESULT_OK;
		}
		else
		{
			char szMsg[8092] = {0};
			sprintf( szMsg, "buflen=%d,dblen=%d,dbstr=%s", BufLen, OutLength, GetColData(ColIndex-1) );
			KCheckEx(FALSE,szMsg);
		}
		
		return OutLength;
	}

	return 0;
	__UNGUARD__ return 0;
}



/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DBCore::CleanEnv()
{
	if(m_hSQLStmt)
	{
		SQLCloseCursor(m_hSQLStmt);
		SQLFreeStmt(m_hSQLStmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hSQLStmt);
		m_hSQLStmt = NULL;
	}

	if(m_hSQLDbc)
	{
		SQLDisconnect(m_hSQLDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hSQLDbc);
		m_hSQLDbc = NULL;
	}

	if(m_hSQLEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hSQLEnv);
		m_hSQLEnv = NULL;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::CollectResultsInfo()
{
	if(!m_bConnected || m_hSQLStmt == NULL)
	{
		return FALSE;
	}

	if(Failed(SQLRowCount(m_hSQLStmt, &m_AffectCount))) return FALSE;
	if(Failed(SQLNumResultCols(m_hSQLStmt, &m_nColCount))) return FALSE;

	ClearColumnInfo();

	m_papszColNames = (char **) m_ColInfoAllocator.Calloc(sizeof(char *), (m_nColCount + 1));
	m_papszColValues = (char **) m_ColInfoAllocator.Calloc(sizeof(char *), (m_nColCount + 1));
	m_panColValueLengths = (SQLLEN *) m_ColInfoAllocator.Calloc(sizeof(SQLLEN), (m_nColCount + 1));

	m_panColType = (int16 *) m_ColInfoAllocator.Calloc(sizeof(int16), m_nColCount);
	m_papszColTypeNames = (char **) m_ColInfoAllocator.Calloc(sizeof(char *), (m_nColCount + 1));
	m_panColSize = (SQLULEN *) m_ColInfoAllocator.Calloc(sizeof(SQLULEN), m_nColCount);
	m_panColPrecision = (int16 *) m_ColInfoAllocator.Calloc(sizeof(int16), m_nColCount);
	m_panColNullable = (int16 *) m_ColInfoAllocator.Calloc(sizeof(int16), m_nColCount);

	for(SQLUSMALLINT iCol = 0; iCol < m_nColCount; iCol++)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		SQLCHAR		szName[256];
		SQLSMALLINT	nNameLength = 0;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		if
		(
			Failed
				(
					SQLDescribeCol
						(
							m_hSQLStmt,
							iCol + 1,
							szName,
							sizeof(szName),
							&nNameLength,
							m_panColType + iCol,
							m_panColSize + iCol,
							m_panColPrecision + iCol,
							m_panColNullable + iCol
						)
				)
		) return FALSE;

		szName[nNameLength] = '\0';

		m_papszColNames[iCol] = (char *) m_ColInfoAllocator.Malloc(nNameLength + 1);
		tsnprintf(m_papszColNames[iCol], nNameLength + 1, "%s", szName);

		if
		(
			Failed
				(
					SQLColAttribute
						(
							m_hSQLStmt,
							iCol + 1,
							SQL_DESC_TYPE_NAME,
							szName,
							sizeof(szName),
							&nNameLength,
							NULL
						)
				)
		) return FALSE;

		szName[nNameLength] = '\0';
		m_papszColTypeNames[iCol] = (char *) m_ColInfoAllocator.Malloc(nNameLength + 1);
		tsnprintf(m_papszColTypeNames[iCol], nNameLength + 1, "%s", szName);
	}

	return TRUE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::GetColCount()
{
	return m_nColCount;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
SQLSMALLINT DBCore::GetTypeMapping(SQLSMALLINT nTypeCode)
{
	switch(nTypeCode)
	{
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
	case SQL_WCHAR:
	case SQL_WVARCHAR:
	case SQL_WLONGVARCHAR:
		return SQL_C_CHAR;

	case SQL_DECIMAL:
	case SQL_NUMERIC:
		return SQL_C_NUMERIC;

	case SQL_SMALLINT:
		return SQL_C_SSHORT;

	case SQL_INTEGER:
		return SQL_C_SLONG;

	case SQL_REAL:
		return SQL_C_FLOAT;

	case SQL_FLOAT:
	case SQL_DOUBLE:
		return SQL_C_DOUBLE;

	case SQL_BIT:
	case SQL_TINYINT:
	case SQL_BIGINT:
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
	case SQL_TYPE_TIMESTAMP:
	case SQL_INTERVAL_MONTH:
	case SQL_INTERVAL_YEAR:
	case SQL_INTERVAL_YEAR_TO_MONTH:
	case SQL_INTERVAL_DAY:
	case SQL_INTERVAL_HOUR:
	case SQL_INTERVAL_MINUTE:
	case SQL_INTERVAL_SECOND:
	case SQL_INTERVAL_DAY_TO_HOUR:
	case SQL_INTERVAL_DAY_TO_MINUTE:
	case SQL_INTERVAL_DAY_TO_SECOND:
	case SQL_INTERVAL_HOUR_TO_MINUTE:
	case SQL_INTERVAL_HOUR_TO_SECOND:
	case SQL_INTERVAL_MINUTE_TO_SECOND:
	case SQL_GUID:
		return SQL_C_CHAR;

	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		return SQL_C_BINARY;

	default:
		return SQL_C_CHAR;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
const char *DBCore::GetColName(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return NULL;
	else
		return m_papszColNames[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int16 DBCore::GetColType(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return -1;
	else
		return m_panColType[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
const char *DBCore::GetColTypeName(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return NULL;
	else
		return m_papszColTypeNames[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int16 DBCore::GetColSize(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return -1;
	else
		return (int16) m_panColSize[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int16 DBCore::GetColPrecision(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return -1;
	else
		return m_panColPrecision[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int16 DBCore::GetColNullable(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return -1;
	else
		return m_panColNullable[iCol];
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
const char *DBCore::GetColData(int32 iCol, const char *pszDefault)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return pszDefault;
	else if(m_papszColValues[iCol] != NULL)
		return m_papszColValues[iCol];
	else
		return pszDefault;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
const char *DBCore::GetColData(const char *pszColName, const char *pszDefault)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32	iCol = GetColId(pszColName);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(iCol == -1)
		return pszDefault;
	else
		return GetColData(iCol, pszDefault);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::GetColDataLength(int32 iCol)
{
	if(iCol < 0 || iCol >= m_nColCount)
		return 0;
	else if(m_papszColValues[iCol] != NULL)
		return(int32) m_panColValueLengths[iCol];
	else
		return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 DBCore::GetColId(const char *pszColName)
{
	for(SQLSMALLINT iCol = 0; iCol < m_nColCount; iCol++)
		if(strcmp(pszColName, m_papszColNames[iCol]) == 0) return iCol;

	return -1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL DBCore::HandleError()
{
	switch(m_RetCode)
	{
	case 2006:
	case 2008:
	case 2013:
	case 2055:
		{
			return Connect();
		}
		break;
	}

	return FALSE;
}




