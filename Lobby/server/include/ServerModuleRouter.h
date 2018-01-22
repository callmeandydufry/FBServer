#ifndef __SERVER_MODULE_ROUTER_H__
#define __SERVER_MODULE_ROUTER_H__

#include "ServerModuleExportInterface.h"
#include "ServerModuleUtils.h"
#include "ServerModuleBase.h"
#include "ServerModuleManager.h"

class ServerModuleRouter
	: public ServerModuleBase
	, public IServerModuleRouter
{
public:
	ServerModuleRouter()
	{

	}

	~ServerModuleRouter()
	{

	}

	virtual BOOL initServerModule(int32 moduleName, ModuleComponent* defaultComponent, int32 groupID);
	virtual void uinitServerModule();
	virtual void tickModule(uint32 uTick);

	virtual int64 getComponentId() { return MODULE_DEFAULT_ID; }

	void onRpcSyncLocal2RemoteModuleRegListSuccess(ModuleRegList& regList);

	void onRpcSyncLocal2RemoteModuleRegListOvertime();

	// 普通服务器调用此函数向协调服务器同步本地的模块列表 [12/5/2017 yz]
	void syncRemoteModuleRegInfo();

	// 检测是否所有的模块都已注册完成 [1/12/2018 yz]
	BOOL checkAllModuleRegistered();

	//----------------------------------------------------------------------
	//RPC调用接口 START
	//----------------------------------------------------------------------
	virtual ModuleRegList rpcSyncLocal2RemoteModuleRegList(ModuleRegList& regList);
	//----------------------------------------------------------------------
	//RPC调用接口 END
	//----------------------------------------------------------------------

protected:
	HASH_MAP<std::string, ModuleRegisterInfo> mOnlineModuleInfos;
};

class RouterModuleFactory : public ServerModuleFactory
{
	ServerModuleRouter* mPtrServerModule;
public:
	virtual EServerModuleType getType() const;
	virtual ServerModuleBase* createModule(EServerModuleType type, int32 group);
	virtual void destroyModule(ModuleBase* module);
};

#endif