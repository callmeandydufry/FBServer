

/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "stdafx.h"
#include "DBCore.h"
#include "SqlStatement.h"
#include "Struct.h"
#include "ArchiveInterface.h"


DBCheckConnection::DBCheckConnection(DBCore *pCore)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Clear();
	/*~~~~~~~~~~~~~~~~~~~~*/

	KCheck(pCore);
	mDBCoreFace = pCore;

	__UNGUARD__
}

void DBCheckConnection::Clear()
{
	m_Ret = 0;
	m_nAffectedRowCount = 0;
}

BOOL DBCheckConnection::DB2Memory()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ SQLString	*pQuery = GetPlayerDBString();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	pQuery->Parse(CheckDBConnection);

	return KLODBCBase::DB2Memory();

	__UNGUARD__ return FALSE;
}

BOOL DBCheckConnection::Save()
{
	return TRUE;
}

BOOL DBCheckConnection::FetchDATAS(void *pResult)
{
	/*~~~~~~~~~~~~~~~~*/
	__GUARD__ enum { DB_MaxSNID = 1 };
	KCheck(mDBCoreFace);
	int32	RetCode;
	/*~~~~~~~~~~~~~~~~*/

	if (m_nAffectedRowCount <= 0) return FALSE;

	if (!mDBCoreFace->Fetch()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	SNID_t	maxSNID = mDBCoreFace->Get_Property_UInt(DB_MaxSNID, RetCode);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	mDBCoreFace->Clear();

	return TRUE;

	__UNGUARD__ return FALSE;
}


DBCheckVersion::DBCheckVersion(DBCore *pCore)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Clear();
	/*~~~~~~~~~~~~~~~~~~~~*/

	KCheck(pCore);
	mDBCoreFace = pCore;

	__UNGUARD__
}

void DBCheckVersion::Clear()
{
	m_Ret = 0;
	m_nAffectedRowCount = 0;
}

BOOL DBCheckVersion::DB2Memory()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ SQLString	*pQuery = GetPlayerDBString();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	pQuery->Parse(CheckDBVersion);

	return KLODBCBase::DB2Memory();

	__UNGUARD__ return FALSE;
}

BOOL DBCheckVersion::Save()
{
	return TRUE;
}

BOOL DBCheckVersion::FetchDATAS(void *pResult)
{
	return TRUE;
}

BOOL DBCheckVersion::FetchDATAS(int32 dbVersion)
{
	__GUARD__ enum { DB_Version = 1 };
	KCheck(mDBCoreFace);
	int32	RetCode;
	/*~~~~~~~~~~~~~~~~*/

	if (m_nAffectedRowCount <= 0) return FALSE;

	if (!mDBCoreFace->Fetch()) return FALSE;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int32 nVerNum = mDBCoreFace->Get_Property_Int(DB_Version, RetCode);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	if (nVerNum != dbVersion)
	{
		return FALSE;
	}

	mDBCoreFace->Clear();

	return TRUE;

	__UNGUARD__ return FALSE;
}
