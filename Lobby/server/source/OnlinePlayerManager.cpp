#include "stdafx.h"
#include "OnlinePlayerManager.h"
#include "OnlinePlayer.h"

OnlinePlayerManager::OnlinePlayerManager()
{

}

OnlinePlayerManager::~OnlinePlayerManager()
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

BOOL OnlinePlayerManager::initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID)
{
	ServerModuleBase::initServerModule(moduleName, this, groupID);

	mAccountPlayerMap.InitTable(MODULE_ONLINEPLAYER_NUM, MAX_ACCOUNT);
	mSnidPlayerMap.InitTable(MODULE_ONLINEPLAYER_NUM);

	mPlayerPool.initPool(MODULE_ONLINEPLAYER_NUM);

	return TRUE;
}

void OnlinePlayerManager::uinitServerModule()
{
	ServerModuleBase::uinitServerModule();
}

void OnlinePlayerManager::tickModule(uint32 uTick)
{
	ServerModuleBase::tickModule(uTick);

	TDynaArray<OnlinePlayer*> destroyList;
	for (int32 i = 0; i < mPlayerList.GetSize(); i++)
	{
		if (mPlayerList[i]->isInvalid())
		{
			destroyList.AddItem(mPlayerList[i]);
		}
	}

	for (int32 i = 0; i < destroyList.GetSize(); i++)
	{
		removeInvalidPlayer(destroyList[i]);
	}
}

OnlinePlayer* OnlinePlayerManager::getPlayerByAccount(const char* account)
{
	return mAccountPlayerMap.Get(account);
}

OnlinePlayer* OnlinePlayerManager::getPlayerBySnid(SNID_t snid)
{
	return mSnidPlayerMap.Get(snid);
}

void OnlinePlayerManager::removeInvalidPlayer(OnlinePlayer* player)
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

		player->uninit();
		mPlayerPool.freeObject(player);
	}
	
	__UNGUARD__
}

BOOL OnlinePlayerManager::rpcReqeustOnline(const char* account, SNID_t snid, int64 sessionID)
{
	__GUARD__
	OnlinePlayer* player = mSnidPlayerMap.Get(snid);
	if (NULL != player)
	{
		if (player->getOnlineState() < OnlinePlayer::EOnlineState_WaitDBData)
		{
			player->requestDBData();
		}
		// 玩家已经在线了 [12/15/2017 yz]
		return TRUE;
	}

	OnlinePlayer* newPlayer = mPlayerPool.allocObject();
	if (NULL == newPlayer)
	{
		return FALSE;
	}

	mPlayerList.AddItem(newPlayer);
	mSnidPlayerMap.Add(snid, newPlayer);
	mAccountPlayerMap.Add(account, newPlayer);

	newPlayer->init(this, account, snid);
	newPlayer->bindSessionID(sessionID);
	newPlayer->requestDBData();

	return TRUE;
	__UNGUARD__
		return FALSE;
}

void OnlinePlayerManager::setPlayerOnline(OnlinePlayer* pPlayer, SNID_t snid, int64 sessionID)
{
	__GUARD__

	mSnidPlayerMap.Add(snid, pPlayer);

	__UNGUARD__
}

BOOL OnlinePlayerManager::rpcRequestOffline(SNID_t snid)
{
	__GUARD__
	OnlinePlayer* player = mSnidPlayerMap.Get(snid);
	if (NULL == player)
	{
		return TRUE;
	}

	player->leaveOnlineState();

	return TRUE;
	__UNGUARD__
	return FALSE;
}

void OnlinePlayerManager::rpcRequestPlayerExtData(SNID_t u64PlayerID)
{
	__GUARD__;

	OnlinePlayer* player = mSnidPlayerMap.Get(u64PlayerID);
	if (NULL == player)
	{
		return ;
	}

	player->requestPlayerDetialData();

	__UNGUARD__;
}

//----------------------------------------------------------------------
// factory
//----------------------------------------------------------------------

EServerModuleType OnlinePlayerFactory::getType() const
{
	return EServerModuleType_OnlinePlayer;
}

ServerModuleBase* OnlinePlayerFactory::createModule(EServerModuleType type, int32 group)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}

	mPtrServerModule = new OnlinePlayerManager;
	return mPtrServerModule;
}

void OnlinePlayerFactory::destroyModule(ModuleBase* module)
{
	if (mPtrServerModule)
	{
		delete(mPtrServerModule);
		mPtrServerModule;
	}
}