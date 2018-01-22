#ifndef __REGISTER_PLAYER_MANAGER_H__
#define __REGISTER_PLAYER_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "StringHashMap.h"
#include "ServerModuleExportInterface.h"
#include "CommonObjectPool.h"
#include "ServerModuleUtils.h"
#include "ServerModuleManager.h"

class RegisterPlayer;

class RegisterPlayerManager
	: public ServerModuleBase
	, public IRegisterPlayerManager
{
public:
	RegisterPlayerManager();
	~RegisterPlayerManager();

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	RegisterPlayer* getPlayerByAccount(const char* account);
	RegisterPlayer* getPlayerBySnid(SNID_t snid);

	// 根据DB数据初始化注册玩家对象 [12/19/2017 yz]
	void initRegisterPlayer(RegisterPlayerArchive& dbArchive);

	void removeReigsterPlayer(RegisterPlayer* player);

	// 请求某个玩家上线，然后发送数据到OnlinePlayerManager去创建OnlinePlayer [12/13/2017 yz]
	void requestPlayerCreate(BOOL isFromLogin, const char* account, int64 sessionID, SNID_t u64PlayerID);
	
	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------
	virtual BOOL rpcRequestPlayerOnline(const char* account, int64 sessionID);
	virtual BOOL rpcUpdatePlayerOnlineState(SNID_t snid, BOOL isOnline);

	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------
	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }
protected:
	void initNewPlayerDB(RegisterPlayerArchive& dbArchive);

	// 启动时分多次向DB获取注册玩家数据 [12/19/2017 yz]
	void tickFetchDataFromDB();

	// 一次性从DBProxy获取一个SNID区间的多个玩家注册数据 [1/2/2018 yz]
	void fetchRegisterPlayerByRange();

	// 分配一个新的未使用的SNID给新注册玩家 [1/16/2018 yz]
	SNID_t allocNewSnid();

protected:
	CommonObjectPool<RegisterPlayer> mPlayerPool;
	TStaticArray<RegisterPlayer*, MODULE_REGISTERPLAYER_NUM> mPlayerList;

	// 是否已经从DB中获取了全部的注册玩家 [12/19/2017 yz]
	BOOL mIsALLPlayerInited;
	// 是否正在向DBProxy申请数据 [1/16/2018 yz]
	BOOL mIsReqeustingDataFromDB;

	// 下一个可分配的Snid [1/16/2018 yz]
	SNID_t mNextValidSnid;

	// 之前注册的玩家Snid中有不连续的，将不连续的Snid先收集起来，分配的时候先分配这些漏掉的Snid [1/16/2018 yz]
	TDynaArray<SNID_t> mLeakedValidSnids;

protected:
	typedef TStringHashMap<RegisterPlayer*>		RegPlayerStrHashMap;
	typedef TIntegerHashMap<RegisterPlayer*>	RegPlayerIntHashMap;

	RegPlayerStrHashMap	mAccountPlayerMap;			// 通过账号查找角色 [12/13/2017 yz]
	RegPlayerIntHashMap	mSnidPlayerMap;				// 通过SNID查找角色 [12/13/2017 yz]
	RegPlayerStrHashMap	mNamePlayerMap;				// 通过名字查找玩家 [1/4/2018 Chief]
};

//----------------------------------------------------------------------
//factory
//----------------------------------------------------------------------

class RegisterModuleFactory : public ServerModuleFactory
{
	RegisterPlayerManager* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};

#endif