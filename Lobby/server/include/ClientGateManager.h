#ifndef __CLIENT_GATE_MANAGER_H__
#define __CLIENT_GATE_MANAGER_H__

#include "TypeDefine.h"
#include "ServerModuleBase.h"
#include "ServerModuleExportInterface.h"
#include "Connector.h"
#include "ConnectorManager_Epoll.h"
#include "ConnectorManager_Select.h"
#include "ServerModuleUtils.h"
#include "CommonObjectPool.h"
#include "StringHashMap.h"
#include "ServerModuleManager.h"
#include "MsgTransDefine.h"

class IncomingConnectorManager;
class SessionConnectorManager;
class ClientSession;

struct tagIllegalIP
{
	FixedString<IP_SIZE>	mIP;
	int32 mNum;
	uint32 mLastFailTimer;
};

class ClientGateManager
	: public ServerModuleBase
	, public IClientGateManager
{
public:
	ClientGateManager();
	~ClientGateManager();

	virtual BOOL						initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void						uinitServerModule();
	virtual void						tickModule(uint32 uTick);

	IncomingConnectorManager*			getIncomingConnectorManager() const { return mIncomingConnectorManager; }
	SessionConnectorManager*			getSessionConnectorManager() const { return mSessionConnectorManager; }

	int32								allocSessionID();

	ClientSession*						createNewSession(const char* account, const char* token);
	void								destroySession(ClientSession* session);
	virtual int64						getComponentId() { return MODULE_DEFAULT_ID; }
	
public:

	//---------------------------------------------------------------------------------------
	// RPC远程接口定义 START [12/16/2017 yz]
	//---------------------------------------------------------------------------------------

	virtual uint64						rpcCreateRobotNewSession(const char* account, const char* token);  // server to server

	// 世界聊天 [1/15/2018 Chief]
	virtual uint64						rpcS2CSynWorldChatData(const char* szMsg, uint32 chatID, SNID_t u64Sender);

	//---------------------------------------------------------------------------------------
	// RPC远程接口定义 END [12/16/2017 yz]
	//---------------------------------------------------------------------------------------

public:
	BOOL								checkSessionOnlineBySnid(SNID_t u64PlayerID);
	uint64								getSessionIDBySnid(SNID_t u64PlayerID);

	// todo 获得非法ip、每10秒所有非法ip值--，==0则清除，保证不频繁的失败登录，并记录登录失败log [1/24/2018 Chief]
	// todo session中获得当前登录的时间&& 如何记录失败登录的时间，失败后session会清理，sessionmgr进行缓存，新增结构体 ip/num/timer
	// todo mgr中tick，登录时间在一分钟之内，num++
	BOOL								getIllegalIP(FixedString<IP_SIZE> ip);
protected:
	uint32								mIncomingPort;

	// 
	ConnectorPool						mConnectorPool;

	IncomingConnectorManager*			mIncomingConnectorManager;
	SessionConnectorManager*			mSessionConnectorManager;

	// 已分配的会话id [12/23/2017 yz]
	int32								mAllocedSessionID;

	TStaticArray<ClientSession*, MODULE_GATESESSION_NUM> mSessionList;
	CommonObjectPool<ClientSession>		mSessionPool;

	// 通过账号查找角色 [12/13/2017 yz]
	TStringHashMap<ClientSession*>		mAccountSessionMap;
	// 通过SNID查找角色 [12/13/2017 yz]
	TIntegerHashMap<ClientSession*>		mSessionIDMap;

	// 非法ip [1/24/2018 Chief]
	CommonObjectPool<tagIllegalIP>		mIllegalIPList;
	TStringHashMap<tagIllegalIP*>		mIllegalIPMap;
};

//---------------------------------------------------------------------------------------
// todo factory [1/11/2018 Chief]
//---------------------------------------------------------------------------------------
class ClientGateModuleFactory : public ServerModuleFactory
{
	ClientGateManager*					mPtrServerModule;
public:
	virtual EServerModuleType			getType() const;
	virtual ServerModuleBase*			createModule(EServerModuleType type, int32 group);
	virtual void						destroyModule(ModuleBase* module);
};

#endif