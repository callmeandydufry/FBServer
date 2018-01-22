/*$T Share/DBSystem/DataBase/KLODBCBase.h GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __KLODBC_BASE_H__
#define __KLODBC_BASE_H__

#include "TypeDefine.h"
#include "DBConst.h"
#include "DBCore.h"

class	KLODBCBase
{
/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	enum DATABASE_OP_TYPE { DBOP_LOAD, DBOP_SAVE, DBOP_ADDNEW, DBOP_DELETE, };

	enum DATABASE_ERROR { ERROR_REDUPLICATE_PRIMARY_KEY = -2, ERROR_REDUPLICATE_ACCOUNT = -3};

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	KLODBCBase();

protected:
	SQLString			*GetPlayerDBString();
	int32				GetAffectCount();
	BOOL				IsPrepare();

/*
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */
public:
	virtual BOOL		DB2Memory();
	virtual BOOL		exeNewOp();
	virtual BOOL		exeEraseOp();
	virtual BOOL		Save(void *pSource);
	virtual BOOL		FetchDATAS(void *pResult) = 0;

public:
	uint32				Get_AffectedRowCount();
	int32				GetRetCode();
	char				*GetSQLErrorMsg();

	BOOL				Commit();
	BOOL				Rollback();

	char               *GetErrorMessage();

protected:
	int32				m_nAffectedRowCount;
	BOOL				m_Ret;
	DATABASE_OP_TYPE	m_DBOpType;
	DBCore				*mDBCoreFace;
};
#endif
