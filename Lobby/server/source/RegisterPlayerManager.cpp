#include "stdafx.h"
#include "RegisterPlayerManager.h"
#include "RegisterPlayer.h"
#include "ServerModuleUtils.h"
#include "ModuleManager.h"
#include "ClientGateManager.h"

RegisterPlayerManager::RegisterPlayerManager()
{
	mIsALLPlayerInited = FALSE;
	mNextValidSnid = INVALID_SNID;
	mIsReqeustingDataFromDB = FALSE;
}

RegisterPlayerManager::~RegisterPlayerManager()
{
	if (mPlayerList.GetSize() > 0)
	{
		for (int32 i = 0; i < mPlayerList.GetSize(); ++i)
		{
			mPlayerPool.freeObject(mPlayerList[i]);
		}
		mPlayerList.RemoveAll();
	}
}

BOOL RegisterPlayerManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	QLogSystem::QUICK_LOG
	(
		SLOW_LOG_MSGERROR,
		"sizeof DBRegisterPlayerState=%d",
		sizeof DBRegisterPlayerState
	);

	mAccountPlayerMap.InitTable(MODULE_REGISTERPLAYER_NUM, MAX_ACCOUNT);
	mSnidPlayerMap.InitTable(MODULE_REGISTERPLAYER_NUM);

	mPlayerPool.initPool(MODULE_REGISTERPLAYER_NUM);

	// 算出这个服第一个Snid [1/16/2018 yz]
	mNextValidSnid = SnidUtil::genPlayerSnid(groupID, 0);

	return TRUE;
}

void RegisterPlayerManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void RegisterPlayerManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);

	// 服务器启动之后需要不停去DB拉取注册玩家数据，直到所有DB中的Player全部拉取到内存中 [1/16/2018 yz]
	if (!mIsALLPlayerInited)
	{
		tickFetchDataFromDB();
	}
}

RegisterPlayer* RegisterPlayerManager::getPlayerByAccount(const char* account)
{
	return mAccountPlayerMap.Get(account);
}

RegisterPlayer* RegisterPlayerManager::getPlayerBySnid(SNID_t snid)
{
	return mSnidPlayerMap.Get(snid);
}

void RegisterPlayerManager::initRegisterPlayer(RegisterPlayerArchive& dbArchive)
{
	__GUARD__
	RegisterPlayerArchive regArchive;
	regArchive.mDBState.copyProperty(&dbArchive.mDBState);

	RegisterPlayer* newPlayer = mPlayerPool.allocObject();
	if (NULL == newPlayer)
	{
		return;
	}

	mPlayerList.AddItem(newPlayer);
	mSnidPlayerMap.Add(regArchive.mDBState.getSnid(), newPlayer);
	mAccountPlayerMap.Add(regArchive.mDBState.getAccountName().c_str(), newPlayer);

	newPlayer->initDB(regArchive.mDBState);

	// 检测Snid是否有漏空的情况 [1/16/2018 yz]
	if (regArchive.mDBState.getSnid() > mNextValidSnid)
	{
		for (SNID_t id = mNextValidSnid; id < regArchive.mDBState.getSnid(); ++id)
		{
			mLeakedValidSnids.AddItem(id);
		}
	}

	mNextValidSnid = regArchive.mDBState.getSnid() + 1;

	__UNGUARD__
}

void RegisterPlayerManager::removeReigsterPlayer(RegisterPlayer* player)
{
	__GUARD__
	KCheck(player);

	BOOL found = FALSE;
	for (int32 i = 0; i < mPlayerList.GetSize(); ++i)
	{
		if (mPlayerList[i] == player)
		{
			mPlayerList.RemoveSwap(i);
			found = TRUE;
		}
	}

	if (found)
	{
		mSnidPlayerMap.Remove(player->getSnid());
		mAccountPlayerMap.Remove(player->getAccount());

		player->clear();
		mPlayerPool.freeObject(player);
	}
	__UNGUARD__
}

class __RegisterPlayerManager_rpcCreateNewPlayerArchive_Callback : public RegisterPlayerManager
{
public:
	void callbackSuccess(DataStream& userData)
	{
		RegisterPlayerArchive dbArchive;
		BOOL isFromLogin;
		int64 sessionID;
		dbArchive.unserialize(userData);
		userData.getValue(isFromLogin);
		userData.getValue(sessionID);

		initRegisterPlayer(dbArchive);

		// 如果创建角色的请求是来自登录流程，则应该在角色创建完成后立即开始上线流程 [12/19/2017 yz]
		if (isFromLogin)
		{
			rpcRequestPlayerOnline(dbArchive.mDBState.getAccountName().c_str(), sessionID);
		}
	}

	void callbackOvertime(DataStream& userData)
	{
		
	}
};


void RegisterPlayerManager::requestPlayerCreate(BOOL isFromLogin, const char* account, int64 sessionID, SNID_t u64PlayerID)
{
	RegisterPlayerArchive dbArchive;
	initNewPlayerDB(dbArchive);
	dbArchive.mDBState.setSnid(u64PlayerID);
	dbArchive.mDBState.setSnidDirty(TRUE);
	dbArchive.mDBState.setAccountName(account);
	dbArchive.mDBState.setAccountNameDirty(TRUE);
	dbArchive.mDBState.setCharacterName(account);
	dbArchive.mDBState.setCharacterNameDirty(TRUE);


	// 保留到函数闭包之中，异步回调中使用 [12/19/2017 yz]
	DataStream userData;
	dbArchive.serialize(userData, FALSE);
	userData.pushValue(isFromLogin);
	userData.pushValue(sessionID);

	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__RegisterPlayerManager_rpcCreateNewPlayerArchive_Callback> >(
		ServerModuleUtil::getDBProxyModuleByPlayerSnid(dbArchive.mDBState.getSnid()),
		MODULE_DEFAULT_ID,
		(__RegisterPlayerManager_rpcCreateNewPlayerArchive_Callback*)this
		)
		->rpcCreateNewPlayerArchive(dbArchive)
		->bindCallback(
			&__RegisterPlayerManager_rpcCreateNewPlayerArchive_Callback::callbackSuccess,
			&__RegisterPlayerManager_rpcCreateNewPlayerArchive_Callback::callbackOvertime,
			userData,
			10000
		);
}

BOOL RegisterPlayerManager::rpcRequestPlayerOnline(const char* account, int64 sessionID)
{
	__GUARD__

	// 必须等所有现有注册玩家全部读取到内存之后才能进行登录和创建角色的操作，否则会出现新创建角色与已有角色Snid冲突的问题 [1/16/2018 yz]
	if (!mIsALLPlayerInited)
	{
		return FALSE;
	}

	RegisterPlayer* player = mAccountPlayerMap.Get(account);
	if (!player)
	{
		SNID_t newSnid = allocNewSnid();
		requestPlayerCreate(TRUE, account, sessionID, newSnid);
		return TRUE;
	}

	SNID_t snid = player->getData().getSnid();

	// 已经在线了，不用重复上线 [12/16/2017 yz]
	if (player->isOnline())
	{
		// 通知session已成功上线 [1/16/2018 Chief]
		g_pModuleManager->getExportCallable< IClientSessionExportCallable<RegisterPlayerManager> >(
			ServerModuleUtil::getSessionModuleBySessionID(sessionID),
			sessionID,
			this
			)
			->rpcNotifyPlayerOnline(snid);

		return TRUE;
	}

	g_pModuleManager->getExportCallable< IOnlinePlayerManagerExportCallable<RegisterPlayerManager> >(
		ServerModuleUtil::getOnlineModuleByPlayerSnid(snid),
		MODULE_DEFAULT_ID,
		this
		)
		->rpcReqeustOnline(account, snid, sessionID);  

	return TRUE;
	__UNGUARD__
		return FALSE;
}

BOOL RegisterPlayerManager::rpcUpdatePlayerOnlineState(SNID_t snid, BOOL isOnline)
{
	__GUARD__

	if (!mIsALLPlayerInited)
	{
		return FALSE;
	}

	RegisterPlayer* player = mSnidPlayerMap.Get(snid);
	if (!player)
	{
		// 通知登录的时候找不到玩家，肯定是有问题的，这个时候OnlinePlayer都已经创建了 [12/16/2017 yz]
		return FALSE;
	}

	player->setOnline(isOnline);
	return TRUE;

	__UNGUARD__
		return FALSE;
}

void RegisterPlayerManager::initNewPlayerDB(RegisterPlayerArchive& dbArchive)
{
	__GUARD__;
	dbArchive.clear();
	dbArchive.mDBState.markDirtyFlags();
	__UNGUARD__
}

void RegisterPlayerManager::tickFetchDataFromDB()
{
	__GUARD__;

	if (!mIsReqeustingDataFromDB)
	{
		fetchRegisterPlayerByRange();
	}

	__UNGUARD__;
}

class __RegisterPlayerManager_rpcFetchRegisterPlayerInSnidRange_Callback : public RegisterPlayerManager
{
public:
	void callbackSuccess(BatchRegPlayerArchive& regPlayerArchives)
	{
		for (int32 i = 0; i < regPlayerArchives.mBatchNum; ++i)
		{
			initRegisterPlayer(regPlayerArchives.mBatchPlayerData[i]);
		}

		// 一次申请的批量玩家没有返回申请的数量，说明数据库中的玩家已经被找完了 [1/16/2018 yz]
		if (regPlayerArchives.mBatchNum < BATCH_REGPLAYER_NUM)
		{
			mIsALLPlayerInited = TRUE;
		}

		mIsReqeustingDataFromDB = FALSE;
	}

	void callbackOvertime()
	{
		mIsReqeustingDataFromDB = FALSE;
	}
};

void RegisterPlayerManager::fetchRegisterPlayerByRange()
{
	__GUARD__

	int32 serverID = getGroupID();
	g_pModuleManager->getExportCallable< IDBProxyManagerExportCallable<__RegisterPlayerManager_rpcFetchRegisterPlayerInSnidRange_Callback> >(
		ServerModuleUtil::getModuleNameByTypeAndGroup(EServerModuleType_DBProxy, 0),
		MODULE_DEFAULT_ID,
		(__RegisterPlayerManager_rpcFetchRegisterPlayerInSnidRange_Callback*)this
		)
		->rpcFetchRegisterPlayerInSnidRange(serverID, mNextValidSnid, BATCH_REGPLAYER_NUM)
		->bindCallback(
			&__RegisterPlayerManager_rpcFetchRegisterPlayerInSnidRange_Callback::callbackSuccess,
			&__RegisterPlayerManager_rpcFetchRegisterPlayerInSnidRange_Callback::callbackOvertime,
			20000
		);

	mIsReqeustingDataFromDB = TRUE;
	__UNGUARD__
}

SNID_t RegisterPlayerManager::allocNewSnid()
{
	if (mLeakedValidSnids.GetSize() > 0)
	{
		SNID_t newSnid = mLeakedValidSnids.Get(0);
		mLeakedValidSnids.Remove(0);
		return newSnid;
	}

	SNID_t maxValidSnid = SnidUtil::genPlayerSnid(getGroupID(), MODULE_REGISTERPLAYER_NUM);
	if (mNextValidSnid < maxValidSnid)
	{
		SNID_t newSnid = mNextValidSnid;
		++mNextValidSnid;
		return newSnid;
	}

	return INVALID_SNID;
}

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType RegisterModuleFactory::getType() const
{
	return EServerModuleType_RegisterPlayer;
}

ServerModuleBase* RegisterModuleFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new RegisterPlayerManager;
	return mPtrServerModule;
}

void RegisterModuleFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}