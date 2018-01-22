/*$T Share/DBSystem/DataBase/KLDBCore.h GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __DB_CORE_H__
#define __DB_CORE_H__

#include "TypeDefine.h"
#include "DBConst.h"

// sql.h中会定义下面两个类型，防止重复定义，去掉宏定义 [2/15/2017 yz]
#ifdef HANDLE
#undef HANDLE
#endif

#ifdef HINSTANCE
#undef HINSTANCE
#endif

#include "sql.h"
#include "sqlext.h"
#include "Static_Allocator.h"

#define MAX_COLUMNINFO_BUFFER	8192
#define MAX_COLUMNVALUE_BUFFER	409600
#define MAX_LONG_COLUMN_BUFFER	200000
#define MAX_SQL_ERROR_MSG		255

#define DBOPMODE_LOAD	0
#define DBOPMODE_SAVE	1

class	DBCore
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	enum { QUERY_RESULT_OK=0, QUERY_RESULT_NULL=-100, QUERY_RESULT_NOCOL=-102 };

	DBCore();
	~DBCore();

	BOOL Connect(const char *ConnectStr, const char *UserName = NULL, const char *Password = NULL);
	BOOL Connect();

	BOOL Close();

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	int32 GetRetCode()
	{
		return m_RetCode;
	}

	void	__FlushMsg(const char *pLog);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	int32 Get_AffectedRowCount()
	{
		return (int32)m_AffectCount;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	BOOL IsPrepare()
	{
		return IsConnected();
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	void SetRetCode(int32 error)
	{
		m_RetCode = error;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	char *GetSQLErrorMsg()
	{
		return (char *) m_szSQLErrorMsg;
	}

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	BOOL IsConnected()
	{
		return m_bConnected;
	}

	int32	Get_Property_Int(int32 ColIndex, int32 &RetCode);

	int32	GetField(int32 nCol, char *buf, int32 BufLen, int32 &RetCode);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uchar Get_Property_UChar(int32 ColIndex, int32 &RetCode)
	{
		return(uchar) Get_Property_Int(ColIndex, RetCode);
	}

	uint64	Get_Property_UInt(int32 ColIndex, int32 &RetCode);

	void	Clear();

	BOOL	Fetch(int32 nOrientation = SQL_FETCH_NEXT, int32 nOffset = 0);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */

	SQLString &GetQuery()
	{
		return m_Query;
	}

	BOOL	Execute();

	float	Get_Property_Float(int32 ColIndex, int32 &RetCode);

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	uint16 Get_Property_UInt16(int32 ColIndex, int32 &RetCode)
	{
		return(uint16) Get_Property_Int(ColIndex, RetCode);
	}

	void	CleanEnv();

	/*
	 ===============================================================================================================
	 ===============================================================================================================
	 */
	int16 Get_Property_Int16(int32 ColIndex, int32 &RetCode)
	{
		return(int16) Get_Property_Int(ColIndex, RetCode);
	}

	int32	GetString(int32 nCol, char *buf, int32 BufLen, int32 &RetCode);

	void	ErrorDiag();



/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	int32		CollectResultsInfo();
	void		ClearColumnInfo();
	void		ClearColumnData();

	int32		GetColCount();

	const char	*GetColName(int32 iCol);

	int16		GetColType(int32 iCol);

	const char	*GetColTypeName(int32 iCol);

	int16		GetColSize(int32 iCol);

	int16		GetColPrecision(int32 iCol);

	int16		GetColNullable(int32 iCol);

	int32		GetColId(const char *pszColName);

	const char	*GetColData(const char *pszColName, const char *pszDefault = NULL);

	const char	*GetColData(int32 iCol, const char *pszDefault = NULL);

	int32		GetColDataLength(int32);

	uchar		GetDBOpMode() { return m_uOpMode; }
	void		SetDBOpMode(uchar u) { m_uOpMode=u; }

	char *GetErrorMsg()
	{
		return (char *) m_szSQLErrorMsg;
	}

	int32 GetError()
	{
		return m_RetCode;
	}


/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
private:
	char			m_szConnect[DB_MAX_STR_LEN];
	char			m_szUserName[DB_MAX_USER_LEN];
	char			m_szPassWord[DB_MAX_PASSWD_LEN];
	uchar			m_uOpMode;

	SQLHSTMT		m_hSQLStmt;
	SQLRETURN		m_SQLReturn;

	SQLString		m_Query;
	SQLINTEGER		m_RetCode;
	SQLCHAR			m_szSQLErrorMsg[MAX_SQL_ERROR_MSG];

	SQLLEN			m_AffectCount;
	BOOL			m_bConnected;
	SQLHENV			m_hSQLEnv;
	SQLHDBC			m_hSQLDbc;

	SQLSMALLINT		m_nColCount;
	char			**m_papszColNames;
	SQLSMALLINT		*m_panColType;
	char			**m_papszColTypeNames;
	SQLULEN			*m_panColSize;
	SQLSMALLINT		*m_panColPrecision;
	SQLSMALLINT		*m_panColNullable;

	char			**m_papszColValues;
	SQLLEN			*m_panColValueLengths;

	Static_Allocator	m_ColInfoAllocator;
	char			m_szColInfoBuff[MAX_COLUMNINFO_BUFFER];

	Static_Allocator	m_ColValueAllocator;  
	char			m_szColValueBuff[MAX_COLUMNVALUE_BUFFER];

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
protected:
	int32		Failed(int32);

	SQLSMALLINT	GetTypeMapping(SQLSMALLINT nTypeCode);

	BOOL		HandleError();
};
#endif
