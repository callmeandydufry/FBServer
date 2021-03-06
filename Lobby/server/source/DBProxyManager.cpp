#include "stdafx.h"
#include "DBProxyManager.h"
#include "DBOper.h"
#include "PlayerDBProxy.h"
#include "RegisterPlayerStruct.h"
#include "MailDBProxy.h"

DBProxyManager::DBProxyManager()
{
	mVersionNum = 0;
	mDBConnNum = 0;
	memset(mDBConnectionInfos, 0, sizeof(mDBConnectionInfos));
	memset(mCategoryConnMap, 0, sizeof(mCategoryConnMap));
	memset(mDBConnections, 0, sizeof(mDBConnections));
}

DBProxyManager::~DBProxyManager()
{

}

BOOL DBProxyManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	loadDBConfig();

	for (int32 i = 0; i < mDBConnNum; ++i)
	{
		mDBConnections[i] = new DBOper();
		mDBConnections[i]->Init(mVersionNum,
			mDBConnectionInfos[i].mIP.c_str(),
			mDBConnectionInfos[i].mPort,
			mDBConnectionInfos[i].mDBName.c_str(),
			mDBConnectionInfos[i].mDBUser.c_str(),
			mDBConnectionInfos[i].mDBPwd.c_str());

		CheckVersion(i);
	}

	mPlayerDBProxy			= new PlayerDBProxy(this);
	mMailDBProxy			= new MailDBProxy(this);

	return TRUE;
}

void DBProxyManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
	if (mPlayerDBProxy)
	{
		KSafeDelete(mPlayerDBProxy);
		mPlayerDBProxy = NULL;
	}
	
}

void DBProxyManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);

	// 检测DB连接是否已断开，断开后重新连接 [12/13/2017 yz]
	for (int32 i = 0; i < mDBConnNum; ++i)
	{
		mDBConnections[i]->CheckDBConnection();
	}
}

DBOper* DBProxyManager::getDBConnByCategory(EDBDataCategory dataCategory, int32 groupIndex)
{
	if (groupIndex < 0 || groupIndex >= DB_MAX_CATEGORY_GROUP)
		return NULL;
	int32 connIndex = mCategoryConnMap[dataCategory][groupIndex];
	KCheck(mDBConnections[connIndex]);
	return mDBConnections[connIndex];
}

//---------------------------------------------------------------------------------------
// t_player
//---------------------------------------------------------------------------------------
PlayerArchive& DBProxyManager::rpcFetchOnlinePlayerArchive(SNID_t snid)
{
	__GUARD__
		mReturnPlayerArchive.clear();
	mPlayerDBProxy->doFetchPlayerArchiveBySnid(snid, &mReturnPlayerArchive);
	return mReturnPlayerArchive;
	__UNGUARD__
		return mReturnPlayerArchive;
}

void DBProxyManager::rpcCreateNewPlayerArchive(RegisterPlayerArchive& archive)
{
	__GUARD__
	mPlayerDBProxy->doCreateNewPlayerArchive(&archive);
	__UNGUARD__
}

BOOL DBProxyManager::rpcSavePlayerArchive(PlayerArchive& archive)
{
	__GUARD__
		return mPlayerDBProxy->doSavePlayerArchive(&archive);
	__UNGUARD__
		return FALSE;
}

int32 DBProxyManager::rpcFetchPlayerNumInSnidRange(int32 serverID, SNID_t begin, SNID_t end)
{
	__GUARD__
	int32 playerNum = 0;
	mPlayerDBProxy->doFetchPlayerNumInSnidRange(serverID, begin, end, playerNum);
	return playerNum;
	__UNGUARD__
	return INVALID_ID;
}

BatchRegisterPlayerArchive& DBProxyManager::rpcFetchRegisterPlayerInSnidRange(int32 serverID, SNID_t begin, int32 limit)
{
	mBatchPlayerArchive.clear();
	__GUARD__;
	mPlayerDBProxy->doFetchRegisterPlayerInSnidRange(serverID, begin, limit, &mBatchPlayerArchive);
	return mBatchPlayerArchive;
	__UNGUARD__
	return mBatchPlayerArchive;
}

//---------------------------------------------------------------------------------------
// loadDBConfig
//---------------------------------------------------------------------------------------
void DBProxyManager::loadDBConfig()
{
	__GUARD__;
	Configer	ini(FILE_DBCONFIG);

	mVersionNum = ini.Get_Property_Int("ALL", "DBVerNum");
	mDBConnNum = ini.Get_Property_Int("ALL", "DBConnNum");

	char szDBIP[IP_SIZE] = { 0 };
	char szDBName[DB_MAX_STR_LEN] = { 0 };
	char szDBUser[DB_MAX_USER_LEN] = { 0 };
	char szPassword[DB_MAX_USER_LEN * 2 + 3] = { 0 };
	char szPassword_Decoded[DB_MAX_USER_LEN] = { 0 };
	uint32 uDBPort = 0;

	for (int32 i = 0; i < mDBConnNum; ++i)
	{
		String systemName = StringUtil::printf("System%d", i);
		ini.GetText(systemName.c_str(), "DBIP", szDBIP, IP_SIZE);
		uDBPort = (uint32)(ini.Get_Property_Int(systemName.c_str(), "DBPort"));
		ini.GetText(systemName.c_str(), "DBName", szDBName, DB_MAX_STR_LEN);
		ini.GetText(systemName.c_str(), "DBUser", szDBUser, DB_MAX_USER_LEN);
		ini.GetText(systemName.c_str(), "DBPassword", szPassword, DB_MAX_USER_LEN * 2 + 3);

		DecodeDBPassword(szPassword, szPassword_Decoded);

		mDBConnectionInfos[i].mIP = szDBIP;
		mDBConnectionInfos[i].mPort = uDBPort;
		mDBConnectionInfos[i].mDBName = szDBName;
		mDBConnectionInfos[i].mDBUser = szDBUser;
		mDBConnectionInfos[i].mDBPwd = szPassword_Decoded;
	}

	{
		EDBDataCategory dbCategory = EDBCategory_Player;

		int32 groupNum = ini.Get_Property_Int("Player", "GroupNum", 0);
		for (int32 i = 0; i < groupNum; ++i)
		{
			String groupName = StringUtil::printf("Group%d", i);
			mCategoryConnMap[dbCategory][i] = ini.Get_Property_Int("Player", groupName.c_str());
		}
	}

	{
		EDBDataCategory dbCategory = EDBCategory_Friend;

		int32 groupNum = ini.Get_Property_Int("Friend", "GroupNum", 0);
		for (int32 i = 0; i < groupNum; ++i)
		{
			String groupName = StringUtil::printf("Group%d", i);
			mCategoryConnMap[dbCategory][i] = ini.Get_Property_Int("Friend", groupName.c_str());
		}
	}

	{
		EDBDataCategory dbCategory = EDBCategory_Guild;

		int32 groupNum = ini.Get_Property_Int("Guild", "GroupNum", 0);
		for (int32 i = 0; i < groupNum; ++i)
		{
			String groupName = StringUtil::printf("Group%d", i);
			mCategoryConnMap[dbCategory][i] = ini.Get_Property_Int("Guild", groupName.c_str());
		}
	}

	{
		EDBDataCategory dbCategory = EDBCategory_Mail;

		int32 groupNum = ini.Get_Property_Int("Mail", "GroupNum", 0);
		for (int32 i = 0; i < groupNum; ++i)
		{
			String groupName = StringUtil::printf("Group%d", i);
			mCategoryConnMap[dbCategory][i] = ini.Get_Property_Int("Guild", groupName.c_str());
		}
	}

	__UNGUARD__

}

BOOL DBProxyManager::DecodeDBPassword(const char* pIn, char* pOut)
{
	__GUARD__;
	char pOut_Binary[DB_MAX_USER_LEN * 2 + 2] = { 0 };
	int32 idx = 0;
	uint32 OutLenTmp = 0;

	int32 nStartIndex = pIn[0] % (DB_MAX_USER_LEN * 2 + 2);
	if (nStartIndex == 0)
		nStartIndex = 1;

	char szLen_Binary[3] = { 0 };
	szLen_Binary[0] = pIn[DB_MAX_USER_LEN * 2];
	szLen_Binary[1] = pIn[DB_MAX_USER_LEN * 2 + 1];
	char PassLen = 0;

	Str2Binary(szLen_Binary, 2, (char*)&PassLen, 2, OutLenTmp);

	memset(pOut_Binary, 0, DB_MAX_USER_LEN * 2 + 2);
	for (int _Idx = nStartIndex; idx < PassLen * 2; _Idx++)
	{
		if (_Idx >= DB_MAX_USER_LEN * 2)
			_Idx = 1;
		pOut_Binary[idx] = pIn[_Idx];
		idx++;
	}

	Str2Binary(pOut_Binary, (uint32)strlen(pOut_Binary), pOut, DB_MAX_USER_LEN * 2 + 2, OutLenTmp);

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

//---------------------------------------------------------------------------------------
// t_version
//---------------------------------------------------------------------------------------
BOOL DBProxyManager::CheckVersion(int32 nConnNum)
{
	__GUARD__;
	
	DBOper* dbOper = mDBConnections[nConnNum];
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doCheckVersion error");
		return FALSE;
	}

	// 检查版本 [12/27/2017 Chief]
	DBVersionPro stDBVerPro;
	stDBVerPro.clear();

	mDBVersionState.clear();
	if (mDBVersionState.doSQLSelect(dbCore))
	{
		if (mDBVersionState.getAffectedRowCount() > 0)
		{
			mDBVersionState.doSQLFetch(dbCore, &stDBVerPro);
		}
	}

	if (stDBVerPro.getverNum() != mVersionNum)
	{
		KCheckEx(FALSE, "DBVersion is error");
	}

	return TRUE;

	__UNGUARD__
	return FALSE;
}

//---------------------------------------------------------------------------------------
// t_global
//---------------------------------------------------------------------------------------
int32 DBProxyManager::rpcFetchGlobalVal(int32 nGroup, int32 eType)
{
	int32 nVal = 0;
	__GUARD__;

	if (nGroup < 0 || nGroup >= DB_MAX_CONNECTION)
		return nVal;

	DBOper* dbOper = mDBConnections[nGroup];
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doCheckVersion error");
		return nVal;
	}

	DBGlobalPro stDBGlobalPro;
	stDBGlobalPro.clear();

	mGlobalState.clear();
	mGlobalState.setCheckType((EProFlag)eType);
	if (mGlobalState.doSQLSelect(dbCore))
	{
		if (mGlobalState.getAffectedRowCount() > 0)
		{
			mGlobalState.doSQLFetch(dbCore, &stDBGlobalPro);
			nVal = stDBGlobalPro.getGlobalVal();
		}
	}

	return nVal;
	__UNGUARD__;
	return nVal;
}

void DBProxyManager::rpcSaveGlobalVal(int32 nGroup, int32 eType, int32 nVal)
{
	__GUARD__;
	if (nGroup < 0 || nGroup >= DB_MAX_CONNECTION)
		return;

	if (nGroup < 0 || nGroup >= DB_MAX_CONNECTION)
		return;

	DBOper* dbOper = mDBConnections[nGroup];
	KCheck(dbOper);
	DBCore* dbCore = dbOper->GetInterface();
	KCheck(dbCore);

	if (!dbCore->IsConnected())
	{
		QLogSystem::QUICK_LOG(SLOW_LOG_GL_DBCENTER, "PlayerDBProxy::doCheckVersion error");
		return;
	}

	mGlobalState.clear();
	DBGlobalPro stDBGlobalPro;
	stDBGlobalPro.clear();
	stDBGlobalPro.setGlobalIndex((int32)eType);
	stDBGlobalPro.setGlobalVal(nVal);
	mGlobalState.clear();
	mGlobalState.readFromProps(&stDBGlobalPro);
	mGlobalState.doSQLUpdate(dbCore);

	return;
	__UNGUARD__
		return;
}

//---------------------------------------------------------------------------------------
// 获取邮件总数 [2/5/2018 Chief]
//---------------------------------------------------------------------------------------
tagMailsNum& DBProxyManager::rpcRequestAllMailsNum(int32 nGroup)
{
	mStMailsNum.clear();
	__GUARD__;

	mStMailsNum.setBaseMailNum(mMailDBProxy->doFetchMailNum(EDBT_Base, nGroup));
	mStMailsNum.setAttachmentMailNum(mMailDBProxy->doFetchMailNum(EDBT_Attach, nGroup));
	mStMailsNum.setContentMailNum(mMailDBProxy->doFetchMailNum(EDBT_Content, nGroup));
	mStMailsNum.setSystemMailNum(mMailDBProxy->doFetchMailNum(EDBT_System, nGroup));

	return mStMailsNum;

	__UNGUARD__;
	return mStMailsNum;
}

//---------------------------------------------------------------------------------------
// 批量获取邮件数据 [2/5/2018 Chief]
//---------------------------------------------------------------------------------------
BatchBaseMailArchive& DBProxyManager::rpcFetchBaseMailData(int32 nStartID, int32 nSelectNum, int32 nGroupID)
{
	mBatchBaseMailData.clear();
	__GUARD__;

	mMailDBProxy->doFetchBaseMailsInfo(mBatchBaseMailData, nGroupID, nStartID, nSelectNum);

	return mBatchBaseMailData;

	__UNGUARD__;
	return mBatchBaseMailData;
}

BatchAttachmentMailArchive& DBProxyManager::rpcFetchAttachmentMailData(int32 nStartID, int32 nSelectNum, int32 nGroupID)
{
	mBatchAttachmentMailData.clear();
	__GUARD__;

	mMailDBProxy->doFetchAttachmentMailsInfo(mBatchAttachmentMailData, nGroupID, nStartID, nSelectNum);

	return mBatchAttachmentMailData;

	__UNGUARD__;
	return mBatchAttachmentMailData;
}

BatchContentMailArchive& DBProxyManager::rpcFetchContentMailData(int32 nStartID, int32 nSelectNum, int32 nGroupID)
{
	mBatchContentMailData.clear();
	__GUARD__;

	mMailDBProxy->doFetchContentMailsInfo(mBatchContentMailData, nGroupID, nStartID, nSelectNum);

	return mBatchContentMailData;

	__UNGUARD__;
	return mBatchContentMailData;
}

BatchSystemMailArchive& DBProxyManager::rpcFetchSystemMailData(int32 nStartID, int32 nSelectNum, int32 nGroupID)
{
	mBatchSystemMailData.clear();
	__GUARD__;

	mMailDBProxy->doFetchSystemMailsInfo(mBatchSystemMailData, nGroupID, nStartID, nSelectNum);

	return mBatchSystemMailData;

	__UNGUARD__;
	return mBatchSystemMailData;
}

//---------------------------------------------------------------------------------------
// 操作邮件 [2/7/2018 Chief]
//---------------------------------------------------------------------------------------
BOOL DBProxyManager::rpcOperatorBaseMailData(BaseMailArchive& stArchive, int32 nGroupID, int32 nOper)
{
	__GUARD__;

	if (EDBO_Insert == nOper)
	{
		mMailDBProxy->doInsertBaseMailsInfo(stArchive, nGroupID);
	}
	else if (EDBO_Update == nOper)
	{
		mMailDBProxy->doUpdateBaseMailsInfo(stArchive, nGroupID);
	}

	return TRUE;
	__UNGUARD__;
	return FALSE;
}

void DBProxyManager::rpcOperatorAttachmentMailData(AttachmentMailArchive& stArchive, int32 nGroupID, int32 nOper)
{
	__GUARD__;

	if (EDBO_Insert == nOper)
	{
		mMailDBProxy->doInsertAttachmentMailsInfo(stArchive, nGroupID);
	}
	else if (EDBO_Update == nOper)
	{
		mMailDBProxy->doUpdateAttachmentMailsInfo(stArchive, nGroupID);
	}

	__UNGUARD__;
}

void DBProxyManager::rpcOperatorContentMailData(ContentMailArchive& stArchive, int32 nGroupID, int32 nOper)
{
	__GUARD__;

	if (EDBO_Insert == nOper)
	{
		mMailDBProxy->doInsertContentMailsInfo(stArchive, nGroupID);
	}
	else if (EDBO_Update == nOper)
	{
		mMailDBProxy->doUpdateContentMailsInfo(stArchive, nGroupID);
	}

	__UNGUARD__;
}

void DBProxyManager::rpcOperatorSystemMailData(SystemMailArchive& stArchive, int32 nGroupID, int32 nOper)
{
	__GUARD__;

	if (EDBO_Insert == nOper)
	{
		mMailDBProxy->doInsertSystemMailsInfo(stArchive, nGroupID);
	}
	else if (EDBO_Update == nOper)
	{
		mMailDBProxy->doUpdateSystemMailsInfo(stArchive, nGroupID);
	}

	__UNGUARD__;
}

//---------------------------------------------------------------------------------------
// factory
//---------------------------------------------------------------------------------------

EServerModuleType DBProxyModuleFactory::getType() const
{
	return EServerModuleType_DBProxy;
}

ServerModuleBase* DBProxyModuleFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new DBProxyManager;
	return mPtrServerModule;
}

void DBProxyModuleFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}