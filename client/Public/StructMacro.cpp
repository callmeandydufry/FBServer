#include "stdafx.h"
#include "StructMacro.h"
#include "DBCore.h"

BOOL DBModelBase::doSQLInsert(DBCore* dbCore)
{
	__GUARD__

	dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLInsertString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: insert into table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLBatchInsert(DBCore* dbCore)
{
	__GUARD__

		dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLBatchInsertString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: insert into table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLUpdate(DBCore* dbCore)
{
	__GUARD__

	dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLUpdateString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: update table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLDelete(DBCore* dbCore)
{
	__GUARD__

		dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLDeleteString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: delete from table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLVallidate(DBCore* dbCore)
{
	__GUARD__

		dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLValidateString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: validate from table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLInvallidate(DBCore* dbCore)
{
	__GUARD__

		dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLInvalidateString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: invalidate from table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLSelect(DBCore* dbCore)
{
	__GUARD__

		dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLSelectString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: select from table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLSelectCount(DBCore* dbCore)
{
	__GUARD__;

	dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();

	String sqlStr = getSQLSelectCountString();
	pQuery->SetSql(sqlStr.c_str());

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: select from table '%s' failed",
			getDBTableName()
		);
		return FALSE;
	}

	m_AffectedRowCount = dbCore->Get_AffectedRowCount();

	return TRUE;
	__UNGUARD__ return FALSE;
}

BOOL DBModelBase::doSQLFetch(DBCore* dbCore, void* structPtr)
{
	if (!dbCore->Fetch())
		return FALSE;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		mAllModelProps[i]->fetchFromDB(dbCore);
		mAllModelProps[i]->writeToProp(structPtr);
	}
	return TRUE;
}

String DBModelBase::getSQLSelectString()
{
	String paramNameStr;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (i > 0)
		{
			paramNameStr += ",";
		}
		paramNameStr += mAllModelProps[i]->getPropName();
	}

	String sqlString = StringUtil::printf("select %s from %s where %s", paramNameStr.c_str(), getDBTableName(), getSQLQueryConditionString().c_str());
	return sqlString;
}

String DBModelBase::getSQLSelectCountString()
{
	String sqlString = StringUtil::printf("select count(*) from %s ", getDBTableName());
	return sqlString;
}

String DBModelBase::getSQLInsertString()
{
	String paramNameStr;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (i > 0)
		{
			paramNameStr += ",";
		}
		paramNameStr += mAllModelProps[i]->getPropName();
	}

	String paramValStr;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (i > 0)
		{
			paramValStr += ",";
		}

		if (mAllModelProps[i]->getPropType() == DBModelProperty::EPT_STRING ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_BINARY ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_FIXEDSTRING)
		{
			// 如果是字符串变量需要用单引号包起来 [12/14/2017 yz]
			paramValStr += "'";
			paramValStr += mAllModelProps[i]->getPropValString();
			paramValStr += "'";
		}
		else
		{
			paramValStr += mAllModelProps[i]->getPropValString();
		}
	}

	String sqlString = StringUtil::printf("insert into %s (%s) values (%s)", getDBTableName(), paramNameStr.c_str(), paramValStr.c_str());
	return sqlString;
}

String DBModelBase::getSQLBatchInsertString()
{
	String paramNameStr;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (i > 0)
		{
			paramNameStr += ",";
		}
		paramNameStr += mAllModelProps[i]->getPropName();
	}

	String paramValStr;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (i > 0)
		{
			paramValStr += ",";
		}

		//paramValStr += "(";

		if (mAllModelProps[i]->getPropType() == DBModelProperty::EPT_STRING ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_BINARY ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_FIXEDSTRING)
		{
			// 如果是字符串变量需要用单引号包起来 [12/14/2017 yz]
			paramValStr += "'";
			paramValStr += mAllModelProps[i]->getPropValString();
			paramValStr += "'";
		}
		else
		{
			paramValStr += mAllModelProps[i]->getPropValString();
		}

		//paramValStr += "),";
	}

	// insert into t_table (name1, name2) values (val1, val2),(val1, val2),(val1, val2)...
	// todo 拼个串

	char szPtr = ';';
	paramValStr[paramValStr.size() - 1] = szPtr;

	String sqlString = StringUtil::printf("insert into %s (%s) values (%s)", getDBTableName(), paramNameStr.c_str(), paramValStr.c_str());
	return sqlString;
}

String DBModelBase::getSQLUpdateString()
{
	String paramString;
	int32 num = 0;
	for (int32 i = 0; i < mAllModelProps.GetSize(); ++i)
	{
		if (!mAllModelProps[i]->isValid())
			continue;

		if (num > 0)
		{
			paramString += ",";
		}

		paramString += mAllModelProps[i]->getPropName();
		paramString += "=";
		if (mAllModelProps[i]->getPropType() == DBModelProperty::EPT_STRING ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_BINARY ||
			mAllModelProps[i]->getPropType() == DBModelProperty::EPT_FIXEDSTRING)
		{
			// 如果是字符串变量需要用单引号包起来 [12/14/2017 yz]
			paramString += "'";
			paramString += mAllModelProps[i]->getPropValString();
			paramString += "'";
		}
		else
		{
			paramString += mAllModelProps[i]->getPropValString();
		}

		num++;
	}

	String sqlString = StringUtil::printf("update %s set %s where %s", getDBTableName(), paramString.c_str(), getSQLQueryConditionString().c_str());
	return sqlString;
}

String DBModelBase::getSQLDeleteString()
{
	String sqlString = StringUtil::printf("delete from %s where %s", getDBTableName(), getSQLQueryConditionString().c_str());
	return sqlString;
}

String DBModelBase::getSQLValidateString()
{
	String sqlString = StringUtil::printf("update %s set valid=1 where %s", getDBTableName(), getSQLQueryConditionString().c_str());
	return sqlString;
}

String DBModelBase::getSQLInvalidateString()
{
	String sqlString = StringUtil::printf("update %s set valid=0 where %s", getDBTableName(), getSQLQueryConditionString().c_str());
	return sqlString;
}