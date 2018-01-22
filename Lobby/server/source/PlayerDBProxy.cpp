#include "stdafx.h"
#include "DBProxyManager.h"
#include "DBOper.h"
#include "PlayerDBProxy.h"
#include "PlayerArchiveStruct.h"
#include "RegisterPlayerStruct.h"

void PlayerDBProxy::doCreateNewPlayerArchive(RegisterPlayerArchive* archive)
{
	__GUARD__

	int32 serverID = SnidUtil::getPlayerServerID(archive->mDBState.getSnid());

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Player, serverID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doCreateNewPlayerArchive DBConnection disconnect error");
		return;
	}

	// 将新的数据insert到数据库中 [12/15/2017 yz]
	mDBModelRegisterPlayerState.clear();
	mDBModelRegisterPlayerState.readFromProps(&archive->mDBState);
	mDBModelRegisterPlayerState.doSQLInsert(dbCore);

	__UNGUARD__
}

BOOL PlayerDBProxy::doSavePlayerArchive(PlayerArchive* archive)
{
	__GUARD__

	int32 serverID = SnidUtil::getPlayerServerID(archive->mDBState.getSnid());

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Player, serverID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doSavePlayerArchive DBConnection disconnect error");
		return FALSE;
	}

	// 存储PlayerState [12/15/2017 yz]
	mDBModelPlayerState.clear();
	mDBModelPlayerState.readFromProps(&archive->mDBState);
	mDBModelPlayerState.setSnid(archive->mDBState.getSnid());
	mDBModelPlayerState.doSQLUpdate(dbCore);

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL PlayerDBProxy::doFetchPlayerArchiveBySnid(SNID_t snid, PlayerArchive* archive)
{
	__GUARD__

	int32 serverID = SnidUtil::getPlayerServerID(snid);
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Player, serverID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchPlayerArchiveBySnid DBConnection disconnect error SNID=%llu", snid);
		return FALSE;
	}

	// 存储PlayerState [12/15/2017 yz]
	mDBModelPlayerState.clear();
	mDBModelPlayerState.setSnid(snid);
	if (mDBModelPlayerState.doSQLSelect(dbCore))
	{
		if (mDBModelPlayerState.getAffectedRowCount() > 0)
		{
			mDBModelPlayerState.doSQLFetch(dbCore, &archive->mDBState);
			return TRUE;
		}
	}
	
	return FALSE;
	__UNGUARD__
		return FALSE;
}

BOOL PlayerDBProxy::doFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end, int32& playerNum)
{
	__GUARD__

	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Player, serverID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	playerNum = 0;

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchPlayerNumInSnidRange error: DBConnection disconnect");
		return FALSE;
	}

	dbCore->Clear();

	SQLString* pQuery = &dbCore->GetQuery();
	if (!pQuery)
	{
		KCheck(FALSE);
	}

	pQuery->Clear();
	pQuery->Parse("select count(*) from t_player where Snid > %llu and Snid < %llu", begin, end);

	if (!dbCore->IsPrepare())
		return FALSE;

	BOOL ret = dbCore->Execute();
	if (!ret)
	{
		QLogSystem::QUICK_LOG
		(
			SLOW_LOG_GL_DBCENTER,
			"DBModel Error: select from table t_player failed"
		);
		return FALSE;
	}

	if (dbCore->Get_AffectedRowCount() > 0)
	{
		if (!dbCore->Fetch())
			return FALSE;
		const char* str = dbCore->GetColData(0);
		playerNum = StringUtil::parseInt(str);
		return TRUE;
	}

	return FALSE;
	__UNGUARD__
	return FALSE;
}

BOOL PlayerDBProxy::doFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit, BatchRegPlayerArchive* batchPlayerData)
{
	__GUARD__
	DBOper* dbOper = mDBManager->getDBConnByCategory(EDBCategory_Player, serverID);
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doFetchRegisterPlayerInSnidRange DBConnection disconnect error");
		return FALSE;
	}
	batchPlayerData->clear();

	mDBModelRegisterPlayerState.clear();
	mDBModelRegisterPlayerState.setSnidRange(begin, limit);
	if (mDBModelRegisterPlayerState.doSQLSelect(dbCore))
	{
		if (mDBModelRegisterPlayerState.getAffectedRowCount() > 0)
		{
			for (int32 i = 0; i < BATCH_REGPLAYER_NUM && i < mDBModelRegisterPlayerState.getAffectedRowCount(); ++i)
			{
				mDBModelRegisterPlayerState.doSQLFetch(dbCore, &batchPlayerData->mBatchPlayerData[batchPlayerData->mBatchNum].mDBState);
				batchPlayerData->mBatchNum++;
			}
			
			return TRUE;
		}
	}

	return FALSE;
	__UNGUARD__
	return FALSE;
}