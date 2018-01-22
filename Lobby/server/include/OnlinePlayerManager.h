#ifndef __ONLINE_PLAYER_MANAGER_H__
#define __ONLINE_PLAYER_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "StringHashMap.h"
#include "CommonObjectPool.h"
#include "ServerModuleUtils.h"
#include "ServerModuleExportInterface.h"
#include "ServerModuleManager.h"

class OnlinePlayer;

class OnlinePlayerManager
	: public ServerModuleBase
	, public IOnlinePlayerManager
{
public:
	OnlinePlayerManager();
	~OnlinePlayerManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	OnlinePlayer* getPlayerByAccount(const char* account);
	OnlinePlayer* getPlayerBySnid(SNID_t snid);

	// 清理状态错误的玩家 [12/18/2017 yz]
	void removeInvalidPlayer(OnlinePlayer* player);

	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------
	// 玩家上线流程，去DB获取玩家完整数据，然后初始化OnlinePlayer对象 [12/13/2017 yz]
	virtual BOOL rpcReqeustOnline(const char* account, SNID_t snid, int64 sessionID);
	virtual BOOL rpcRequestOffline(SNID_t snid);
	virtual void rpcRequestPlayerExtData(SNID_t u64PlayerID);
	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }

public:
	void setPlayerOnline(OnlinePlayer* pPlayer, SNID_t snid, int64 sessionID);

protected:
	TStaticArray<OnlinePlayer*, MODULE_ONLINEPLAYER_NUM> mPlayerList;
	CommonObjectPool<OnlinePlayer> mPlayerPool;

	// 通过账号查找角色 [12/13/2017 yz]
	TStringHashMap<OnlinePlayer*> mAccountPlayerMap;
	// 通过SNID查找角色 [12/13/2017 yz]
	TIntegerHashMap<OnlinePlayer*> mSnidPlayerMap;
};

class OnlinePlayerFactory : public ServerModuleFactory
{
	OnlinePlayerManager* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};
#endif