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