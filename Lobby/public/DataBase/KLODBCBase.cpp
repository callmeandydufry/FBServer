/*$T Share/DBSystem/DataBase/KLODBCBase.cpp GC 1.140 10/10/07 10:06:29 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "KLODBCBase.h"
#include "DBCore.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

KLODBCBase::KLODBCBase()
{
	m_Ret = 0;
	m_nAffectedRowCount = 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 KLODBCBase::GetAffectCount()
{
	return mDBCoreFace->Get_AffectedRowCount();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL KLODBCBase::exeEraseOp()
{
	__GUARD__ if(!IsPrepare()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DBCore	*pCore = mDBCoreFace;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!pCore) return FALSE;

	m_DBOpType = DBOP_DELETE;

	m_Ret = pCore->Execute();

	m_nAffectedRowCount = pCore->Get_AffectedRowCount();

	return m_Ret;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL KLODBCBase::IsPrepare()
{
	__GUARD__ return mDBCoreFace->IsPrepare();

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
uint32 KLODBCBase::Get_AffectedRowCount()
{
	return m_nAffectedRowCount;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
SQLString *KLODBCBase::GetPlayerDBString()
{
	return &mDBCoreFace->GetQuery();
}


/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL KLODBCBase::Save(void *pSource)
{
	__GUARD__ if(!IsPrepare()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DBCore	*pCore = mDBCoreFace;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!pCore) return FALSE;

	m_DBOpType = DBOP_SAVE;

	m_Ret = pCore->Execute();

	m_nAffectedRowCount = pCore->Get_AffectedRowCount();

	return m_Ret;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char *KLODBCBase::GetSQLErrorMsg()
{
	return mDBCoreFace->GetSQLErrorMsg();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int32 KLODBCBase::GetRetCode()
{
	return mDBCoreFace->GetRetCode();
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL KLODBCBase::exeNewOp()
{
	__GUARD__ if(!IsPrepare()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DBCore	*pCore = mDBCoreFace;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!pCore) return FALSE;

	m_DBOpType = DBOP_ADDNEW;

	m_Ret = pCore->Execute();

	m_nAffectedRowCount = pCore->Get_AffectedRowCount();

	return m_Ret;

	__UNGUARD__ return FALSE;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
BOOL KLODBCBase::DB2Memory()
{
	__GUARD__ if(!IsPrepare()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DBCore	*pCore = mDBCoreFace;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!pCore) return FALSE;

	m_DBOpType = DBOP_LOAD;

	m_Ret = pCore->Execute();

	m_nAffectedRowCount = pCore->Get_AffectedRowCount();

	return m_Ret;

	__UNGUARD__ return FALSE;
}

BOOL KLODBCBase::Commit()
{
//#ifdef _ATTR_AUTO_COMMIT_
	__GUARD__

	int32 Result;
	SQLString *pQuery = GetPlayerDBString();
	KCheck(pQuery);
	pQuery->Clear();
	pQuery->Parse("commit;");
	
	return KLODBCBase::Save(&Result);
	__UNGUARD__
//#endif
	return FALSE;
}

BOOL KLODBCBase::Rollback()
{
//#ifdef _ATTR_AUTO_COMMIT_
	__GUARD__

	int32 Result;
	SQLString *pQuery = GetPlayerDBString();
	KCheck(pQuery);
	pQuery->Clear();
	pQuery->Parse("rollback;");
	
	return KLODBCBase::Save(&Result);
	__UNGUARD__
//#endif
	return FALSE;
}
char * KLODBCBase::GetErrorMessage()
{
	return mDBCoreFace->GetErrorMsg();
}

