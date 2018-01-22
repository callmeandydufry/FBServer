#include "stdafx.h"
#include "SnidallocateeDBProxy.h"
#include "DBOper.h"

#if 0

// 获取DB中有多少条SNID/Account数据 [1/5/2018 Chief] 
int32 SnidAllocateeDBProxy::doFetchAllSnidNum()
{
	int32 nCount = 0;
	__GUARD__;
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Snid, 0);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchRegisterPlayerInSnidRange DBConnection disconnect error");
		return FALSE;
	}

	mDBSnidAllocState.clear();
	mDBSnidAllocState.setSnidRange(1, INVALID_SNID);
	if (mDBSnidAllocState.doSQLSelectCount(dbCore))
	{
		if (!dbCore->Fetch())
			return nCount;

		int32 nRet;
		nCount = dbCore->Get_Property_Int(1, nRet);
	}

	return nCount;
	__UNGUARD__;
	return nCount;
}

// 批量获取 [1/5/2018 Chief]
BOOL SnidAllocateeDBProxy::doFetchSnidDataByNum(SNID_t u64Snid, int32 nFetchNum, BatchSnidAllocArchive& stArchive)
{
	__GUARD__;
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Snid, 0);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	stArchive.clear();

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchRegisterPlayerInSnidRange DBConnection disconnect error");
		return FALSE;
	}

	mDBSnidAllocState.clear();
	mDBSnidAllocState.setSnidRange(u64Snid, u64Snid + nFetchNum);
	mDBSnidAllocState.setCountLimit(nFetchNum);
	if (mDBSnidAllocState.doSQLSelect(dbCore))
	{
		if (mDBSnidAllocState.getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < nFetchNum && i < mDBSnidAllocState.getAffectedRowCount(); ++i)
			{
				mDBSnidAllocState.doSQLFetch(dbCore, &stArchive.mBatchSnidData[stArchive.mBatchNum].mDBState);
				stArchive.mBatchNum++;
			}

			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__;
	return FALSE;
}

// 批量插入SNID数据 [1/5/2018 Chief]
BOOL SnidAllocateeDBProxy::doBatchInsertData(int32 nInsertNum, SNID_t n64Snid)
{
	__GUARD__;
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Snid, 0);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchRegisterPlayerInSnidRange DBConnection disconnect error");
		return FALSE;
	}

	// todo 如何拼成批量出入的串

// 	mDBSnidAllocState.clear();
// 	mDBSnidAllocState.setSnidRange(n64Snid, n64Snid + nInsertNum);
// 	mDBSnidAllocState.setCountLimit(nInsertNum);
// 	BOOL bRet = mDBSnidAllocState.doSQLBatchInsert(dbCore);

	DBSnidAllocPro stDBState;

	for (int32 i = 0; i < nInsertNum; ++i)
	{
		stDBState.clear();
		stDBState.setAccountName(" ");
		stDBState.setSnid(n64Snid++);

		mDBSnidAllocState.clear();
		mDBSnidAllocState.readFromProps(&stDBState);
		mDBSnidAllocState.doSQLInsert(dbCore);
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

// update[1/5/2018 Chief]
BOOL SnidAllocateeDBProxy::doUpdateSnidData(SNID_t u64Snid, const char* szAccountName)
{
	__GUARD__;
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Snid, 0);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchRegisterPlayerInSnidRange DBConnection disconnect error");
		return FALSE;
	}

	DBSnidAllocPro stDBState;
	stDBState.clear();
	stDBState.setAccountName(szAccountName);

	mDBSnidAllocState.clear();
	mDBSnidAllocState.readFromProps(&stDBState);
	mDBSnidAllocState.setSnid(u64Snid);
	mDBSnidAllocState.doSQLUpdate(dbCore);

	return FALSE;
	__UNGUARD__;
	return FALSE;
}

#endif